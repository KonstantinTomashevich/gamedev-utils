#include "DebugCameraBase.hpp"
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>

#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <UniversalException/UniversalException.hpp>

DebugCameraBase::DebugCameraBase (Urho3D::Context *context) :
        Urho3D::Object (context),
        cameraNode_ (nullptr)
{

}

void DebugCameraBase::SetupCamera (Urho3D::Node *cameraNode)
{
    cameraNode_ = cameraNode;
    auto *camera = cameraNode_->CreateComponent <Urho3D::Camera> (Urho3D::LOCAL);
    auto *renderer = context_->GetSubsystem <Urho3D::Renderer> ();
    renderer->SetNumViewports (1);
    renderer->SetViewport (0, new Urho3D::Viewport (context_, cameraNode_->GetScene (), camera));

    UnsubscribeFromAllEvents ();
    SubscribeToEvent (cameraNode_->GetScene (), Urho3D::E_SCENEUPDATE,
            URHO3D_HANDLER (DebugCameraBase, HandleSceneUpdate));
}

Urho3D::Node *DebugCameraBase::GetCameraNode () const
{
    return cameraNode_;
}

Urho3D::RayQueryResult DebugCameraBase::RaycastSingle (int screenX, int screenY) const
{
    if (cameraNode_ == nullptr)
    {
        throw UniversalException <DebugCameraBase::CameraNotFound> (
                "DebugCameraBase: camera node is null, can not raycast!");
    }

    auto *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    auto *camera = cameraNode_->GetComponent <Urho3D::Camera> ();
    Urho3D::Ray ray = camera->GetScreenRay (screenX * 1.0f / graphics->GetWidth (),
            screenY * 1.0f / graphics->GetHeight ());

    Urho3D::PODVector <Urho3D::RayQueryResult> queryResult;
    Urho3D::RayOctreeQuery query (queryResult, ray);

    auto *octree = camera->GetScene ()->GetComponent <Urho3D::Octree> ();
    octree->RaycastSingle (query);
    return queryResult[0];
}

Urho3D::Node *DebugCameraBase::RaycastNode (int screenX, int screenY) const
{
    return RaycastSingle (screenX, screenY).node_;
}
