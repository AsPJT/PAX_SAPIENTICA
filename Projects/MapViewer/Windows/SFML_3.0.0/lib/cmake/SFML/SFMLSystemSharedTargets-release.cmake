#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::System" for configuration "Release"
set_property(TARGET SFML::System APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::System PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/sfml-system.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sfml-system-3.dll"
  )

list(APPEND _cmake_import_check_targets SFML::System )
list(APPEND _cmake_import_check_files_for_SFML::System "${_IMPORT_PREFIX}/lib/sfml-system.lib" "${_IMPORT_PREFIX}/bin/sfml-system-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
