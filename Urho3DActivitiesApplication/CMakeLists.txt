# Dependencies: UniversalException.
set (TARGET_NAME Urho3DActivitiesApplication)
define_source_files (RECURSE GLOB_H_PATTERNS "*.hpp")
define_dependency_libs (Urho3D)
setup_library (STATIC)
