set(LIBQEMU_TARGETS riscv64)

add_executable(riscv64-base-vp
    src/main.cc
)

add_executable(riscv_cpu
    src/riscv_cpu.cc
)

target_link_libraries(riscv64-base-vp
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_link_libraries(riscv_cpu
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_include_directories(
    riscv64-base-vp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

  target_include_directories(
    riscv_cpu PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

