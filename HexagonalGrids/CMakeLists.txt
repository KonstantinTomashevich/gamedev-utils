# Dependencies: UniversalException, GraphPathfinding, SharedPointer.
add_library (HexagonalGrids DoubledCoordsHexGrid.cpp DoubledCoordsHexGrid.hpp)
target_link_libraries (HexagonalGrids GraphPathfinding SharedPointer)

install (TARGETS HexagonalGrids EXPORT FindHexagonalGrids
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindHexagonalGrids DESTINATION targets)
install (FILES DoubledCoordsHexGrid.hpp DESTINATION include/HexagonalGrids)

# Sample, additional dependencies: Urho3D, FirstPersonDebugCamera.
if (EnableUrho3DUtils)
    set (TARGET_NAME Urho3DHexagonalGrid)
    set (SOURCE_FILES Urho3DHexagonalGrid.hpp Urho3DHexagonalGrid.cpp)
    setup_main_executable ()
    target_link_libraries (Urho3DHexagonalGrid HexagonalGrids Urho3DDebugCamera)
endif ()
