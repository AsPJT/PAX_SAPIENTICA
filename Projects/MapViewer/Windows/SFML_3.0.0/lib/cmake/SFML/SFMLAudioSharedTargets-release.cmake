#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::Audio" for configuration "Release"
set_property(TARGET SFML::Audio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::Audio PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/sfml-audio.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sfml-audio-3.dll"
  )

list(APPEND _cmake_import_check_targets SFML::Audio )
list(APPEND _cmake_import_check_files_for_SFML::Audio "${_IMPORT_PREFIX}/lib/sfml-audio.lib" "${_IMPORT_PREFIX}/bin/sfml-audio-3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
