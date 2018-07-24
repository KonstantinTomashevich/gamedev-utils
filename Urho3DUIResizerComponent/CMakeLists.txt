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

# Copy Urho3D Data for editor.
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    message (STATUS "Copying Urho3D's Data asset for editor with UIResizer component...")
    if (NOT EXISTS "$ENV{URHO3D_HOME}/share/Resources/Data/")
        file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/Resources/Data/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    else ()
        file (COPY "$ENV{URHO3D_HOME}/share/Resources/Data/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    endif ()
endif ()

set (TARGET_NAME Urho3DEditorWithUIResizerLauncher)
set (RESOURCE_DIRS "${CMAKE_SOURCE_DIR}/bin/CoreData;${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
set (SOURCE_FILES EditorLauncher.cpp)

setup_main_executable ()
target_link_libraries (Urho3DEditorWithUIResizerLauncher Urho3DUIResizerComponent)

set_target_properties (
        Urho3DEditorWithUIResizerLauncher
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)
