#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3DDebugCamera/ThirdPersonDebugCamera.hpp>
#include <SharedPointer/ReferenceCounted.hpp>
#include <HexagonalGrids/DoubledCoordsHexGrid.hpp>

class Urho3DHexagonalGrid : public Urho3D::Application
{
    URHO3D_OBJECT (Urho3DHexagonalGrid, Application)
public:
    Urho3DHexagonalGrid (Urho3D::Context *context);
    virtual ~Urho3DHexagonalGrid () = default;

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();

private:
    const float MOVE_SPEED = 1.0f;

    void SetupLight ();
    void SetupCamera ();
    void LoadGrid ();

    void SetupGrid ();
    Urho3D::Node *CreateBall () const;

    void HandleMouseClick (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleSceneUpdate (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void ResetMovement ();
    void Teleport (unsigned int cell, Urho3D::Node *node) const;
    void SetupMovement (unsigned int cell);

    Urho3D::Scene *scene_;
    Urho3D::Node *ball_;
    ThirdPersonDebugCamera *debugCamera_;
    DoubledCoordsHexGrid *grid_;

    bool moving_;
    unsigned int currentWayointIndex_;
    std::vector <int> waypoints_;
    std::vector <Urho3D::Node *> waypointsSpheres_;
};
