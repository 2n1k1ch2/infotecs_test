cmake_minimum_required(VERSION 3.10)
project(LoggerLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Статическая библиотека
add_library(logger_static STATIC 
    src/logger.cpp
    src/socket_logger.cpp
)
target_include_directories(logger_static PUBLIC inc)

# Динамическая библиотека
add_library(logger_shared SHARED 
    src/logger.cpp
    src/socket_logger.cpp  
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


enable_testing()

add_executable(logger_tests
    test/test_logger.cpp
    test/test_utils.cpp
    test/test_main.cpp
    src/logger.cpp
    src/logqueue.cpp
    src/utils.cpp
)
target_include_directories(logger_tests PRIVATE inc)
target_link_libraries(logger_tests PRIVATE logger_static)

add_test(NAME logger_tests COMMAND logger_tests)

# запуск тестов после компиляции
add_custom_command(TARGET logger_tests POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Указываем, что тесты должны проходить перед сборкой основных целей
add_dependencies(loggerapp logger_tests)
