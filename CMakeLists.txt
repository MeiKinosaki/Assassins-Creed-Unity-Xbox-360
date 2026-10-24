cmake_minimum_required(VERSION 3.10)
project(ACUnity360GameplayCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(acunity360_gameplay INTERFACE)
target_include_directories(acunity360_gameplay INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gameplay
    ${CMAKE_CURRENT_SOURCE_DIR}/src/save
)

enable_testing()

foreach(test_name
    test_inventory_system
    test_gameplay_event_queue
    test_objective_tracker
    test_gameplay_systems
    test_save_data_codec
    test_gameplay_save_adapter
    test_equipment_system
    test_platform_services
    test_gameplay_runtime
    test_save_storage_service
)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE acunity360_gameplay)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_name} PRIVATE -Wall -Wextra -Werror -pedantic -UNDEBUG)
    endif()
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
