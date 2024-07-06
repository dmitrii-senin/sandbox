add_library(ProjectOptions INTERFACE)
add_library(Project::Options ALIAS ProjectOptions)

If (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
message(STATUS "Setting C++ standard to C++${CMAKE_CXX_STANDARD}")

target_compile_features(ProjectOptions INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
