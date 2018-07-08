add_library (SharedPointer SharedPointer.hpp ReferenceCounted.hpp ReferenceCounted.cpp)
add_executable (TestSharedPointer Test.cpp)
target_link_libraries (TestSharedPointer SharedPointer)
add_test (NAME TestSharedPointer COMMAND TestSharedPointer)
