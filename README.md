# fourier-cpp-sdk

[![Action Status](https://github.com/FourierDynamics/fourier-cpp-sdk/actions/workflows/main.yml/badge.svg?branch=main)](https://github.com/FourierDynamics/fourier-cpp-sdk/actions/workflows/main.yml)

## 环境依赖

git

cmake

## 使用说明

### 将fourier_cpp_sdk克隆到你的项目

`git clone https://github.com/FourierDynamics/fourier-cpp-sdk`

#### 在CMakeLists.txt中使用说明

在你的CMakeLists.txt中添加

```C++
set(FOURIER_DIR fourier-cpp-sdk)
set(fourier_cpp_sdk_build_dir fourier-cpp-sdk-${CMAKE_BUILD_TYPE})
string(TOLOWER ${fourier_cpp_sdk_build_dir} fourier_cpp_sdk_build_dir)
get_filename_component(fourier_cpp_sdk_build_dir "${CMAKE_CURRENT_BINARY_DIR}/${fourier_cpp_sdk_build_dir}" REALPATH)

add_subdirectory(${FOURIER_DIR} ${fourier_cpp_build_dir})

IF (WIN32)
    link_directories(
        ${PROJECT_SOURCE_DIR}/fourier-cpp-sdk/fourier/lib/win_x64
    )
ELSEIF (APPLE)
    link_directories(
        ${PROJECT_SOURCE_DIR}/fourier-cpp-sdk/fourier/lib/mac_x86_64
    )
ELSEIF (UNIX)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
    message( STATUS "Architecture: ${ARCHITECTURE}" )
    if( ${ARCHITECTURE} STREQUAL  "armv7l" )
        link_directories(
            ${PROJECT_SOURCE_DIR}/fourier-cpp-sdk/fourier/lib/linux_armv7l
        )
    elseif( ${ARCHITECTURE} STREQUAL  "x86_64" )
        link_directories(
            ${PROJECT_SOURCE_DIR}/fourier-cpp-sdk/fourier/lib/linux_x86_64
        )
    else()
        message( "ERROR: Platform architecture mismatch" )
    endif()
ENDIF ()

target_link_libraries(${PROJECT_NAME}
    PRIVATE fourier fourierc++
)
```

## 接口文档

自动生成的接口文档参考doc文件夹。

## [FOURIER Robotics](http://wiki.fourierobotics.com/getting-started_cn/)

## 第三方库

fourier_cpp_sdk中集成了[spdlog](https://github.com/gabime/spdlog)、[yaml-cpp](https://github.com/jbeder/yaml-cpp)、[json](https://github.com/nlohmann/json)等第三方库
