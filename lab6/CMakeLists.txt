cmake_minimum_required(VERSION 3.16)
project(lab6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)
    set(QT Qt5)
else()
    set(QT Qt6)
endif()

qt_add_executable(lab6 main.cpp)

target_link_libraries(lab6 PRIVATE ${QT}::Widgets ${QT}::Sql)

add_custom_command(TARGET lab6 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/input_tree.txt"
        "$<TARGET_FILE_DIR:lab6>/input_tree.txt"
)
