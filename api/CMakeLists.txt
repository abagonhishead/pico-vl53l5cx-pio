add_library(vl53l5cx INTERFACE)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

target_sources(vl53l5cx INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/platform.c
    ${CMAKE_CURRENT_LIST_DIR}/vl53l5cx_api.c
    ${CMAKE_CURRENT_LIST_DIR}/vl53l5cx_plugin_detection_thresholds.c
    ${CMAKE_CURRENT_LIST_DIR}/vl53l5cx_plugin_motion_indicator.c
    ${CMAKE_CURRENT_LIST_DIR}/vl53l5cx_plugin_xtalk.c
    ${CMAKE_CURRENT_LIST_DIR}/pio_i2c.c
)

pico_generate_pio_header(vl53l5cx ${CMAKE_CURRENT_LIST_DIR}/i2c.pio)


target_link_libraries(vl53l5cx
    INTERFACE
    pico_stdlib
    
    hardware_pio
)