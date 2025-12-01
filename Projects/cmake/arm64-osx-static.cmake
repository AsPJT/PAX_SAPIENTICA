set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Darwin)
set(VCPKG_OSX_ARCHITECTURES arm64)

# Force CMake to use native macOS OpenGL framework and ignore X11
set(VCPKG_CMAKE_CONFIGURE_OPTIONS
    "-DCMAKE_DISABLE_FIND_PACKAGE_X11=TRUE"
    "-DOpenGL_GL_PREFERENCE=GLVND"
    "-DOPENGL_INCLUDE_DIR=/System/Library/Frameworks/OpenGL.framework"
    "-DOPENGL_gl_LIBRARY=/System/Library/Frameworks/OpenGL.framework"
)
