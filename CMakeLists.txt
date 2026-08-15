cmake_minimum_required(VERSION 3.13)

# ============================================================
# Raspberry Pi Pico SDK
# ============================================================

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)

if(DEFINED ENV{PICO_SDK_PATH} AND NOT EXISTS "$ENV{PICO_SDK_PATH}/pico_sdk_init.cmake")
    unset(ENV{PICO_SDK_PATH})
endif()

set(PICO_SDK_CANDIDATES
    ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk
    ${CMAKE_CURRENT_LIST_DIR}/../pico-sdk-src
    ${USERHOME}/pico-sdk
    /opt/pico-sdk)

if(NOT DEFINED ENV{PICO_SDK_PATH} AND NOT PICO_SDK_PATH)
    foreach(candidate ${PICO_SDK_CANDIDATES})
        if(EXISTS ${candidate}/pico_sdk_init.cmake)
            set(PICO_SDK_PATH ${candidate} CACHE PATH "Path to the Raspberry Pi Pico SDK" FORCE)
            break()
        endif()
    endforeach()
endif()

if(NOT DEFINED ENV{PICO_SDK_PATH} AND NOT PICO_SDK_PATH)
    set(PICO_SDK_FETCH_FROM_GIT ON CACHE BOOL "Fetch Raspberry Pi Pico SDK when no local SDK is available" FORCE)
    set(PICO_SDK_FETCH_FROM_GIT_TAG ${sdkVersion} CACHE STRING "Raspberry Pi Pico SDK git tag" FORCE)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/pico_sdk_import.cmake)

# ============================================================
# Project
# ============================================================

project(CastleProcess C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# ============================================================
# Initialise Pico SDK
# ============================================================

pico_sdk_init()

# ============================================================
# Executable
# ============================================================

add_executable(CastleProcess
    CastleProcess.cpp
)

# ============================================================
# Include paths
# ============================================================

target_include_directories(CastleProcess PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(CastleProcess PRIVATE
    PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
)

# ============================================================
# Libraries
# ============================================================

target_link_libraries(CastleProcess
    pico_stdlib
    pico_unique_id
    
    hardware_adc
    hardware_pwm
    hardware_dma
    hardware_interp
    hardware_spi
    hardware_pio
    hardware_clocks
    hardware_flash
    hardware_i2c
)

pico_enable_stdio_usb(CastleProcess 0)
pico_set_binary_type(CastleProcess copy_to_ram)

# ============================================================
# Create UF2/BIN/etc
# ============================================================

pico_add_extra_outputs(CastleProcess)
