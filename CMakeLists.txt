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
        "Include Urho3DDebugCamera util." ON "EnableUrho3DUtils" OFF)

if (EnableUrho3DUtils)
    # Copy Urho3D CMake modules.
    if (NOT CMAKE_MODULE_PATH)
        message (STATUS "CMAKE_MODULE_PATH isn't setted! Setting CMAKE_MODULE_PATH to urho3d-cmake/Modules.")
        set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/urho3d-cmake/Modules")
        if (NOT EXISTS "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
            message (STATUS "Copying Urho3D's CMake modules...")
            if (NOT EXISTS "$ENV{URHO3D_HOME}/share/CMake/")
                file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
            else ()
                file (COPY "$ENV{URHO3D_HOME}/share/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake")
            endif ()
        endif ()
        message (STATUS "Setted CMAKE_MODULE_PATH to urho3d-cmake/Modules!")
    endif ()

    # Copy Urho3D CoreData.
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/CoreData")
        message (STATUS "Copying Urho3D's CoreData asset...")
        if (NOT EXISTS "$ENV{URHO3D_HOME}/share/Resources/CoreData/")
            file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
        else ()
            file (COPY "$ENV{URHO3D_HOME}/share/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
        endif ()
    endif ()

    # Include Urho3D package.
    include (UrhoCommon)
    find_package (Urho3D REQUIRED)
    include_directories (${URHO3D_INCLUDE_DIRS})
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
