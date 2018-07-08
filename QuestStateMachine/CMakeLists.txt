# Dependencies: UniversalException, SharedPointer.
add_library (QuestStateMachine QuestStateMachine.hpp QuestStateMachine.cpp QuestState.hpp QuestState.cpp)
target_link_libraries (QuestStateMachine SharedPointer)

add_executable (DialogueTest DialogueTest.cpp)
target_link_libraries (DialogueTest QuestStateMachine)
