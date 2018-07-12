#pragma once
#include "DebugCameraBase.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

typedef int KeyCode;
class FirstPersonDebugCamera : public DebugCameraBase
{
URHO3D_OBJECT (FirstPersonDebugCamera, DebugCameraBase)
public:
    explicit FirstPersonDebugCamera (Urho3D::Context *context);
    virtual ~FirstPersonDebugCamera () = default;

    float GetMoveSpeed () const;
    void SetMoveSpeed (float moveSpeed);

    float GetRotationSpeed () const;
    void SetRotationSpeed (float rotationSpeed);

    KeyCode GetKeyForward () const;
    void SetKeyForward (KeyCode keyForward);

    KeyCode GetKeyBackward () const;
    void SetKeyBackward (KeyCode keyBackward);

    KeyCode GetKeyRotateUp () const;
    void SetKeyRotateUp (KeyCode keyRotateUp);

    KeyCode GetKeyRotateDown () const;
    void SetKeyRotateDown (KeyCode keyRotateDown);

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
    KeyCode keyRotateUp_;
    KeyCode keyRotateDown_;
    KeyCode keyRotateLeft_;
    KeyCode keyRotateRight_;
};

