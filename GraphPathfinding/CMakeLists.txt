add_library (GraphPathfinding GraphAdapter.hpp GraphPathfinding.cpp GraphPathfinding.hpp)
add_executable (TestGraphPathfinding TestGraphPathfinding.cpp)
target_link_libraries (TestGraphPathfinding GraphPathfinding)

install (TARGETS GraphPathfinding EXPORT FindGraphPathfinding
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindGraphPathfinding DESTINATION targets)
install (FILES GraphAdapter.hpp GraphPathfinding.hpp DESTINATION include/GraphPathfinding)
