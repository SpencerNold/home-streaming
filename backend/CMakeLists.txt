cmake_minimum_required(VERSION 3.29.3)

project(home-streaming-backend)

find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)

link_directories(${AVFORMAT_LIBRARY_DIRS})
link_directories(${AVCODEC_LIBRARY_DIRS})
link_directories(${AVUTIL_LIBRARY_DIRS})

add_executable(${PROJECT_NAME}
    src/main.c src/server.c
)

target_link_libraries(${PROJECT_NAME}
    ${OPENSSL_LIBRARIES}
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
)

target_compile_options(${PROJECT_NAME}
    PRIVATE ${AVFORMAT_CFLAGS_OTHER}
    ${AVCODEC_CFLAGS_OTHER}
    ${AVUTIL_CFLAGS_OTHER}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PUBLIC ${AVFORMAT_INCLUDE_DIRS}
    PUBLIC ${AVCODEC_INCLUDE_DIRS}
    PUBLIC ${AVUTIL_INCLUDE_DIRS}
    PUBLIC ${OPENSSL_INCLUDE_DIR}
)