#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Ogg::ogg" for configuration "Debug"
set_property(TARGET Ogg::ogg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Ogg::ogg PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/oggd.lib"
  )

list(APPEND _cmake_import_check_targets Ogg::ogg )
list(APPEND _cmake_import_check_files_for_Ogg::ogg "${_IMPORT_PREFIX}/lib/oggd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
