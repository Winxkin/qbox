/*
 * This file is part of libqbox
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All Rights Reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LIBQBOX_COMPONENTS_VIRTIO_GPU_PCI_H
#define _LIBQBOX_COMPONENTS_VIRTIO_GPU_PCI_H

#include <module_factory_registery.h>

#include <virtio_gpu.h>

class virtio_gpu_pci : public QemuVirtioGpu
{
public:
    virtio_gpu_pci(const sc_core::sc_module_name& name, sc_core::sc_object* o, sc_core::sc_object* t)
        : virtio_gpu_pci(name, *(dynamic_cast<QemuInstance*>(o)), (dynamic_cast<qemu_gpex*>(t)))
    {
    }
    virtio_gpu_pci(const sc_core::sc_module_name& name, QemuInstance& inst, qemu_gpex* gpex): QemuVirtioGpu(name, inst, "pci", gpex) {}
};

extern "C" void module_register();
#endif
