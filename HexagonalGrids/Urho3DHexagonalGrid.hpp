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
    void SetupLight ();
    void SetupCamera ();
    void LoadGrid ();
    void SetupGrid ();

    Urho3D::Scene *scene_;
    ThirdPersonDebugCamera *debugCamera_;
    DoubledCoordsHexGrid *grid_;
};
