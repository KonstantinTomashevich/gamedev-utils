# Dependencies: UniversalException, SharedPointer.
add_library (QuestStateMachine QuestStateMachine.hpp QuestStateMachine.cpp QuestState.hpp QuestState.cpp)
target_link_libraries (QuestStateMachine SharedPointer)

add_executable (DialogueTest DialogueTest.cpp)
target_link_libraries (DialogueTest QuestStateMachine)

install (TARGETS QuestStateMachine EXPORT FindQuestStateMachine
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindQuestStateMachine DESTINATION targets)
install (FILES QuestState.hpp QuestStateMachine.hpp DESTINATION include/QuestStateMachine)
install (TARGETS DialogueTest RUNTIME DESTINATION bin CONFIGURATIONS ${CMAKE_BUILD_TYPE})
