# Dependencies: UniversalException.
set (TARGET_NAME Urho3DDebugCamera)
define_source_files (RECURSE GLOB_H_PATTERNS "*.hpp")
define_dependency_libs (Urho3D)
setup_library (STATIC)

install (TARGETS Urho3DDebugCamera EXPORT FindUrho3DDebugCamera
        LIBRARY DESTINATION lib CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        ARCHIVE DESTINATION lib/static CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        )
install (EXPORT FindUrho3DDebugCamera DESTINATION targets)
install (FILES DebugCameraBase.hpp FirstPersonDebugCamera.hpp ThirdPersonDebugCamera.hpp DESTINATION include/Urho3DDebugCamera)
