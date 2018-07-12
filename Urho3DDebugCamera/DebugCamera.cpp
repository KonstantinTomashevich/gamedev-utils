#include "DebugCamera.hpp"
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>

DebugCamera::DebugCamera (Urho3D::Context *context) :
        Urho3D::Object (context),
        cameraNode_ (nullptr),
        moveSpeed_ (10.0f),
        rotationSpeed_ (90.0f),

        keyForward_ (Urho3D::KEY_W),
        keyBackward_ (Urho3D::KEY_S),
        keyRotateUp_ (Urho3D::KEY_Q),
        keyRotateDown_ (Urho3D::KEY_E),
        keyRotateLeft_ (Urho3D::KEY_A),
        keyRotateRight_ (Urho3D::KEY_D)
{

}

void DebugCamera::SetupCamera (Urho3D::Node *cameraNode)
{
    cameraNode_ = cameraNode;
    auto *camera = cameraNode_->CreateComponent <Urho3D::Camera> (Urho3D::LOCAL);
    auto *renderer = context_->GetSubsystem <Urho3D::Renderer> ();
    renderer->SetNumViewports (1);
    renderer->SetViewport (0, new Urho3D::Viewport (context_, cameraNode_->GetScene (), camera));

    UnsubscribeFromAllEvents ();
    SubscribeToEvent (cameraNode_->GetScene (), Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (DebugCamera, HandleSceneUpdate));
}

Urho3D::Node *DebugCamera::GetCameraNode () const
{
    return cameraNode_;
}

Urho3D::Node *DebugCamera::RaycastNode (int screenX, int screenY)
{
    if (cameraNode_ == nullptr)
    {
        return nullptr;
    }

    auto *graphics = context_->GetSubsystem <Urho3D::Graphics> ();
    auto *camera = cameraNode_->GetComponent <Urho3D::Camera> ();
    Urho3D::Ray ray = camera->GetScreenRay (screenX * 1.0f / graphics->GetWidth (),
            screenY * 1.0f / graphics->GetHeight ());

    Urho3D::PODVector <Urho3D::RayQueryResult> queryResult;
    Urho3D::RayOctreeQuery query (queryResult, ray);

    auto *octree = camera->GetScene ()->GetComponent <Urho3D::Octree> ();
    octree->RaycastSingle (query);

    if (queryResult.Empty ())
    {
        return nullptr;
    }

    return queryResult.At (0).node_;
}

float DebugCamera::GetMoveSpeed () const
{
    return moveSpeed_;
}

void DebugCamera::SetMoveSpeed (float moveSpeed)
{
    moveSpeed_ = moveSpeed;
}

float DebugCamera::GetRotationSpeed () const
{
    return rotationSpeed_;
}

void DebugCamera::SetRotationSpeed (float rotationSpeed)
{
    rotationSpeed_ = rotationSpeed;
}

KeyCode DebugCamera::GetKeyForward () const
{
    return keyForward_;
}

void DebugCamera::SetKeyForward (KeyCode keyForward)
{
    keyForward_ = keyForward;
}

KeyCode DebugCamera::GetKeyBackward () const
{
    return keyBackward_;
}

void DebugCamera::SetKeyBackward (KeyCode keyBackward)
{
    keyBackward_ = keyBackward;
}

KeyCode DebugCamera::GetKeyRotateUp () const
{
    return keyRotateUp_;
}

void DebugCamera::SetKeyRotateUp (KeyCode keyRotateUp)
{
    keyRotateUp_ = keyRotateUp;
}

KeyCode DebugCamera::GetKeyRotateDown () const
{
    return keyRotateDown_;
}

void DebugCamera::SetKeyRotateDown (KeyCode keyRotateDown)
{
    keyRotateDown_ = keyRotateDown;
}

KeyCode DebugCamera::GetKeyRotateLeft () const
{
    return keyRotateLeft_;
}

void DebugCamera::SetKeyRotateLeft (KeyCode keyRotateLeft)
{
    keyRotateLeft_ = keyRotateLeft;
}

KeyCode DebugCamera::GetKeyRotateRight () const
{
    return keyRotateRight_;
}

void DebugCamera::SetKeyRotateRight (KeyCode keyRotateRight)
{
    keyRotateRight_ = keyRotateRight;
}

void DebugCamera::HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (cameraNode_ == nullptr)
    {
        return;
    }

    float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
    auto *input = context_->GetSubsystem <Urho3D::Input> ();
    float deltaForward = 0.0f;
    float deltaPitch = 0.0f;
    float deltaYaw = 0.0f;

    if (input->GetKeyDown (keyForward_))
    { deltaForward += 1.0f; }
    if (input->GetKeyDown (keyBackward_))
    { deltaForward -= 1.0f; }

    if (input->GetKeyDown (keyRotateDown_))
    { deltaPitch += 1.0f; }
    if (input->GetKeyDown (keyRotateUp_))
    { deltaPitch -= 1.0f; }

    if (input->GetKeyDown (keyRotateRight_))
    { deltaYaw += 1.0f; }
    if (input->GetKeyDown (keyRotateLeft_))
    { deltaYaw -= 1.0f; }

    cameraNode_->Pitch (rotationSpeed_ * deltaPitch * timeStep, Urho3D::TS_LOCAL);
    cameraNode_->Yaw (rotationSpeed_ * deltaYaw * timeStep, Urho3D::TS_LOCAL);
    cameraNode_->Translate (Urho3D::Vector3 (0.0f, 0.0f, moveSpeed_ * deltaForward * timeStep), Urho3D::TS_LOCAL);
}
