gs_create_dymod(cpu_vt_riscv)

target_include_directories(
    cpu_vt_riscv PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/qemu-components>
  )