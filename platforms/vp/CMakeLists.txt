set(LIBQEMU_TARGETS riscv64)

add_executable(riscv-64-vp
    src/main.cc
)

add_executable(vp_cpu
    src/remote_cpu.cc
)

target_link_libraries(riscv-64-vp
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_link_libraries(vp_cpu
    ${TARGET_LIBS}
    GreenSocs::qbox
)

target_include_directories(
    riscv-64-vp PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

  target_include_directories(
    vp_cpu PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/systemc-components>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )

set(BUILD_DIRECTORY "${CMAKE_BINARY_DIR}")


