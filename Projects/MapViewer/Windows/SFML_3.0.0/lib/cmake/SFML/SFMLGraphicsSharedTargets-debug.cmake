#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::Graphics" for configuration "Debug"
set_property(TARGET SFML::Graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Graphics PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/sfml-graphics-d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/sfml-graphics-d-3.dll"
  )

list(APPEND _cmake_import_check_targets SFML::Graphics )
list(APPEND _cmake_import_check_files_for_SFML::Graphics "${_IMPORT_PREFIX}/lib/sfml-graphics-d.lib" "${_IMPORT_PREFIX}/bin/sfml-graphics-d-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
