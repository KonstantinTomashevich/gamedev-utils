# Dependencies: UniversalException, GraphPathfinding, SharedPointer.
add_library (HexagonalGrids DoubledCoordsHexGrid.cpp DoubledCoordsHexGrid.hpp)
target_link_libraries (HexagonalGrids GraphPathfinding SharedPointer)

# Sample, additional dependencies: Urho3D, DebugCamera.
if (EnableUrho3DUtils)
    set (TARGET_NAME Urho3DHexagonalGrid)
    set (SOURCE_FILES Urho3DHexagonalGrid.hpp Urho3DHexagonalGrid.cpp)
    setup_main_executable ()
    target_link_libraries (Urho3DHexagonalGrid HexagonalGrids Urho3DDebugCamera)
endif ()
