#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

typedef int KeyCode;
typedef int MouseButtonCode;

class DebugCamera : public Urho3D::Object
{
URHO3D_OBJECT (DebugCamera, Object)
public:
    DebugCamera (Urho3D::Context *context);
    virtual ~DebugCamera () = default;

    void SetupCamera (Urho3D::Node *cameraNode);
    Urho3D::Node *GetCameraNode () const;
    Urho3D::Node *RaycastNode (int screenX, int screenY);

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

private:
    void HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    Urho3D::Node *cameraNode_;
    float moveSpeed_;
    float rotationSpeed_;

    KeyCode keyForward_;
    KeyCode keyBackward_;
    KeyCode keyRotateUp_;
    KeyCode keyRotateDown_;
    KeyCode keyRotateLeft_;
    KeyCode keyRotateRight_;
};

