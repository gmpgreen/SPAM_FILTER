# We use a cached string variable instead of the standard (boolean) OPTION
# command so that we can default to auto-detecting optional depencies, while
# still providing a mechanism to force/disable these optional dependencies, as
# prescribed in http://www.gentoo.org/proj/en/qa/automagic.xml

option (ENABLE_ASAN "Enable Address Sanitizer" ON)

option (ENABLE_UBSAN "Undefined Behaviour Sanitizer" ON)

# Reference code from https://github.com/apitrace/apitrace/blob/master/CMakeLists.txt
# Enable Address Sanitizer

if (ENABLE_UBSAN)
    set (_FLAGS "-fsanitize=undefined")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_FLAGS}")
endif ()

if (ENABLE_ASAN)
    set (_FLAGS "-fsanitize=address")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_FLAGS}")
endif ()

