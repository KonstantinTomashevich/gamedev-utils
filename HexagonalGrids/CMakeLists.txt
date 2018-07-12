# Dependencies: UniversalException, GraphPathfinding, SharedPointer.
add_library (HexagonalGrids DoubledCoordsHexGrid.cpp DoubledCoordsHexGrid.hpp)
target_link_libraries (HexagonalGrids GraphPathfinding)
