project (SimpleCardGame)
cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
enable_testing ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory (QuestStateMachine)
add_subdirectory (SharedPointer)
