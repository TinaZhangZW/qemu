/*
 * virtio-iommu device
 *
 * Copyright (c) 2020 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef QEMU_VIRTIO_IOMMU_H
#define QEMU_VIRTIO_IOMMU_H

#include "standard-headers/linux/virtio_iommu.h"
#include "hw/virtio/virtio.h"
#include "hw/virtio/vhost.h"
#include "hw/pci/pci.h"
#include "qom/object.h"

#define TYPE_VIRTIO_IOMMU "virtio-iommu-device"
#define TYPE_VIRTIO_IOMMU_PCI "virtio-iommu-pci"
OBJECT_DECLARE_SIMPLE_TYPE(VirtIOIOMMU, VIRTIO_IOMMU)

#define TYPE_VIRTIO_IOMMU_MEMORY_REGION "virtio-iommu-memory-region"

typedef struct Hwpt {
    uint32_t hwpt_id;
    int iommufd;
    uint32_t parent_id; /* ioas_id or hwpt_id */
    uint32_t users;
} Hwpt;

typedef struct PASIDAddressSpace {
    PCIBus *bus;
    uint8_t devfn;
    uint32_t pasid;
    Hwpt hwpt;
} PASIDAddressSpace;

typedef struct IOMMUDevice {
    void         *viommu;
    PCIBus       *bus;
    int           devfn;
    IOMMUMemoryRegion  iommu_mr;
    AddressSpace  as;
    MemoryRegion root;          /* The root container of the device */
    MemoryRegion bypass_mr;     /* The alias of shared memory MR */
    IOMMUFDDevice *idev;
} IOMMUDevice;

typedef struct IOMMUPciBus {
    PCIBus       *bus;
    IOMMUDevice  *pbdev[]; /* Parent array is sparse, so dynamically alloc */
} IOMMUPciBus;

struct vhost_iommu_ {
    struct vhost_dev dev;
    struct vhost_virtqueue *vqs;
    uint8_t *probe_buffer;
    int fd;
    bool started;
};

struct VirtIOIOMMU {
    VirtIODevice parent_obj;
    VirtQueue *event_vq;
    /* Array of num_queues pointers */
    VirtQueue **req_vq;
    struct virtio_iommu_config config;
    uint64_t features;
    GHashTable *as_by_busptr;
    IOMMUPciBus *iommu_pcibus_by_bus_num[PCI_BUS_MAX];
    PCIBus *primary_bus;
    ReservedRegion *reserved_regions;
    uint32_t nb_reserved_regions;
    GTree *domains;
    QemuRecMutex mutex;
    GTree *endpoints;
    bool boot_bypass;
    bool bypass_feature;
    uint16_t num_queues;
    bool page_tables;
    bool posted_map;
    void *hw_info;
    GTree *pasid_ass;       /* PASIDAddressSpace instances */
    Hwpt *s2_hwpt;
    bool use_vhost;
    struct vhost_iommu_ *vhost_iommu;
};

#endif
