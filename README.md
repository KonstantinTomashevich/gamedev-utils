# Gamedev Utils (by Konstantin Tomashevich)
My personal archive of useful small C++ libraries for gamedev projects. Supports SDK and CMake target export throught `install` command.

## GraphPathfinding
No dependencies.

`Dijkstra` and `A*` pathfinding algorithms. Input graph must implement `GraphAdapter` interface.

## HexagonalGrids
Dependencies: GraphPathfinding, SharedPointer, UniversalException, Urho3D (only for example), Urho3DDebugCamera (only for example).

Implementation of doubled coords hexagonal map (double-row-coord and double-col-coord). Supports pathfinding and custom cell metadata. In addition, there is simple Urho3D example app.
<img src="https://preview.ibb.co/idcUDA/image.png" alt="image" border="0"/>

WASD - move camera, QE -- rotate camera, LMB -- teleport ball, RMB -- try navigate to cell.

## QuestStateMachine
Dependencies: SharedPointer, UniversalException.

Simple state machine for dialogs and quests. See simple console calculator example which emulates dialog on this machine.

## SharedPointer
No dependencies.

Simple shared pointer for `ReferenceCounted` derived classes.

## UniversalException
No dependencies.

Simple exception class that works both on Visual C++ and GCC. Also it supports "custom" exceptions (see QuestStateMachine for example).

## Urho3DActivitiesApplication
Dependencies: Urho3D, UniversalException.

ActivitesApplication is useful for splitting game to 2 parallel activities like client and server in the same process. Also ModularActivity is something like local (for this activity) version of `Urho3D::Context` which is useful in some cases.

## Urho3DDebugCamera
Dependencies: Urho3D, UniversalException.

Two simple camera handlers useful for prototyping. See Urho3DHexagonalGrids (HexagonalGrids example) as example.

## Urho3DUIResizer
Dependencies: Urho3D.

Urho3D component for `Urho3D::UIElement`s resizing and layouting. Used `Urho3D::UIElement` vars:

### For position calculation:
*VX* : float, X-coord modifier.

*VY* : float, X-coord modifier.

*XDep* : String, dependency for X-coord, value: "SH" (Screen Height), "SW" (Screen Width), "PW" (Parent Width), "PH" (ParentHeight).

*YDep* : String, dependency for Y-coord, value: "SH" (Screen Height), "SW" (Screen Width), "PW" (Parent Width), "PH" (ParentHeight).

*Result X* = VX * value (XDep)

*Result Y* = VY * value (YDep)

### For size calculation:
*VWidth* : float, width modifier.

*VHeight* : float, height modifier.

*WDep* : String, dependency for width, value: "SH" (Screen Height), "SW" (Screen Width), "PW" (Parent Width), "PH" (ParentHeight).

*HDep* : String, dependency for height, value: "SH" (Screen Height), "SW" (Screen Width), "PW" (Parent Width), "PH" (ParentHeight).

*Result Width* = VX * value (XDep)

*Result Height* = VY * value (YDep)

### For layouting:
*LayoutType* : String, "H" (Horizontal) or "V" (Vertical) or nothing (no layouting).

*UpdatePS* : bool, if true, primary size will be calculated from children size.

*UpdateSS* : bool, if true, secondary size will be calculated from children size.

Also `Layout Border` and `Layout Spacing` of `Urho3D::UIElement` are used too (result is 0.001 * value (dep) where dep is HDep or WDep depending on layout type).

### For texts (Urho3D::Text and Urho3D::LineEdit only):
*VTextSize* : float, text size modifier.

*TSDep* : String, dependency for test size, value: "SH" (Screen Height), "SW" (Screen Width), "PW" (Parent Width), "PH" (ParentHeight).

*Result Text Size* = VTextSize * value (TSDep)
