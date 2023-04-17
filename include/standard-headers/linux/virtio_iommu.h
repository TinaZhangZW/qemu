/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Virtio-iommu definition v0.12
 *
 * Copyright (C) 2019-2021 Arm Ltd.
 */
#ifndef _LINUX_VIRTIO_IOMMU_H
#define _LINUX_VIRTIO_IOMMU_H

#include "standard-headers/linux/types.h"

/* Feature bits */
#define VIRTIO_IOMMU_F_INPUT_RANGE		0
#define VIRTIO_IOMMU_F_DOMAIN_RANGE		1
#define VIRTIO_IOMMU_F_MAP_UNMAP		2
#define VIRTIO_IOMMU_F_BYPASS			3
#define VIRTIO_IOMMU_F_PROBE			4
#define VIRTIO_IOMMU_F_MMIO			5
#define VIRTIO_IOMMU_F_BYPASS_CONFIG		6
#define VIRTIO_IOMMU_F_MQ			7
#define VIRTIO_IOMMU_F_ATTACH_TABLE		8
#define VIRTIO_IOMMU_F_POSTED_MAP		9

struct virtio_iommu_range_64 {
	uint64_t					start;
	uint64_t					end;
};

struct virtio_iommu_range_32 {
	uint32_t					start;
	uint32_t					end;
};

struct virtio_iommu_config {
	/* Supported page sizes */
	uint64_t					page_size_mask;
	/* Supported IOVA range */
	struct virtio_iommu_range_64		input_range;
	/* Max domain ID size */
	struct virtio_iommu_range_32		domain_range;
	/* Probe buffer size */
	uint32_t					probe_size;
	uint8_t					bypass;
	uint8_t					reserved;
	uint16_t					num_queues;
};

/* Request types */
#define VIRTIO_IOMMU_T_ATTACH			0x01
#define VIRTIO_IOMMU_T_DETACH			0x02
#define VIRTIO_IOMMU_T_MAP			0x03
#define VIRTIO_IOMMU_T_UNMAP			0x04
#define VIRTIO_IOMMU_T_PROBE			0x05
#define VIRTIO_IOMMU_T_ATTACH_TABLE		0x06
#define VIRTIO_IOMMU_T_INVALIDATE		0x07

/* Status types */
#define VIRTIO_IOMMU_S_OK			0x00
#define VIRTIO_IOMMU_S_IOERR			0x01
#define VIRTIO_IOMMU_S_UNSUPP			0x02
#define VIRTIO_IOMMU_S_DEVERR			0x03
#define VIRTIO_IOMMU_S_INVAL			0x04
#define VIRTIO_IOMMU_S_RANGE			0x05
#define VIRTIO_IOMMU_S_NOENT			0x06
#define VIRTIO_IOMMU_S_FAULT			0x07
#define VIRTIO_IOMMU_S_NOMEM			0x08

struct virtio_iommu_req_head {
	uint8_t					type;
	uint8_t					reserved[3];
};

struct virtio_iommu_req_tail {
	uint8_t					status;
	uint8_t					reserved[3];
};

#define VIRTIO_IOMMU_ATTACH_F_BYPASS		(1 << 0)
#define VIRTIO_IOMMU_ATTACH_F_POSTED		(1 << 1)

struct virtio_iommu_req_attach {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint32_t					endpoint;
	uint32_t					flags;
	uint8_t					reserved[4];
	struct virtio_iommu_req_tail		tail;
};

struct virtio_iommu_req_detach {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint32_t					endpoint;
	uint8_t					reserved[8];
	struct virtio_iommu_req_tail		tail;
};

struct virtio_iommu_req_attach_table {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint32_t					endpoint;
	uint32_t					flags;
	uint16_t					format;
	uint8_t					reserved[66];
	struct virtio_iommu_req_tail		tail;
};

#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_LINEAR	0x0
#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_4KL2	0x1
#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_64KL2	0x2

#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_DSS_TERM	0x0
#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_DSS_BYPASS 0x1
#define VIRTIO_IOMMU_PSTF_ARM_SMMU_V3_DSS_0	0x2

/* Arm SMMUv3 PASID Table Descriptor */
struct virtio_iommu_req_attach_pst_arm {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint32_t					endpoint;
	uint32_t					flags;
	uint16_t					format;
	uint8_t					s1fmt;
	uint8_t					s1dss;
	uint32_t					s1cdmax;
	uint64_t					s1contextptr;
	uint8_t					reserved[48];
	struct virtio_iommu_req_tail		tail;
};

/* Virt I/O page table */
struct virtio_iommu_req_attach_pgt_virt {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint32_t					endpoint;
	uint32_t					flags;
	uint16_t					format;
	uint8_t					reserved[6];
	uint64_t					pgd;
	uint8_t					reserved2[48];
	struct virtio_iommu_req_tail		tail;
};

#define VIRTIO_IOMMU_MAP_F_READ			(1 << 0)
#define VIRTIO_IOMMU_MAP_F_WRITE		(1 << 1)
#define VIRTIO_IOMMU_MAP_F_MMIO			(1 << 2)

#define VIRTIO_IOMMU_MAP_F_MASK			(VIRTIO_IOMMU_MAP_F_READ |	\
						 VIRTIO_IOMMU_MAP_F_WRITE |	\
						 VIRTIO_IOMMU_MAP_F_MMIO)

struct virtio_iommu_req_map {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint64_t					virt_start;
	uint64_t					virt_end;
	uint64_t					phys_start;
	uint32_t					flags;
	struct virtio_iommu_req_tail		tail;
};

