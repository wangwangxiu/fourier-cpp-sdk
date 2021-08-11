cmake_minimum_required(VERSION 3.0)
project(amber_cpp_examples)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
get_filename_component(ROOT_DIR "${ROOT_DIR}" REALPATH)


if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(AMBER_DIR ${ROOT_DIR})

set(amber_cpp_build_dir amber-cpp-${CMAKE_BUILD_TYPE})
string(TOLOWER ${amber_cpp_build_dir} amber_cpp_build_dir)
get_filename_component(amber_cpp_build_dir "${CMAKE_CURRENT_BINARY_DIR}/${amber_cpp_build_dir}" REALPATH)

add_subdirectory(${AMBER_DIR} ${amber_cpp_build_dir})

# 添加库目录
include_directories(${ROOT_DIR}/src)
include_directories(${ROOT_DIR}/amber/include)

link_directories(
    ${ROOT_DIR}/amber/lib/win_x64
    ${ROOT_DIR}/amber/lib/mac_x86_64
    ${ROOT_DIR}/amber/lib/linux_armv7l
    ${ROOT_DIR}/amber/lib/linux_x86_64
)

# 构建AIOS例子
SET(AIOS_SOURCES
  ${ROOT_DIR}/examples/src/01_lookup.cpp
  ${ROOT_DIR}/examples/src/03_feedback.cpp
  ${ROOT_DIR}/examples/src/04_command.cpp
  ${ROOT_DIR}/examples/src/05_latency_test.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../bin)
foreach (EXAMPLE ${AIOS_SOURCES})

  # 获取单个文件的文件名
  get_filename_component(EX_NAME ${EXAMPLE} NAME_WE)
  # 编译可执行文件
  add_executable(${EX_NAME} ${EXAMPLE})

  # 链接静态库
  target_link_libraries(${EX_NAME} amberc++-static)

  target_link_libraries(${EX_NAME} amber)

  # 创建日志目录，以备示例记录数据
  add_custom_command(TARGET ${EX_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E make_directory
                         $<TARGET_FILE_DIR:${EX_NAME}>/logs)

endforeach (EXAMPLE ${AIOS_SOURCES})

