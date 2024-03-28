add_library(BGT60LTR11XXX INTERFACE)

target_include_directories(BGT60LTR11XXX INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(BGT60LTR11XXX INTERFACE
    pico_stdlib  
    hardware_spi  
)
