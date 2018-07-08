include (${CMAKE_CURRENT_SOURCE_DIR}/CMake/TotalList.cmake)

project (SimpleCardGame)
cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
enable_testing ()

option (IncludeUniversalException "Include UniversalException util." ON)
option (IncludeSharedPointer "Include SharedPointer util." ON)
cmake_dependent_option (IncludeQuestStateMachine
        "Include QuestStateMachine util." ON "IncludeUniversalException;IncludeSharedPointer" OFF)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
if (IncludeQuestStateMachine)
    add_subdirectory (QuestStateMachine)
endif ()

if (IncludeSharedPointer)
    add_subdirectory (SharedPointer)
endif ()
