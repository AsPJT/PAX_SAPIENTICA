#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "FLAC::FLAC" for configuration "Debug"
set_property(TARGET FLAC::FLAC APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(FLAC::FLAC PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;RC"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/FLACd.lib"
  )

list(APPEND _cmake_import_check_targets FLAC::FLAC )
list(APPEND _cmake_import_check_files_for_FLAC::FLAC "${_IMPORT_PREFIX}/lib/FLACd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
