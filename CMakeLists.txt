cmake_minimum_required(VERSION 3.14)
project(UVCStream)

set(CMAKE_CXX_STANDARD 17)

# 项目头文件，源文件
include_directories(${PROJECT_SOURCE_DIR}/Capture)
aux_source_directory(${PROJECT_SOURCE_DIR}/Capture CAP_SRC)

# 外部库路径
set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
# FFmpeg
set(FFMPEG_INCLUDE_DIR ${THIRD_PARTY_DIR}/ffmpeg/include)
set(FFMPEG_LIB_DIR ${THIRD_PARTY_DIR}/ffmpeg/lib)

include_directories(${FFMPEG_INCLUDE_DIR})

# 生成可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(UVCStream main.cpp ${CAP_SRC})

target_link_libraries(UVCStream
        d3d11
        dxgi
        ${FFMPEG_LIB_DIR}/avcodec.lib
        ${FFMPEG_LIB_DIR}/avformat.lib
        ${FFMPEG_LIB_DIR}/avutil.lib
        ${FFMPEG_LIB_DIR}/swscale.lib
)

# 自动拷贝共享库
# TODO: 只拷贝需要的共享库
set(FFMPEG_DLL_DIR ${THIRD_PARTY_DIR}/ffmpeg/bin)
set(TARGET_DIR1 ${PROJECT_SOURCE_DIR}/bin)
set(TARGET_DIR2 ${CMAKE_BINARY_DIR})
file(GLOB FFMPEG_DLLS "${FFMPEG_DLL_DIR}/*.dll")
foreach (DLL ${FFMPEG_DLLS})
    message(STATUS "Copying ${DLL} to ${TARGET_DIR1} and ${TARGET_DIR2}")
    add_custom_command(TARGET UVCStream POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}" "${TARGET_DIR1}"
            )
    add_custom_command(TARGET UVCStream POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}" "${TARGET_DIR2}"
            )
endforeach ()

message(STATUS "FFmpeg DLLs will be copied to ${TARGET_DIR1} and ${TARGET_DIR2}")

