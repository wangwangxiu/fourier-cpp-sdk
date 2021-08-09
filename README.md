# amber_cpp_sdk

[![Action Status](https://github.com/AMBERobotics/amber_cpp_sdk/actions/workflows/main.yml/badge.svg?branch=main)](https://github.com/AMBERobotics/amber_cpp_sdk/actions/workflows/main.yml)

## 环境

git

cmake

## 使用说明

### 将amber_cpp_sdk克隆到你的项目

`git clone https://github.com/AMBERobotics/amber_cpp_sdk.git`

#### 在CMakeLists.txt中使用说明

在你的CMakeLists.txt中添加

```C++
set(AMBER_DIR amber_cpp_sdk)
set(amber_cpp_sdk_build_dir amber_cpp_sdk-${CMAKE_BUILD_TYPE})
string(TOLOWER ${amber_cpp_sdk_build_dir} amber_cpp_sdk_build_dir)
get_filename_component(amber_cpp_sdk_build_dir "${CMAKE_CURRENT_BINARY_DIR}/${amber_cpp_sdk_build_dir}" REALPATH)

add_subdirectory(${AMBER_DIR} ${amber_cpp_build_dir})

IF (WIN32)
    link_directories(
        ${PROJECT_SOURCE_DIR}/amber_cpp_sdk/amber/lib/win_x64
    )
ELSEIF (APPLE)
    link_directories(
        ${PROJECT_SOURCE_DIR}/amber_cpp_sdk/amber/lib/mac_x86_64
    )
ELSEIF (UNIX)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    message( STATUS "Architecture: ${ARCHITECTURE}" )
    if( ${ARCHITECTURE} STREQUAL  "armv7l" )
        link_directories(
            ${PROJECT_SOURCE_DIR}/amber_cpp_sdk/amber/lib/linux_armv7l
        )
    elseif( ${ARCHITECTURE} STREQUAL  "x86_64" )
        link_directories(
            ${PROJECT_SOURCE_DIR}/amber_cpp_sdk/amber/lib/linux_x86_64
        )
    else()
        message( "ERROR: Platform architecture mismatch" )
    endif()
ENDIF ()

target_link_libraries(${PROJECT_NAME}
    # 链接动态库
    PRIVATE amber amberc++
)
```

## 接口文档

自动生成的接口文档参考doc文件夹。

## [AMBER Robotics](http://wiki.amberobotics.com/getting-started_cn/)

## 第三方库

amber_cpp_sdk中集成了[spdlog](https://github.com/gabime/spdlog)、[yaml-cpp](https://github.com/jbeder/yaml-cpp)、[json](https://github.com/nlohmann/json)等第三方库
