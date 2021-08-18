# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
# cmake需要的最低版本
cmake_minimum_required (VERSION 3.8)

# 项目名称
cmake_policy(SET CMP0048 NEW)
project (amber_cpp_api VERSION 0.4.0 LANGUAGES CXX)


# 添加编译宏
# add_definitions(-DDEBUG)

# 指定头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/amber/include)


# 跨平台引入库目录头文件
IF (WIN32)
    link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/amber/lib/win_x64
    )
ELSEIF (APPLE)
    link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/amber/lib/mac_x86_64
    )
ELSEIF (UNIX)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    if( ${ARCHITECTURE} STREQUAL  "armv7l" )
        link_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/amber/lib/linux_armv7l
        )
    elseif( ${ARCHITECTURE} STREQUAL  "x86_64" )
        link_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/amber/lib/linux_x86_64
        )
    else()
        message( "ERROR: Platform architecture mismatch" )
    endif()
ENDIF ()

# 编译静态库
set(amber_cpp_src
    src/lookup.cpp
    src/groupCommand.cpp
    src/groupFeedback.cpp
    src/group.cpp
    src/version.cpp
)

# 编译库
add_library(_amberc++-obj OBJECT ${amber_cpp_src})
add_library(amberc++ SHARED $<TARGET_OBJECTS:_amberc++-obj>)
add_library(amberc++-static STATIC $<TARGET_OBJECTS:_amberc++-obj>)

# 链接动态库amber
target_link_libraries(amberc++ PUBLIC amber)
target_link_libraries(amberc++-static PUBLIC amber)

target_include_directories(_amberc++-obj
  PUBLIC src include amber
)
target_include_directories(amberc++ INTERFACE $<TARGET_PROPERTY:_amberc++-obj,INTERFACE_INCLUDE_DIRECTORIES>)
target_include_directories(amberc++-static INTERFACE $<TARGET_PROPERTY:_amberc++-obj,INTERFACE_INCLUDE_DIRECTORIES>)

# 项目支持C++14
set_target_properties(_amberc++-obj amberc++ amberc++-static PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  POSITION_INDEPENDENT_CODE ON
  VERSION ${amber_cpp_api_VERSION})

if (WIN32)
  set_target_properties(amberc++ PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set_property(TARGET amberc++ amberc++-static PROPERTY INTERFACE_LINK_LIBRARIES
    $<$<BOOL:${UNIX}>:m pthread>
    $<$<BOOL:${WIN32}>:kernel32>
)
