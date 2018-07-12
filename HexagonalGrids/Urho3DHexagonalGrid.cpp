#include "Urho3DHexagonalGrid.hpp"
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Input/Input.h>

#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Octree.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>

URHO3D_DEFINE_APPLICATION_MAIN (Urho3DHexagonalGrid)
class ReferenceCountedString : public ReferenceCounted
{
public:
    ReferenceCountedString (const Urho3D::String &content) : ReferenceCounted (), Content (content) {}
    virtual ~ReferenceCountedString () = default;

    Urho3D::String Content;
};

Urho3DHexagonalGrid::Urho3DHexagonalGrid (Urho3D::Context *context) :
        Urho3D::Application (context)
{

}

void Urho3DHexagonalGrid::Setup ()
{
    Application::Setup ();
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
    engineParameters_[Urho3D::EP_LOG_NAME] = "Urho3DHexagonalGrid.log";
    engineParameters_[Urho3D::EP_WINDOW_TITLE] = "Urho3D Hexagonal Grid";
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
}

void Urho3DHexagonalGrid::Stop ()
{
    Application::Stop ();
    scene_->Clear ();
    delete scene_;
    delete debugCamera_;
}

void Urho3DHexagonalGrid::SetupLight ()
{
    scene_->CreateComponent <Urho3D::Octree> ();
    Urho3D::Node *lightNode = scene_->CreateChild ("Light");
    lightNode->Pitch (60.0f);

    auto *light = lightNode->CreateComponent <Urho3D::Light> ();
    light->SetLightType (Urho3D::LIGHT_DIRECTIONAL);
}

void Urho3DHexagonalGrid::SetupCamera ()
{
    debugCamera_ = new DebugCamera (context_);
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
