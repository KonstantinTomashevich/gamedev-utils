#pragma once
#include "DebugCameraBase.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Input/Input.h>

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

    Urho3D::Key GetKeyForward () const;
    void SetKeyForward (Urho3D::Key keyForward);

    Urho3D::Key GetKeyBackward () const;
    void SetKeyBackward (Urho3D::Key keyBackward);

    Urho3D::Key GetKeyUp () const;
    void SetKeyUp (Urho3D::Key keyUp);

    Urho3D::Key GetKeyDown () const;
    void SetKeyDown (Urho3D::Key keyDown);

    Urho3D::Key GetKeyLeft () const;
    void SetKeyLeft (Urho3D::Key keyLeft);

    Urho3D::Key GetKeyRight () const;
    void SetKeyRight (Urho3D::Key keyRight);

    Urho3D::Key GetKeyRotateLeft () const;
    void SetKeyRotateLeft (Urho3D::Key keyRotateLeft);

    Urho3D::Key GetKeyRotateRight () const;
    void SetKeyRotateRight (Urho3D::Key keyRotateRight);

protected:
    virtual void HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

private:
    float moveSpeed_;
    float rotationSpeed_;

    Urho3D::Key keyForward_;
    Urho3D::Key keyBackward_;
    Urho3D::Key keyUp_;
    Urho3D::Key keyDown_;

    Urho3D::Key keyLeft_;
    Urho3D::Key keyRight_;
    Urho3D::Key keyRotateLeft_;
    Urho3D::Key keyRotateRight_;
};

