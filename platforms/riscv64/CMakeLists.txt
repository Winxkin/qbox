set(LIBQEMU_TARGETS riscv64)

add_executable(riscv64-base-vp
    src/riscv64_cpu.cc
)

if(APPLE)
    target_link_options(riscv64-base-vp PRIVATE "LINKER:-dead_strip_dylibs")
else()
    target_link_options(riscv64-base-vp PRIVATE "LINKER:-as-needed")
endif()

target_link_libraries(riscv64-base-vp
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_include_directories(
    riscv64-base-vp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )
