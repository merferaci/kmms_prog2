# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  [[projects\261016_qt\CMakeFiles\keyboard_autogen.dir\AutogenUsed.txt]]
  [[projects\261016_qt\CMakeFiles\keyboard_autogen.dir\ParseCache.txt]]
  [[projects\261016_qt\keyboard_autogen]]
  )
endif()
