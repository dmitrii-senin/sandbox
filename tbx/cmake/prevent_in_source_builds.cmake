#
# This function prevents in-source builds
#
function(prevent_in_source_builds)
  get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
  get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

  if("${srcdir}" STREQUAL "${bindir}")
    message("######################################################")
    message("Error: in-source builds are disabled")
    message("Required actions:")
    message("  1. Create a separate build directory")
    message("  2. Run cmake from the created build directory")
    message("######################################################")
    message(FATAL_ERROR "Quitting configuration...")
  endif()
endfunction()

prevent_in_source_builds()
