set (TARGET_NAME Urho3DUIResizerComponent)
set (SOURCE_FILES UIResizer.hpp UIResizer.cpp)
define_dependency_libs (Urho3D)
setup_library (STATIC)

install (TARGETS Urho3DUIResizerComponent EXPORT FindUrho3DUIResizerComponent
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindUrho3DUIResizerComponent DESTINATION targets)
install (FILES UIResizer.hpp DESTINATION include/Urho3DUIResizerComponent)
