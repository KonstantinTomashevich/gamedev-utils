# Dependencies: UniversalException.
add_library (QuestStateMachine QuestStateMachine.hpp QuestStateMachine.cpp QuestState.hpp QuestState.cpp)
add_executable (DialogueTest DialogueTest.cpp)
target_link_libraries (DialogueTest QuestStateMachine)
