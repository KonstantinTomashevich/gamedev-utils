#include "Urho3DHexagonalGrid.hpp"
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>

#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <GraphPathfinding/GraphPathfinding.hpp>
#include <Urho3D/Scene/SceneEvents.h>

URHO3D_DEFINE_APPLICATION_MAIN (Urho3DHexagonalGrid)
class ReferenceCountedString : public ReferenceCounted
{
public:
    ReferenceCountedString (const Urho3D::String &content) : ReferenceCounted (), Content (content) {}
    virtual ~ReferenceCountedString () = default;

    Urho3D::String Content;
};

Urho3DHexagonalGrid::Urho3DHexagonalGrid (Urho3D::Context *context) :
        Urho3D::Application (context),
        scene_ (nullptr),
        ball_ (nullptr),
        debugCamera_ (nullptr),
        grid_ (nullptr),

        moving_ (false),
        currentWayointIndex_ (0),
        waypoints_ (),
        waypointsSpheres_ ()
{

}

void Urho3DHexagonalGrid::Setup ()
{
    Application::Setup ();
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
    engineParameters_[Urho3D::EP_LOG_NAME] = "Urho3DHexagonalGrid.log";
    engineParameters_[Urho3D::EP_WINDOW_TITLE] = "Urho3D Hexagonal Grid";
    engineParameters_[Urho3D::EP_WINDOW_RESIZABLE] = true;
}

void Urho3DHexagonalGrid::Start ()
{
    Application::Start ();
    auto *input = context_->GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    scene_ = new Urho3D::Scene (context_);
    SetupLight ();
    SetupCamera ();

    LoadGrid ();
    SetupGrid ();

    ball_ = CreateBall ();
    Teleport (grid_->EncodeCellPosition (0, 0), ball_);

    SubscribeToEvent (Urho3D::E_MOUSEBUTTONUP, URHO3D_HANDLER (Urho3DHexagonalGrid, HandleMouseClick));
    SubscribeToEvent (scene_, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (Urho3DHexagonalGrid, HandleSceneUpdate));
}

void Urho3DHexagonalGrid::Stop ()
{
    Application::Stop ();
    scene_->Clear ();
    delete scene_;
    delete debugCamera_;
    delete grid_;
}

void Urho3DHexagonalGrid::SetupLight ()
{
    scene_->CreateComponent <Urho3D::Octree> ();
    Urho3D::Node *lightNode = scene_->CreateChild ("Light");
    lightNode->Pitch (60.0f, Urho3D::TS_WORLD);
    lightNode->Yaw (30.0f, Urho3D::TS_WORLD);

    auto *light = lightNode->CreateComponent <Urho3D::Light> ();
    light->SetLightType (Urho3D::LIGHT_DIRECTIONAL);
}

void Urho3DHexagonalGrid::SetupCamera ()
{
    debugCamera_ = new ThirdPersonDebugCamera (context_);
    Urho3D::Node *cameraNode = scene_->CreateChild ("Camera");

    cameraNode->SetPosition ({0.0f, 7.5f, -7.5f});
    cameraNode->Pitch (45.0f);
    debugCamera_->SetupCamera (cameraNode);
}

void Urho3DHexagonalGrid::LoadGrid ()
{
    auto *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    auto *xml = resourceCache->GetResource <Urho3D::XMLFile> ("Resources/TestHexMap.xml");
    Urho3D::XMLElement root = xml->GetRoot ();

    grid_ = new DoubledCoordsHexGrid (root.GetAttribute ("type") == "DW" ?
            DoubledCoordsHexGrid::Type::DoubleWidth : DoubledCoordsHexGrid::Type::DoubleHeight,
            root.GetUInt ("rows"), root.GetUInt ("cols"), root.GetFloat ("radius")
    );

    Urho3D::XMLElement hex = root.GetChild ("hex");
    while (hex.NotNull ())
    {
        unsigned int row = hex.GetUInt ("row");
        unsigned int col = hex.GetUInt ("col");

        grid_->SetCell (row, col, hex.GetFloat ("cost"), new ReferenceCountedString (hex.GetAttribute ("prefab")));
        hex = hex.GetNext ("hex");
    }
}

