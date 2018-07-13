include (${CMAKE_CURRENT_SOURCE_DIR}/CMake/TotalList.cmake)

project (GamedevUtils)
cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
enable_testing ()

option (EnableUrho3DUtils "Use Urho3D binaries from URHO3D_HOME environment variable." ON)
option (IncludeUniversalException "Include UniversalException util." ON)
option (IncludeSharedPointer "Include SharedPointer util." ON)

cmake_dependent_option (IncludeQuestStateMachine
        "Include QuestStateMachine util." ON "IncludeUniversalException;IncludeSharedPointer" OFF)

cmake_dependent_option (IncludeUrho3DUIResizerComponent
        "Include Urho3DUIResizerComponent util." ON "EnableUrho3DUtils" OFF)

cmake_dependent_option (IncludeUrho3DActivitiesApplication
        "Include Urho3DActivitiesApplication util." ON "EnableUrho3DUtils" OFF)
option (IncludeGraphPathfinding "Include GraphPathfinding util." ON)

cmake_dependent_option (IncludeHexagonalGrids
        "Include HexagonalGrids util." ON "IncludeGraphPathfinding;IncludeSharedPointer;IncludeUniversalException" OFF)

cmake_dependent_option (IncludeUrho3DDebugCamera
        "Include Urho3DDebugCamera util." ON "EnableUrho3DUtils;IncludeUniversalException" OFF)

if (EnableUrho3DUtils)
    include (${CMAKE_CURRENT_SOURCE_DIR}/CMake/IncludeUrho3D.cmake)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
if (IncludeQuestStateMachine)
    add_subdirectory (QuestStateMachine)
endif ()

if (IncludeSharedPointer)
    add_subdirectory (SharedPointer)
endif ()

if (IncludeUrho3DUIResizerComponent)
    add_subdirectory (Urho3DUIResizerComponent)
endif ()

if (IncludeUrho3DActivitiesApplication)
    add_subdirectory (Urho3DActivitiesApplication)
endif ()

if (IncludeGraphPathfinding)
    add_subdirectory (GraphPathfinding)
endif ()

if (IncludeHexagonalGrids)
    add_subdirectory (HexagonalGrids)
endif ()

if (IncludeUrho3DDebugCamera)
    add_subdirectory (Urho3DDebugCamera)
endif ()

if (EnableUrho3DUtils)
    install (DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/CoreData" DESTINATION bin)
    install (DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/Data" DESTINATION bin)
endif ()

install (FILES CMake/CMakeDependentOption.cmake CMake/IncludeUrho3D.cmake DESTINATION cmake-utils)
