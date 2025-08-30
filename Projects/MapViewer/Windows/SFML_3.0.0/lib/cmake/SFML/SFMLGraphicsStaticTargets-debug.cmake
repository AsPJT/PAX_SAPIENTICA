#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::Graphics" for configuration "Debug"
set_property(TARGET SFML::Graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SFML::Graphics PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/sfml-graphics-s-d.lib"
  )

list(APPEND _cmake_import_check_targets SFML::Graphics )
list(APPEND _cmake_import_check_files_for_SFML::Graphics "${_IMPORT_PREFIX}/lib/sfml-graphics-s-d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
