#include "ThirdPersonDebugCamera.hpp"
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>

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

KeyCode ThirdPersonDebugCamera::GetKeyForward () const
{
    return keyForward_;
}

void ThirdPersonDebugCamera::SetKeyForward (KeyCode keyForward)
{
    keyForward_ = keyForward;
}

KeyCode ThirdPersonDebugCamera::GetKeyBackward () const
{
    return keyBackward_;
}

void ThirdPersonDebugCamera::SetKeyBackward (KeyCode keyBackward)
{
    keyBackward_ = keyBackward;
}

KeyCode ThirdPersonDebugCamera::GetKeyUp () const
{
    return keyUp_;
}

void ThirdPersonDebugCamera::SetKeyUp (KeyCode keyUp)
{
    keyUp_ = keyUp;
}

KeyCode ThirdPersonDebugCamera::GetKeyDown () const
{
    return keyDown_;
}

void ThirdPersonDebugCamera::SetKeyDown (KeyCode keyDown)
{
    keyDown_ = keyDown;
}

KeyCode ThirdPersonDebugCamera::GetKeyLeft () const
{
    return keyLeft_;
}

void ThirdPersonDebugCamera::SetKeyLeft (KeyCode keyLeft)
{
    keyLeft_ = keyLeft;
}

KeyCode ThirdPersonDebugCamera::GetKeyRight () const
{
    return keyRight_;
}

void ThirdPersonDebugCamera::SetKeyRight (KeyCode keyRight)
{
    keyRight_ = keyRight;
}

KeyCode ThirdPersonDebugCamera::GetKeyRotateLeft () const
{
    return keyRotateLeft_;
}

void ThirdPersonDebugCamera::SetKeyRotateLeft (KeyCode keyRotateLeft)
{
    keyRotateLeft_ = keyRotateLeft;
}

KeyCode ThirdPersonDebugCamera::GetKeyRotateRight () const
{
    return keyRotateRight_;
}

void ThirdPersonDebugCamera::SetKeyRotateRight (KeyCode keyRotateRight)
{
    keyRotateRight_ = keyRotateRight;
}

void ThirdPersonDebugCamera::HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Node *cameraNode = GetCameraNode ();
    if (cameraNode == nullptr)
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
