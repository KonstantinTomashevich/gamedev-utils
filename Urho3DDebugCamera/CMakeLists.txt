set (TARGET_NAME Urho3DDebugCamera)
define_source_files (RECURSE GLOB_H_PATTERNS "*.hpp")
define_dependency_libs (Urho3D)
setup_library (STATIC)
