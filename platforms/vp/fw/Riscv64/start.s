.global _start
.section .text

_start:

.option arch, +zicsr

csrr t0, mhartid         # read hardware thread ID
bnez t0, _idle           # let default core to run and halt other cores

la sp, _initial_sp       # Setup stack pointer

# Enable exception handler
#    la t1, exception_handler
#    csrw mtvec, t1

# Copy .data from ROM to RAM
#    la a0, __data_rom_addr
#    la a1, __data
#    la a2, __edata
#    beq a1, a2, clear_bss
#copy_loop:
#    ld a3, 0(a0)
#    sd a3, 0(a1)
#    addi a0, a0, 8
#    addi a1, a1, 8
#    blt a1, a2, copy_loop

#clear_bss:
#    la a0, __bss_start
#    la a1, __bss_end
#    beq a0, a1, entry_main

#set_loop:
#    sd zero, 0(a0)
#    addi a0, a0, 8
#    blt a0, a1, set_loop

entry_main:
# Prepare valid argc, argv and envp for main, just in case main uses them
    li a0, 0    # argc = 0
    li a1, 0    # argv = NULL
    li a2, 0    # envp = NULL
    call main

_idle:                  # Infinite loop
  wfi
  j _idle