struct virtio_iommu_req_unmap {
	struct virtio_iommu_req_head		head;
	uint32_t					domain;
	uint64_t					virt_start;
	uint64_t					virt_end;
	uint8_t					reserved[4];
	struct virtio_iommu_req_tail		tail;
};

#define VIRTIO_IOMMU_PROBE_T_NONE		0
#define VIRTIO_IOMMU_PROBE_T_RESV_MEM		1
#define VIRTIO_IOMMU_PROBE_T_PAGE_SIZE_MASK	2
#define VIRTIO_IOMMU_PROBE_T_INPUT_RANGE	3
#define VIRTIO_IOMMU_PROBE_T_OUTPUT_SIZE	4
#define VIRTIO_IOMMU_PROBE_T_PASID_SIZE		5
#define VIRTIO_IOMMU_PROBE_T_PAGE_TABLE_FMT	6
#define VIRTIO_IOMMU_PROBE_T_PASID_TABLE_FMT	7

#define VIRTIO_IOMMU_PROBE_T_MASK		0xfff

struct virtio_iommu_probe_property {
	uint16_t					type;
	uint16_t					length;
};

#define VIRTIO_IOMMU_RESV_MEM_T_RESERVED	0
#define VIRTIO_IOMMU_RESV_MEM_T_MSI		1

struct virtio_iommu_probe_resv_mem {
	struct virtio_iommu_probe_property	head;
	uint8_t					subtype;
	uint8_t					reserved[3];
	uint64_t					start;
	uint64_t					end;
};

struct virtio_iommu_probe_page_size_mask {
	struct virtio_iommu_probe_property	head;
	uint8_t					reserved[4];
	uint64_t					mask;
};

struct virtio_iommu_probe_input_range {
	struct virtio_iommu_probe_property	head;
	uint8_t					reserved[4];
	uint64_t					start;
	uint64_t					end;
};

struct virtio_iommu_probe_output_size {
	struct virtio_iommu_probe_property	head;
	uint8_t					bits;
	uint8_t					reserved[3];
};

struct virtio_iommu_probe_pasid_size {
	struct virtio_iommu_probe_property	head;
	uint8_t					bits;
	uint8_t					reserved[3];
};

/* Arm LPAE page table format */
#define VIRTIO_IOMMU_FOMRAT_PGTF_ARM_LPAE	1
/* Arm smmu-v3 type PASID table format */
#define VIRTIO_IOMMU_FORMAT_PSTF_ARM_SMMU_V3	2
/* Virt I/O page table format */
#define VIRTIO_IOMMU_FORMAT_PGTF_VIRT		3
/* VT-d I/O page table format */
#define VIRTIO_IOMMU_FORMAT_PGTF_VTD		4

struct virtio_iommu_probe_table_format {
	struct virtio_iommu_probe_property	head;
	uint16_t					format;
	uint8_t					reserved[2];
};

struct virtio_iommu_req_probe {
	struct virtio_iommu_req_head		head;
	uint32_t					endpoint;
	uint8_t					reserved[64];

	uint8_t					properties[];

	/*
	 * Tail follows the variable-length properties array. No padding,
	 * property lengths are all aligned on 8 bytes.
	 */
};

#define VIRTIO_IOMMU_INVAL_G_DOMAIN		(1 << 0)
#define VIRTIO_IOMMU_INVAL_G_PASID		(1 << 1)
#define VIRTIO_IOMMU_INVAL_G_VA			(1 << 2)

#define VIRTIO_IOMMU_INV_T_IOTLB		(1 << 0)
#define VIRTIO_IOMMU_INV_T_DEV_IOTLB		(1 << 1)
#define VIRTIO_IOMMU_INV_T_PASID		(1 << 2)

#define VIRTIO_IOMMU_INVAL_F_PASID		(1 << 0)
#define VIRTIO_IOMMU_INVAL_F_ARCHID		(1 << 1)
#define VIRTIO_IOMMU_INVAL_F_LEAF		(1 << 2)

struct virtio_iommu_req_invalidate {
	struct virtio_iommu_req_head		head;
	uint16_t					inv_gran;
	uint16_t					inv_type;

	uint16_t					flags;
	uint8_t					reserved1[2];
	uint32_t					domain;

	uint32_t					pasid;
	uint8_t					reserved2[4];

	uint64_t					archid;
	uint64_t					virt_start;
	uint64_t					nr_pages;

	/* Page size, in nr of bits, typically 12 for 4k, 30 for 2MB, etc.) */
	uint8_t					granule;
	uint8_t					reserved3[11];
	struct virtio_iommu_req_tail		tail;
};

/* Fault types */
#define VIRTIO_IOMMU_FAULT_R_UNKNOWN		0
#define VIRTIO_IOMMU_FAULT_R_DOMAIN		1
#define VIRTIO_IOMMU_FAULT_R_MAPPING		2

#define VIRTIO_IOMMU_FAULT_F_READ		(1 << 0)
#define VIRTIO_IOMMU_FAULT_F_WRITE		(1 << 1)
#define VIRTIO_IOMMU_FAULT_F_EXEC		(1 << 2)
#define VIRTIO_IOMMU_FAULT_F_ADDRESS		(1 << 8)

struct virtio_iommu_fault {
	uint8_t					reason;
	uint8_t					reserved[3];
	uint32_t					flags;
	uint32_t					endpoint;
	uint8_t					reserved2[4];
	uint64_t					address;
};

#endif
