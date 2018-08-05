#pragma once
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Graphics/Octree.h>

class DebugCameraBase : public Urho3D::Object
{
URHO3D_OBJECT (DebugCameraBase, Object)
public:
    explicit DebugCameraBase (Urho3D::Context *context);
    virtual ~DebugCameraBase () = default;

    void SetupCamera (Urho3D::Node *cameraNode);
    Urho3D::Node *GetCameraNode () const;
    bool RaycastSingle (int screenX, int screenY, Urho3D::RayQueryResult &output,
            unsigned char drawableFlags = Urho3D::DRAWABLE_GEOMETRY) const;
    Urho3D::Node *RaycastNode (int screenX, int screenY, unsigned char drawableFlags = Urho3D::DRAWABLE_GEOMETRY) const;

    bool IsEnabled () const;
    void SetEnabled (bool enabled);

    // Exception classes.
    class CameraNotFound;

protected:
    virtual void HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) = 0;

private:
    Urho3D::Node *cameraNode_;
    bool enabled_;
};
