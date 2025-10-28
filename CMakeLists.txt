cmake_minimum_required(VERSION 3.20)
project(AxoCore C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_LINKER ld.lld)

set(CMAKE_C_FLAGS "-target x86_64-pc-none-elf -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-PIC -fno-PIE -nostdlib -O0 -Wall -Wextra -Wno-unused-command-line-argument -gdwarf-4 -g")
set(LINKER_FLAGS "-fuse-ld=lld -T${CMAKE_SOURCE_DIR}/include/config/kernel/linker.ld -nostdlib -static -z max-page-size=0x1000 -g")

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/kernel)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/arch/x86/interrupts)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/arch/x86/timer)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/drivers)

add_custom_target(iso ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory iso_root/boot/grub
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:kernel.elf> iso_root/boot
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/include/config/grub/grub.cfg iso_root/boot/grub
    COMMAND grub-mkrescue -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso iso_root
    DEPENDS kernel.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building .iso file"
)

find_program(QEMU qemu-system-x86_64)
find_program(ALACRITTY alacritty)

add_custom_target(run
    COMMAND ${QEMU}
        -cdrom ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso
        -no-reboot
    DEPENDS iso
    COMMENT "Starting ${PROJECT_NAME} QEMU"
)

add_custom_target(run_debug
    COMMAND ${QEMU}
        -cdrom ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.iso
        -no-reboot
        -no-shutdown
        -machine q35
        -gdb tcp::1234
        -S
        &
    COMMAND sh -c "${ALACRITTY} -e gdb -ex 'set architecture i386:x86-64' -ex 'target remote localhost:1234' -ex 'layout asm' $<TARGET_FILE:kernel.elf>"
    DEPENDS iso
    COMMENT "Starting ${PROJECT_NAME} QEMU in DEBUG MODE"
)