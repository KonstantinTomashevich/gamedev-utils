#pragma once
#include "DebugCameraBase.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

typedef int KeyCode;
class ThirdPersonDebugCamera : public DebugCameraBase
{
URHO3D_OBJECT (ThirdPersonDebugCamera, DebugCameraBase)
public:
    explicit ThirdPersonDebugCamera (Urho3D::Context *context);
    virtual ~ThirdPersonDebugCamera () = default;

    float GetMoveSpeed () const;
    void SetMoveSpeed (float moveSpeed);

    float GetRotationSpeed () const;
    void SetRotationSpeed (float rotationSpeed);

    KeyCode GetKeyForward () const;
    void SetKeyForward (KeyCode keyForward);

    KeyCode GetKeyBackward () const;
    void SetKeyBackward (KeyCode keyBackward);

    KeyCode GetKeyUp () const;
    void SetKeyUp (KeyCode keyUp);

    KeyCode GetKeyDown () const;
    void SetKeyDown (KeyCode keyDown);

    KeyCode GetKeyLeft () const;
    void SetKeyLeft (KeyCode keyLeft);

    KeyCode GetKeyRight () const;
    void SetKeyRight (KeyCode keyRight);

    KeyCode GetKeyRotateLeft () const;
    void SetKeyRotateLeft (KeyCode keyRotateLeft);

    KeyCode GetKeyRotateRight () const;
    void SetKeyRotateRight (KeyCode keyRotateRight);

protected:
    virtual void HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

private:
    float moveSpeed_;
    float rotationSpeed_;

    KeyCode keyForward_;
    KeyCode keyBackward_;
    KeyCode keyUp_;
    KeyCode keyDown_;

    KeyCode keyLeft_;
    KeyCode keyRight_;
    KeyCode keyRotateLeft_;
    KeyCode keyRotateRight_;
};

