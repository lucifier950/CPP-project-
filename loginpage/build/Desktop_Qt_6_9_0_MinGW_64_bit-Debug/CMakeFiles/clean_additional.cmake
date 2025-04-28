# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\loginpage_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\loginpage_autogen.dir\\ParseCache.txt"
  "loginpage_autogen"
  )
endif()