void Urho3DHexagonalGrid::SetupGrid ()
{
    auto *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    const std::unordered_map <unsigned int, SharedPointer <ReferenceCounted>> &metadata = grid_->GetMetadata ();
    Urho3D::Quaternion rotation (0.0f, 90.0f, 0.0f);

    for (auto &cellMetadataPair : metadata)
    {
        std::pair <float, float> position = grid_->GetCellPosition (cellMetadataPair.first);
        Urho3D::Node *hexNode = scene_->InstantiateXML (
                resourceCache->GetResource <Urho3D::XMLFile> (
                        dynamic_cast <ReferenceCountedString *> (cellMetadataPair.second.Get ())->Content
                )->GetRoot (),
                {position.first, 0.0f, position.second}, rotation
        );
    }
}

Urho3D::Node *Urho3DHexagonalGrid::CreateBall () const
{
    auto *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::Node *ball = scene_->CreateChild ("Ball");

    auto *model = ball->CreateComponent <Urho3D::StaticModel> ();
    model->SetModel (resourceCache->GetResource <Urho3D::Model> ("Models/Sphere.mdl"));
    return ball;
}

void Urho3DHexagonalGrid::HandleMouseClick (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    int mouseButton = eventData [Urho3D::MouseButtonUp::P_BUTTON].GetInt ();
    auto *input = context_->GetSubsystem <Urho3D::Input> ();

    Urho3D::IntVector2 mousePosition = input->GetMousePosition ();
    Urho3D::RayQueryResult raycastResult = debugCamera_->RaycastSingle (mousePosition.x_, mousePosition.y_);
    unsigned int cell = grid_->WorldPositionToCell (raycastResult.position_.x_, raycastResult.position_.z_);
    ResetMovement ();

    if (mouseButton == Urho3D::MOUSEB_LEFT)
    {
        Teleport (cell, ball_);
    }
    else if (mouseButton == Urho3D::MOUSEB_RIGHT)
    {
        SetupMovement (cell);
    }
}

void Urho3DHexagonalGrid::HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (moving_)
    {
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        if (currentWayointIndex_ >= waypoints_.size ())
        {
            ResetMovement ();
            return;
        }

        std::pair <float, float> waypoint = grid_->GetCellPosition (waypoints_ [currentWayointIndex_]);
        Urho3D::Vector3 delta = Urho3D::Vector3 (waypoint.first, 1.0f, waypoint.second) - ball_->GetPosition ();

        if (delta.Length () > 0.05f)
        {
            ball_->Translate (delta * (MOVE_SPEED / delta.Length ()) * timeStep, Urho3D::TS_WORLD);
        }
        else
        {
            Teleport (waypoints_ [currentWayointIndex_], ball_);
            ++currentWayointIndex_;
        }
    }
}

void Urho3DHexagonalGrid::ResetMovement ()
{
    moving_ = false;
    for (auto &sphere : waypointsSpheres_)
    {
        sphere->Remove ();
    }

    waypointsSpheres_.clear ();
    waypoints_.clear ();
}

void Urho3DHexagonalGrid::Teleport (unsigned int cell, Urho3D::Node *node) const
{
    std::pair <float, float> cellPosition = grid_->GetCellPosition (cell);
    node->SetPosition ({cellPosition.first, 1.0f, cellPosition.second});
}

void Urho3DHexagonalGrid::SetupMovement (unsigned int cell)
{
    auto *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    float distance;

    unsigned int currentCell = grid_->WorldPositionToCell (ball_->GetPosition ().x_, ball_->GetPosition ().z_);
    Teleport (currentCell, ball_);

    if (GraphPathfinding::AStar (grid_, currentCell, cell, distance, waypoints_))
    {
        moving_ = true;
        currentWayointIndex_ = 1;

        for (auto &waypoint : waypoints_)
        {
            Urho3D::Node *ball = CreateBall ();
            ball->SetScale (0.5f);
            Teleport (waypoint, ball);
            waypointsSpheres_.push_back (ball);
        }
    }
}
