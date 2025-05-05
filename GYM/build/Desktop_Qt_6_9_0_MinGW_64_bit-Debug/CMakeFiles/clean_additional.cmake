# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GYM_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GYM_autogen.dir\\ParseCache.txt"
  "GYM_autogen"
  )
endif()
