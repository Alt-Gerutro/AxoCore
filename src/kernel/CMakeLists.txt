add_executable(kernel.elf
    kernel.c
)

set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS "${LINKER_FLAGS}"
)

target_link_libraries(kernel.elf drivers interrupts timer)