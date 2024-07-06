file(GLOB_RECURSE all_hfiles CONFIGURE_DEPENDS *.h *.hpp)
file(GLOB_RECURSE all_cxxfiles CONFIGURE_DEPENDS *.cxx *.cc *.cpp)

set(private_hfiles ${all_hfiles})
list(FILTER private_hfiles INCLUDE REGEX "/internal/")
set(public_hfiles ${all_hfiles})
list(REMOVE_ITEM public_hfiles ${private_hfiles})

add_library(Tbx)

if (private_files)
  target_sources(Tbx PRIVATE ${private_files})
endif()
if (public_files)
  target_sources(Tbx PUBLIC ${public_files})
endif()
