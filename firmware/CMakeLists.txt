cmake_minimum_required(VERSION 3.13)
find_package(Git REQUIRED)
execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD OUTPUT_VARIABLE COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Computed Git Hash: ${COMMIT_ID}")
add_definitions(-DGIT_HASH="${COMMIT_ID}") # Usable in source code.

#add_definitions(-DDEBUG) # Uncomment for debugging
add_definitions(-DUSBD_MANUFACTURER="Allen Institute")
add_definitions(-DUSBD_PRODUCT="valve-controller")

# PICO_SDK_PATH must be defined.
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# Use modern conventions like std::invoke
set(CMAKE_CXX_STANDARD 17)

project(valve-controller)

pico_sdk_init()
add_subdirectory(lib/harp.core.rp2040/firmware) # Path to harp.core.rp2040.
add_subdirectory(lib/rp2040.pwm)


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/valve_controller_app.cpp
)

add_library(valve_driver
    src/valve_driver.cpp
)

include_directories(inc)

target_link_libraries(valve_driver rp2040_pwm pico_stdlib)
target_link_libraries(${PROJECT_NAME} harp_core harp_c_app rp2040_pwm
                      valve_driver harp_sync pico_stdlib)

pico_add_extra_outputs(${PROJECT_NAME})

if(DEBUG)
    message(WARNING "Debug printf() messages from harp core to UART with baud \
            rate 921600.")
    pico_enable_stdio_uart(${PROJECT_NAME} 1) # UART stdio for printf.
    # Additional libraries need to have stdio init also.
endif()

