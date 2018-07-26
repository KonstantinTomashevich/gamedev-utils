#include "ThirdPersonDebugCamera.hpp"
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

ThirdPersonDebugCamera::ThirdPersonDebugCamera (Urho3D::Context *context) :
        DebugCameraBase (context),
        moveSpeed_ (10.0f),
        rotationSpeed_ (90.0f),

        keyForward_ (Urho3D::KEY_W),
        keyBackward_ (Urho3D::KEY_S),
        keyUp_ (Urho3D::KEY_R),
        keyDown_ (Urho3D::KEY_F),

        keyLeft_ (Urho3D::KEY_A),
        keyRight_ (Urho3D::KEY_D),
        keyRotateLeft_ (Urho3D::KEY_Q),
        keyRotateRight_ (Urho3D::KEY_E)
{

}

float ThirdPersonDebugCamera::GetMoveSpeed () const
{
    return moveSpeed_;
}

void ThirdPersonDebugCamera::SetMoveSpeed (float moveSpeed)
{
    moveSpeed_ = moveSpeed;
}

float ThirdPersonDebugCamera::GetRotationSpeed () const
{
    return rotationSpeed_;
}

void ThirdPersonDebugCamera::SetRotationSpeed (float rotationSpeed)
{
    rotationSpeed_ = rotationSpeed;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyForward () const
{
    return keyForward_;
}

void ThirdPersonDebugCamera::SetKeyForward (Urho3D::Key keyForward)
{
    keyForward_ = keyForward;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyBackward () const
{
    return keyBackward_;
}

void ThirdPersonDebugCamera::SetKeyBackward (Urho3D::Key keyBackward)
{
    keyBackward_ = keyBackward;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyUp () const
{
    return keyUp_;
}

void ThirdPersonDebugCamera::SetKeyUp (Urho3D::Key keyUp)
{
    keyUp_ = keyUp;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyDown () const
{
    return keyDown_;
}

void ThirdPersonDebugCamera::SetKeyDown (Urho3D::Key keyDown)
{
    keyDown_ = keyDown;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyLeft () const
{
    return keyLeft_;
}

void ThirdPersonDebugCamera::SetKeyLeft (Urho3D::Key keyLeft)
{
    keyLeft_ = keyLeft;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyRight () const
{
    return keyRight_;
}

void ThirdPersonDebugCamera::SetKeyRight (Urho3D::Key keyRight)
{
    keyRight_ = keyRight;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyRotateLeft () const
{
    return keyRotateLeft_;
}

void ThirdPersonDebugCamera::SetKeyRotateLeft (Urho3D::Key keyRotateLeft)
{
    keyRotateLeft_ = keyRotateLeft;
}

Urho3D::Key ThirdPersonDebugCamera::GetKeyRotateRight () const
{
    return keyRotateRight_;
}

void ThirdPersonDebugCamera::SetKeyRotateRight (Urho3D::Key keyRotateRight)
{
    keyRotateRight_ = keyRotateRight;
}

void ThirdPersonDebugCamera::HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Node *cameraNode = GetCameraNode ();
    if (!IsEnabled () || cameraNode == nullptr)
    {
        return;
    }

    float timeStep = eventData[Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
    auto *input = context_->GetSubsystem <Urho3D::Input> ();
    float deltaX = 0.0f;
    float deltaY = 0.0f;
    float deltaZ = 0.0f;
    float deltaYaw = 0.0f;

    if (input->GetKeyDown (keyRight_))
    { deltaX += 1.0f; }
    if (input->GetKeyDown (keyLeft_))
    { deltaX -= 1.0f; }

    if (input->GetKeyDown (keyUp_))
    { deltaY += 1.0f; }
    if (input->GetKeyDown (keyDown_))
    { deltaY -= 1.0f; }

    if (input->GetKeyDown (keyForward_))
    { deltaZ += 1.0f; }
    if (input->GetKeyDown (keyBackward_))
    { deltaZ -= 1.0f; }

    if (input->GetKeyDown (keyRotateRight_))
    { deltaYaw += 1.0f; }
    if (input->GetKeyDown (keyRotateLeft_))
    { deltaYaw -= 1.0f; }

    cameraNode->Yaw (rotationSpeed_ * deltaYaw * timeStep, Urho3D::TS_WORLD);
    float cameraPitch = cameraNode->GetRotation ().PitchAngle ();
    cameraNode->Pitch (-cameraPitch);

    cameraNode->Translate (Urho3D::Vector3 (moveSpeed_ * deltaX * timeStep, moveSpeed_ * deltaY * timeStep,
            moveSpeed_ * deltaZ * timeStep), Urho3D::TS_LOCAL);
    cameraNode->Pitch (cameraPitch);
}
