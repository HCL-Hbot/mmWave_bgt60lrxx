add_executable(MAIN
    main.cpp
)

target_link_libraries(MAIN
    pico_stdlib
    BGT60LTR11XXX
)

pico_add_extra_outputs(MAIN)