add_library (SharedPointer SharedPointer.hpp ReferenceCounted.hpp ReferenceCounted.cpp)
add_executable (TestSharedPointer Test.cpp)
target_link_libraries (TestSharedPointer SharedPointer)
add_test (NAME TestSharedPointer COMMAND TestSharedPointer)

install (TARGETS SharedPointer EXPORT FindSharedPointer
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindSharedPointer DESTINATION targets)
install (FILES SharedPointer.hpp ReferenceCounted.hpp DESTINATION include/SharedPointer)
install (TARGETS TestSharedPointer RUNTIME DESTINATION bin CONFIGURATIONS ${CMAKE_BUILD_TYPE})
