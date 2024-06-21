#include <vt_riscv.h>

void module_register()
{
    GSC_MODULE_REGISTER_C(vt_cpu_riscv64, sc_core::sc_object*, uint64_t);
}