#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "minhook::minhook" for configuration "Release"
set_property(TARGET minhook::minhook APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(minhook::minhook PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/minhook.x64.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/minhook.x64.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS minhook::minhook )
list(APPEND _IMPORT_CHECK_FILES_FOR_minhook::minhook "${_IMPORT_PREFIX}/lib/minhook.x64.lib" "${_IMPORT_PREFIX}/bin/minhook.x64.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
