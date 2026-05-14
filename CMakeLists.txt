cmake_minimum_required(VERSION 3.10)

project(MY_SlayTheSpire VERSION 0.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES 
    main.cpp
    src/Game.cpp
    src/DataLoader.cpp
    src/Debug.cpp
    src/Battle.cpp
    src/Effect.cpp
    src/Map.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Data"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Data"
    COMMENT "正在将 Data 文件夹拷贝到输出目录，确保程序能读取到卡牌数据..."
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()