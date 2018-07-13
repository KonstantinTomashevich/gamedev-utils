# Copy Urho3D CMake modules.
if (NOT CMAKE_MODULE_PATH)
    message (STATUS "CMAKE_MODULE_PATH isn't setted! Setting CMAKE_MODULE_PATH to urho3d-cmake/Modules.")
    set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/urho3d-cmake/Modules")
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
        message (STATUS "Copying Urho3D's CMake modules...")
        if (NOT EXISTS "$ENV{URHO3D_HOME}/share/CMake/")
            file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
        else ()
            file (COPY "$ENV{URHO3D_HOME}/share/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake")
        endif ()
    endif ()
    message (STATUS "Setted CMAKE_MODULE_PATH to urho3d-cmake/Modules!")
endif ()

# Copy Urho3D CoreData.
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/CoreData")
    message (STATUS "Copying Urho3D's CoreData asset...")
    if (NOT EXISTS "$ENV{URHO3D_HOME}/share/Resources/CoreData/")
        file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
    else ()
        file (COPY "$ENV{URHO3D_HOME}/share/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
    endif ()
endif ()

# Include Urho3D package.
include (UrhoCommon)
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})
