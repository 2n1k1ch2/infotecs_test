cmake_minimum_required(VERSION 3.10)
project(LoggerLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
add_library(logger_static STATIC 
    src/logger.cpp
    src/socket_logger.cpp  # Добавляем новый файл
)
target_include_directories(logger_static PUBLIC inc)

# Динамическая библиотека
add_library(logger_shared SHARED 
    src/logger.cpp
    src/socket_logger.cpp  # Добавляем новый файл
)
target_include_directories(logger_shared PUBLIC inc)
set_target_properties(logger_shared PROPERTIES OUTPUT_NAME "logger")


#Часть №2
add_executable( loggerapp 
app/task2.cpp
src/logqueue.cpp
src/utils.cpp
)
target_include_directories(loggerapp PRIVATE inc)

target_link_libraries(loggerapp PRIVATE logger_static)

#Часть №3 
add_executable(task3 
    app/task3.cpp
    src/utils.cpp
    src/socket_logger.cpp
)
target_include_directories(task3 PRIVATE inc)
target_link_libraries(task3 PRIVATE logger_static)