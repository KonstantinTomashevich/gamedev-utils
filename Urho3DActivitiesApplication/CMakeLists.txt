# Dependencies: UniversalException.
set (TARGET_NAME Urho3DActivitiesApplication)
define_source_files (RECURSE GLOB_H_PATTERNS "*.hpp")
define_dependency_libs (Urho3D)
setup_library (STATIC)

install (TARGETS Urho3DActivitiesApplication EXPORT FindUrho3DActivitiesApplication
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindUrho3DActivitiesApplication DESTINATION targets)
install (FILES Activity.hpp ActivitiesApplication.hpp ModularActivity.hpp DESTINATION include/Urho3DActivitiesApplication)
