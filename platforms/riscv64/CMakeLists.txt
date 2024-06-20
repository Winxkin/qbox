set(LIBQEMU_TARGETS riscv64)

add_executable(riscv64-base-vp
    src/main.cc
)

add_executable(riscv64_cpu
    src/riscv64_cpu.cc
)

target_link_libraries(riscv64-base-vp
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_link_libraries(riscv64_cpu
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_include_directories(
    riscv64-base-vp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

  target_include_directories(
    riscv64_cpu PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

