#include "FirstPersonDebugCamera.hpp"
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

FirstPersonDebugCamera::FirstPersonDebugCamera (Urho3D::Context *context) :
        DebugCameraBase (context),
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

float FirstPersonDebugCamera::GetMoveSpeed () const
{
    return moveSpeed_;
}

void FirstPersonDebugCamera::SetMoveSpeed (float moveSpeed)
{
    moveSpeed_ = moveSpeed;
}

float FirstPersonDebugCamera::GetRotationSpeed () const
{
    return rotationSpeed_;
}

void FirstPersonDebugCamera::SetRotationSpeed (float rotationSpeed)
{
    rotationSpeed_ = rotationSpeed;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyForward () const
{
    return keyForward_;
}

void FirstPersonDebugCamera::SetKeyForward (Urho3D::Key keyForward)
{
    keyForward_ = keyForward;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyBackward () const
{
    return keyBackward_;
}

void FirstPersonDebugCamera::SetKeyBackward (Urho3D::Key keyBackward)
{
    keyBackward_ = keyBackward;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyRotateUp () const
{
    return keyRotateUp_;
}

void FirstPersonDebugCamera::SetKeyRotateUp (Urho3D::Key keyRotateUp)
{
    keyRotateUp_ = keyRotateUp;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyRotateDown () const
{
    return keyRotateDown_;
}

void FirstPersonDebugCamera::SetKeyRotateDown (Urho3D::Key keyRotateDown)
{
    keyRotateDown_ = keyRotateDown;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyRotateLeft () const
{
    return keyRotateLeft_;
}

void FirstPersonDebugCamera::SetKeyRotateLeft (Urho3D::Key keyRotateLeft)
{
    keyRotateLeft_ = keyRotateLeft;
}

Urho3D::Key FirstPersonDebugCamera::GetKeyRotateRight () const
{
    return keyRotateRight_;
}

void FirstPersonDebugCamera::SetKeyRotateRight (Urho3D::Key keyRotateRight)
{
    keyRotateRight_ = keyRotateRight;
}

void FirstPersonDebugCamera::HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Node *cameraNode = GetCameraNode ();
    if (!IsEnabled () || cameraNode == nullptr)
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

    cameraNode->Pitch (rotationSpeed_ * deltaPitch * timeStep, Urho3D::TS_WORLD);
    cameraNode->Yaw (rotationSpeed_ * deltaYaw * timeStep, Urho3D::TS_WORLD);
    cameraNode->Translate (Urho3D::Vector3 (0.0f, 0.0f, moveSpeed_ * deltaForward * timeStep), Urho3D::TS_LOCAL);
}
