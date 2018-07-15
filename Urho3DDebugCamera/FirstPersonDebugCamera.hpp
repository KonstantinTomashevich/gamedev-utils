#pragma once
#include "DebugCameraBase.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Input/Input.h>

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

    Urho3D::Key GetKeyForward () const;
    void SetKeyForward (Urho3D::Key keyForward);

    Urho3D::Key GetKeyBackward () const;
    void SetKeyBackward (Urho3D::Key keyBackward);

    Urho3D::Key GetKeyRotateUp () const;
    void SetKeyRotateUp (Urho3D::Key keyRotateUp);

    Urho3D::Key GetKeyRotateDown () const;
    void SetKeyRotateDown (Urho3D::Key keyRotateDown);

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
    Urho3D::Key keyRotateUp_;
    Urho3D::Key keyRotateDown_;
    Urho3D::Key keyRotateLeft_;
    Urho3D::Key keyRotateRight_;
};

