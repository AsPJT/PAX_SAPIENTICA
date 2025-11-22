# Read PAX SAPIENTICA version from Version.hpp
#
# This function reads version information from Library/PAX_SAPIENTICA/Version.hpp
# and sets the following variables in the parent scope:
#   - PAX_SAPIENTICA_LIBRARY_MAJOR
#   - PAX_SAPIENTICA_LIBRARY_MINOR
#   - PAX_SAPIENTICA_LIBRARY_PATCHLEVEL
#   - PAX_SAPIENTICA_LIBRARY_VERSION (combined version string)
#
# Usage:
#   include(cmake/ReadVersion.cmake)
#   read_pax_sapientica_version()

function(read_pax_sapientica_version)
    # Read Version.hpp file
    file(READ ${PROJECT_ROOT_DIR}/Library/PAX_SAPIENTICA/Version.hpp VERSION_HPP)

    # Extract MAJOR version
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MAJOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_MAJOR ${CMAKE_MATCH_1} PARENT_SCOPE)

    # Extract MINOR version
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MINOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_MINOR ${CMAKE_MATCH_1} PARENT_SCOPE)

    # Extract PATCHLEVEL version
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_PATCHLEVEL \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_PATCHLEVEL ${CMAKE_MATCH_1} PARENT_SCOPE)

    # Set combined version string
    set(PAX_SAPIENTICA_LIBRARY_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}" PARENT_SCOPE)
endfunction()
