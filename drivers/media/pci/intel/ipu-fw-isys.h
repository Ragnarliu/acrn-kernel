/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2013 - 2018 Intel Corporation */

#ifndef IPU_FW_ISYS_H
#define IPU_FW_ISYS_H

#include "ipu-fw-com.h"

/* Max number of Input/Output Pins */
#define IPU_MAX_IPINS 4

/* worst case is ISA use where a single input pin produces:
 * Mipi output, NS Pixel Output, and Scaled Pixel Output.
 * This is how the 2 is calculated
 */
#define IPU_MAX_OPINS ((IPU_MAX_IPINS) + 2)

/* Max number of supported virtual streams */
#if defined(CONFIG_VIDEO_INTEL_IPU4) || defined(CONFIG_VIDEO_INTEL_IPU4P)
#define IPU_STREAM_ID_MAX 8
#else
#define IPU_STREAM_ID_MAX 16
#endif

/* Aligned with the approach of having one dedicated per stream */
#define IPU_N_MAX_MSG_SEND_QUEUES (IPU_STREAM_ID_MAX)
/* Single return queue for all streams/commands type */
#define IPU_N_MAX_MSG_RECV_QUEUES 1
/* Single device queue for high priority commands (bypass in-order queue) */
#define IPU_N_MAX_DEV_SEND_QUEUES 1
/* Single dedicated send queue for proxy interface */
#define IPU_N_MAX_PROXY_SEND_QUEUES 1
/* Single dedicated recv queue for proxy interface */
#define IPU_N_MAX_PROXY_RECV_QUEUES 1
/* Send queues layout */
#define IPU_BASE_PROXY_SEND_QUEUES 0
#define IPU_BASE_DEV_SEND_QUEUES \
	(IPU_BASE_PROXY_SEND_QUEUES + IPU_N_MAX_PROXY_SEND_QUEUES)
#define IPU_BASE_MSG_SEND_QUEUES \
	(IPU_BASE_DEV_SEND_QUEUES + IPU_N_MAX_DEV_SEND_QUEUES)
#define IPU_N_MAX_SEND_QUEUES \
	(IPU_BASE_MSG_SEND_QUEUES + IPU_N_MAX_MSG_SEND_QUEUES)
/* Recv queues layout */
#define IPU_BASE_PROXY_RECV_QUEUES 0
#define IPU_BASE_MSG_RECV_QUEUES \
	(IPU_BASE_PROXY_RECV_QUEUES + IPU_N_MAX_PROXY_RECV_QUEUES)
#define IPU_N_MAX_RECV_QUEUES \
	(IPU_BASE_MSG_RECV_QUEUES + IPU_N_MAX_MSG_RECV_QUEUES)

/* Consider 1 slot per stream since driver is not expected to pipeline
 * device commands for the same stream
 */
#define IPU_DEV_SEND_QUEUE_SIZE (IPU_STREAM_ID_MAX)

/* Max number of supported SRAM buffer partitions.
 * It refers to the size of stream partitions.
 * These partitions are further subpartitioned internally
 * by the FW, but by declaring statically the stream
 * partitions we solve the buffer fragmentation issue
 */
#define IPU_NOF_SRAM_BLOCKS_MAX (IPU_STREAM_ID_MAX)

/* Max number of supported input pins routed in ISL */
#define IPU_MAX_IPINS_IN_ISL 2

/* Max number of planes for frame formats supported by the FW */
#define IPU_PIN_PLANES_MAX 4

/**
 * enum ipu_fw_isys_resp_type
 */
enum ipu_fw_isys_resp_type {
	IPU_FW_ISYS_RESP_TYPE_STREAM_OPEN_DONE = 0,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_AND_CAPTURE_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_STOP_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_FLUSH_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CLOSE_ACK,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_READY,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_WATERMARK,
	IPU_FW_ISYS_RESP_TYPE_FRAME_SOF,
	IPU_FW_ISYS_RESP_TYPE_FRAME_EOF,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_AND_CAPTURE_DONE,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_DONE,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_SKIPPED,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_SKIPPED,
	IPU_FW_ISYS_RESP_TYPE_FRAME_SOF_DISCARDED,
	IPU_FW_ISYS_RESP_TYPE_FRAME_EOF_DISCARDED,
	IPU_FW_ISYS_RESP_TYPE_STATS_DATA_READY,
	N_IPU_FW_ISYS_RESP_TYPE
};

/**
 * enum ipu_fw_isys_send_type
 */
enum ipu_fw_isys_send_type {
	IPU_FW_ISYS_SEND_TYPE_STREAM_OPEN = 0,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START_AND_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_STOP,
	IPU_FW_ISYS_SEND_TYPE_STREAM_FLUSH,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CLOSE,
	N_IPU_FW_ISYS_SEND_TYPE
};

/**
 * enum ipu_fw_isys_queue_type
 */
enum ipu_fw_isys_queue_type {
	IPU_FW_ISYS_QUEUE_TYPE_PROXY = 0,
	IPU_FW_ISYS_QUEUE_TYPE_DEV,
	IPU_FW_ISYS_QUEUE_TYPE_MSG,
	N_IPU_FW_ISYS_QUEUE_TYPE
};

/**
 * enum ipu_fw_isys_stream_source: Specifies a source for a stream
 */
enum ipu_fw_isys_stream_source {
	IPU_FW_ISYS_STREAM_SRC_PORT_0 = 0,
	IPU_FW_ISYS_STREAM_SRC_PORT_1,
	IPU_FW_ISYS_STREAM_SRC_PORT_2,
	IPU_FW_ISYS_STREAM_SRC_PORT_3,
	IPU_FW_ISYS_STREAM_SRC_PORT_4,
	IPU_FW_ISYS_STREAM_SRC_PORT_5,
	IPU_FW_ISYS_STREAM_SRC_PORT_6,
	IPU_FW_ISYS_STREAM_SRC_PORT_7,
	IPU_FW_ISYS_STREAM_SRC_PORT_8,
	IPU_FW_ISYS_STREAM_SRC_PORT_9,
	IPU_FW_ISYS_STREAM_SRC_PORT_10,
	IPU_FW_ISYS_STREAM_SRC_PORT_11,
	IPU_FW_ISYS_STREAM_SRC_PORT_12,
	IPU_FW_ISYS_STREAM_SRC_PORT_13,
	IPU_FW_ISYS_STREAM_SRC_PORT_14,
	IPU_FW_ISYS_STREAM_SRC_PORT_15,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_0,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_1,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_2,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_3,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_4,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_5,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_6,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_7,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_8,
	IPU_FW_ISYS_STREAM_SRC_MIPIGEN_9,
	N_IPU_FW_ISYS_STREAM_SRC
};

#define IPU_FW_ISYS_STREAM_SRC_CSI2_PORT0 IPU_FW_ISYS_STREAM_SRC_PORT_0
#define IPU_FW_ISYS_STREAM_SRC_CSI2_PORT1 IPU_FW_ISYS_STREAM_SRC_PORT_1
#define IPU_FW_ISYS_STREAM_SRC_CSI2_PORT2 IPU_FW_ISYS_STREAM_SRC_PORT_2
#define IPU_FW_ISYS_STREAM_SRC_CSI2_PORT3 IPU_FW_ISYS_STREAM_SRC_PORT_3

#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_PORTA IPU_FW_ISYS_STREAM_SRC_PORT_4
#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_PORTB IPU_FW_ISYS_STREAM_SRC_PORT_5
#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_CPHY_PORT0 IPU_FW_ISYS_STREAM_SRC_PORT_6
#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_CPHY_PORT1 IPU_FW_ISYS_STREAM_SRC_PORT_7
#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_CPHY_PORT2 IPU_FW_ISYS_STREAM_SRC_PORT_8
#define IPU_FW_ISYS_STREAM_SRC_CSI2_3PH_CPHY_PORT3 IPU_FW_ISYS_STREAM_SRC_PORT_9

#define IPU_FW_ISYS_STREAM_SRC_MIPIGEN_PORT0 IPU_FW_ISYS_STREAM_SRC_MIPIGEN_0
#define IPU_FW_ISYS_STREAM_SRC_MIPIGEN_PORT1 IPU_FW_ISYS_STREAM_SRC_MIPIGEN_1

/**
 * enum ipu_fw_isys_mipi_vc: MIPI csi2 spec
 * supports up to 4 virtual per physical channel
 */
enum ipu_fw_isys_mipi_vc {
	IPU_FW_ISYS_MIPI_VC_0 = 0,
	IPU_FW_ISYS_MIPI_VC_1,
	IPU_FW_ISYS_MIPI_VC_2,
	IPU_FW_ISYS_MIPI_VC_3,
	N_IPU_FW_ISYS_MIPI_VC
};

/**
 *  Supported Pixel Frame formats. Expandable if needed
 */
enum ipu_fw_isys_frame_format_type {
	IPU_FW_ISYS_FRAME_FORMAT_NV11 = 0, /* 12 bit YUV 411, Y, UV plane */
	IPU_FW_ISYS_FRAME_FORMAT_NV12,	/* 12 bit YUV 420, Y, UV plane */
	IPU_FW_ISYS_FRAME_FORMAT_NV12_16, /* 16 bit YUV 420, Y, UV plane */
	IPU_FW_ISYS_FRAME_FORMAT_NV12_TILEY, /* 12 bit YUV 420,
					      * Intel proprietary tiled format,
					      * TileY
					      */
	IPU_FW_ISYS_FRAME_FORMAT_NV16,	/* 16 bit YUV 422, Y, UV plane */
	IPU_FW_ISYS_FRAME_FORMAT_NV21,	/* 12 bit YUV 420, Y, VU plane */
	IPU_FW_ISYS_FRAME_FORMAT_NV61,	/* 16 bit YUV 422, Y, VU plane */
	IPU_FW_ISYS_FRAME_FORMAT_YV12,	/* 12 bit YUV 420, Y, V, U plane */
	IPU_FW_ISYS_FRAME_FORMAT_YV16,	/* 16 bit YUV 422, Y, V, U plane */
	IPU_FW_ISYS_FRAME_FORMAT_YUV420, /* 12 bit YUV 420, Y, U, V plane */
	IPU_FW_ISYS_FRAME_FORMAT_YUV420_10, /* yuv420, 10 bits per subpixel */
	IPU_FW_ISYS_FRAME_FORMAT_YUV420_12, /* yuv420, 12 bits per subpixel */
	IPU_FW_ISYS_FRAME_FORMAT_YUV420_14, /* yuv420, 14 bits per subpixel */
	IPU_FW_ISYS_FRAME_FORMAT_YUV420_16, /* yuv420, 16 bits per subpixel */
	IPU_FW_ISYS_FRAME_FORMAT_YUV422, /* 16 bit YUV 422, Y, U, V plane */
	IPU_FW_ISYS_FRAME_FORMAT_YUV422_16, /* yuv422, 16 bits per subpixel */
	IPU_FW_ISYS_FRAME_FORMAT_UYVY,	/* 16 bit YUV 422, UYVY interleaved */
	IPU_FW_ISYS_FRAME_FORMAT_YUYV,	/* 16 bit YUV 422, YUYV interleaved */
	IPU_FW_ISYS_FRAME_FORMAT_YUV444, /* 24 bit YUV 444, Y, U, V plane */
	IPU_FW_ISYS_FRAME_FORMAT_YUV_LINE, /* Internal format, 2 y lines
					    * followed by a uvinterleaved line
					    */
	IPU_FW_ISYS_FRAME_FORMAT_RAW8,	/* RAW8, 1 plane */
	IPU_FW_ISYS_FRAME_FORMAT_RAW10,	/* RAW10, 1 plane */
	IPU_FW_ISYS_FRAME_FORMAT_RAW12,	/* RAW12, 1 plane */
	IPU_FW_ISYS_FRAME_FORMAT_RAW14,	/* RAW14, 1 plane */
	IPU_FW_ISYS_FRAME_FORMAT_RAW16,	/* RAW16, 1 plane */
	IPU_FW_ISYS_FRAME_FORMAT_RGB565, /* 16 bit RGB, 1 plane. Each 3 sub
					  * pixels are packed into one 16 bit
					  * value, 5 bits for R, 6 bits
					  *   for G and 5 bits for B.
					  */

	IPU_FW_ISYS_FRAME_FORMAT_PLANAR_RGB888,	/* 24 bit RGB, 3 planes */
	IPU_FW_ISYS_FRAME_FORMAT_RGBA888,	/* 32 bit RGBA, 1 plane,
						 * A=Alpha (alpha is unused)
						 */
	IPU_FW_ISYS_FRAME_FORMAT_QPLANE6,	/* Internal, for advanced ISP */
	IPU_FW_ISYS_FRAME_FORMAT_BINARY_8, /* byte stream, used for jpeg. */
	N_IPU_FW_ISYS_FRAME_FORMAT
};

/* Temporary for driver compatibility */
#define IPU_FW_ISYS_FRAME_FORMAT_RAW		(IPU_FW_ISYS_FRAME_FORMAT_RAW16)

/**
 *  Supported MIPI data type. Keep in sync array in ipu_fw_isys_private.c
 */
enum ipu_fw_isys_mipi_data_type {
	/** SYNCHRONIZATION SHORT PACKET DATA TYPES */
	IPU_FW_ISYS_MIPI_DATA_TYPE_FRAME_START_CODE = 0x00,
	IPU_FW_ISYS_MIPI_DATA_TYPE_FRAME_END_CODE = 0x01,
	IPU_FW_ISYS_MIPI_DATA_TYPE_LINE_START_CODE = 0x02,	/* Optional */
	IPU_FW_ISYS_MIPI_DATA_TYPE_LINE_END_CODE = 0x03,	/* Optional */
	/** Reserved 0x04-0x07 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x04 = 0x04,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x05 = 0x05,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x06 = 0x06,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x07 = 0x07,
	/** GENERIC SHORT PACKET DATA TYPES */
	/** They are used to keep the timing information for
	 * the opening/closing of shutters,
	 *  triggering of flashes and etc.
	 */
	/* Generic Short Packet Codes 1 - 8 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT1 = 0x08,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT2 = 0x09,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT3 = 0x0A,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT4 = 0x0B,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT5 = 0x0C,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT6 = 0x0D,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT7 = 0x0E,
	IPU_FW_ISYS_MIPI_DATA_TYPE_GENERIC_SHORT8 = 0x0F,
	/** GENERIC LONG PACKET DATA TYPES */
	IPU_FW_ISYS_MIPI_DATA_TYPE_NULL = 0x10,
	IPU_FW_ISYS_MIPI_DATA_TYPE_BLANKING_DATA = 0x11,
	/* Embedded 8-bit non Image Data */
	IPU_FW_ISYS_MIPI_DATA_TYPE_EMBEDDED = 0x12,
	/** Reserved 0x13-0x17 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x13 = 0x13,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x14 = 0x14,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x15 = 0x15,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x16 = 0x16,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x17 = 0x17,
	/** YUV DATA TYPES */
	/* 8 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV420_8 = 0x18,
	/* 10 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV420_10 = 0x19,
	/* 8 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV420_8_LEGACY = 0x1A,
	/** Reserved 0x1B */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x1B = 0x1B,
	/* YUV420 8-bit Chroma Shifted Pixel Sampling) */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV420_8_SHIFT = 0x1C,
	/* YUV420 8-bit (Chroma Shifted Pixel Sampling) */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV420_10_SHIFT = 0x1D,
	/* UYVY..UVYV, 8 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV422_8 = 0x1E,
	/* UYVY..UVYV, 10 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_YUV422_10 = 0x1F,
	/** RGB DATA TYPES */
	/* BGR..BGR, 4 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RGB_444 = 0x20,
	/* BGR..BGR, 5 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RGB_555 = 0x21,
	/* BGR..BGR, 5 bits B and R, 6 bits G */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RGB_565 = 0x22,
	/* BGR..BGR, 6 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RGB_666 = 0x23,
	/* BGR..BGR, 8 bits per subpixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RGB_888 = 0x24,
	/** Reserved 0x25-0x27 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x25 = 0x25,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x26 = 0x26,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x27 = 0x27,
	/** RAW DATA TYPES */
	/* RAW data, 6 - 14 bits per pixel */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_6 = 0x28,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_7 = 0x29,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_8 = 0x2A,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_10 = 0x2B,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_12 = 0x2C,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_14 = 0x2D,
	/** Reserved 0x2E-2F are used with assigned meaning */
	/* RAW data, 16 bits per pixel, not specified in CSI-MIPI standard */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RAW_16 = 0x2E,
	/* Binary byte stream, which is target at JPEG,
	 * not specified in CSI-MIPI standard
	 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_BINARY_8 = 0x2F,

	/** USER DEFINED 8-BIT DATA TYPES */
	/** For example, the data transmitter (e.g. the SoC sensor)
	 * can keep the JPEG data as
	 *  the User Defined Data Type 4 and the MPEG data as the
	 *  User Defined Data Type 7.
	 */
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF1 = 0x30,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF2 = 0x31,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF3 = 0x32,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF4 = 0x33,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF5 = 0x34,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF6 = 0x35,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF7 = 0x36,
	IPU_FW_ISYS_MIPI_DATA_TYPE_USER_DEF8 = 0x37,
	/** Reserved 0x38-0x3F */
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x38 = 0x38,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x39 = 0x39,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3A = 0x3A,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3B = 0x3B,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3C = 0x3C,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3D = 0x3D,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3E = 0x3E,
	IPU_FW_ISYS_MIPI_DATA_TYPE_RESERVED_0x3F = 0x3F,

	/* Keep always last and max value */
	N_IPU_FW_ISYS_MIPI_DATA_TYPE = 0x40
};

/** enum ipu_fw_isys_pin_type: output pin buffer types.
 * Buffers can be queued and de-queued to hand them over between IA and ISYS
 */
enum ipu_fw_isys_pin_type {
	/* Captured as MIPI packets */
	IPU_FW_ISYS_PIN_TYPE_MIPI = 0,
	/* Captured through the ISApf (with/without ISA)
	 * and the non-scaled output path
	 */
	IPU_FW_ISYS_PIN_TYPE_RAW_NS,
	/* Captured through the ISApf + ISA and the scaled output path */
	IPU_FW_ISYS_PIN_TYPE_RAW_S,
	/* Captured through the SoC path */
	IPU_FW_ISYS_PIN_TYPE_RAW_SOC,
	/* Reserved for future use, maybe short packets */
	IPU_FW_ISYS_PIN_TYPE_METADATA_0,
	/* Reserved for future use */
	IPU_FW_ISYS_PIN_TYPE_METADATA_1,
	/* Legacy (non-PIV2), used for the AWB stats */
	IPU_FW_ISYS_PIN_TYPE_AWB_STATS,
	/* Legacy (non-PIV2), used for the AF stats */
	IPU_FW_ISYS_PIN_TYPE_AF_STATS,
	/* Legacy (non-PIV2), used for the AE stats */
	IPU_FW_ISYS_PIN_TYPE_HIST_STATS,
	/* Used for the PAF FF */
	IPU_FW_ISYS_PIN_TYPE_PAF_FF,
#if !defined(CONFIG_VIDEO_INTEL_IPU4) && !defined(CONFIG_VIDEO_INTEL_IPU4P)
	/* Captured through the SoC path
	 * (2D mode where odd and even lines are handled separately)
	 */
	IPU_FW_ISYS_PIN_TYPE_RAW_DUAL_SOC,
#endif
	/* Keep always last and max value */
	N_IPU_FW_ISYS_PIN_TYPE
};

/**
 * enum ipu_fw_isys_isl_use
 * Describes the ISL/ISA use
 */
enum ipu_fw_isys_isl_use {
	IPU_FW_ISYS_USE_NO_ISL_NO_ISA = 0,
	IPU_FW_ISYS_USE_SINGLE_DUAL_ISL,
	IPU_FW_ISYS_USE_SINGLE_ISA,
	N_IPU_FW_ISYS_USE
};

/**
 * enum ipu_fw_isys_mipi_store_mode. Describes if long MIPI packets reach
 * MIPI SRAM with the long packet header or
 * if not, then only option is to capture it with pin type MIPI.
 */
enum ipu_fw_isys_mipi_store_mode {
	IPU_FW_ISYS_MIPI_STORE_MODE_NORMAL = 0,
	IPU_FW_ISYS_MIPI_STORE_MODE_DISCARD_LONG_HEADER,
	N_IPU_FW_ISYS_MIPI_STORE_MODE
};

/**
 * enum ipu_fw_isys_type_paf. Describes the Type of PAF enabled
 */
enum ipu_fw_isys_type_paf {
	/* PAF data not present */
	IPU_FW_ISYS_TYPE_NO_PAF = 0,
	/* Type 2 sensor types, PAF coming separately from Image Frame  */
	/* PAF data in interleaved format(RLRL or LRLR) */
	IPU_FW_ISYS_TYPE_INTERLEAVED_PAF,
	/* PAF data in non-interleaved format(LL/RR or RR/LL) */
	IPU_FW_ISYS_TYPE_NON_INTERLEAVED_PAF,
	/* Type 3 sensor types , PAF data embedded in Image Frame */
	/* Frame Embedded PAF in interleaved format(RLRL or LRLR) */
	IPU_FW_ISYS_TYPE_FRAME_EMB_INTERLEAVED_PAF,
	/* Frame Embedded PAF non-interleaved format(LL/RR or RR/LL) */
	IPU_FW_ISYS_TYPE_FRAME_EMB_NON_INTERLEAVED_PAF,
	N_IPU_FW_ISYS_TYPE_PAF
};

/**
 * enum ipu_fw_isys_cropping_location. Enumerates the cropping locations in ISYS
 */
enum ipu_fw_isys_cropping_location {
	/* Cropping executed in ISAPF (mainly),
	 * ISAPF preproc (odd column) and MIPI STR2MMIO (odd row)
	 */
	IPU_FW_ISYS_CROPPING_LOCATION_PRE_ISA = 0,
	/* Reserved for legacy mode which will never be implemented */
	IPU_FW_ISYS_CROPPING_LOCATION_RESERVED_1,
	/* Cropping executed in StreamPifConv in the ISA output for
	 * RAW_NS pin
	 */
	IPU_FW_ISYS_CROPPING_LOCATION_POST_ISA_NONSCALED,
	/* Cropping executed in StreamScaledPifConv
	 * in the ISA output for RAW_S pin
	 */
	IPU_FW_ISYS_CROPPING_LOCATION_POST_ISA_SCALED,
	N_IPU_FW_ISYS_CROPPING_LOCATION
};

/**
 * enum ipu_fw_isys_resolution_info. Describes the resolution,
 * required to setup the various ISA GP registers.
 */
enum ipu_fw_isys_resolution_info {
	/* Scaled ISA output resolution before
	 * the StreamScaledPifConv cropping
	 */
	IPU_FW_ISYS_RESOLUTION_INFO_POST_ISA_NONSCALED = 0,
	/* Non-Scaled ISA output resolution before the StreamPifConv cropping */
	IPU_FW_ISYS_RESOLUTION_INFO_POST_ISA_SCALED,
	N_IPU_FW_ISYS_RESOLUTION_INFO
};

/**
 * enum ipu_fw_isys_error. Describes the error type detected by the FW
 */
enum ipu_fw_isys_error {
	IPU_FW_ISYS_ERROR_NONE = 0,	/* No details */
	IPU_FW_ISYS_ERROR_FW_INTERNAL_CONSISTENCY,	/* enum */
	IPU_FW_ISYS_ERROR_HW_CONSISTENCY,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_COMMAND_SEQUENCE,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_DEVICE_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_STREAM_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_FRAME_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_INSUFFICIENT_RESOURCES,	/* enum */
	IPU_FW_ISYS_ERROR_HW_REPORTED_STR2MMIO,	/* HW code */
	IPU_FW_ISYS_ERROR_HW_REPORTED_SIG2CIO,	/* HW code */
	IPU_FW_ISYS_ERROR_SENSOR_FW_SYNC,	/* enum */
	IPU_FW_ISYS_ERROR_STREAM_IN_SUSPENSION,	/* FW code */
	IPU_FW_ISYS_ERROR_RESPONSE_QUEUE_FULL,	/* FW code */
	N_IPU_FW_ISYS_ERROR
};

/**
 * enum ipu_fw_proxy_error. Describes the error type for
 * the proxy detected by the FW
 */
enum ipu_fw_proxy_error {
	IPU_FW_PROXY_ERROR_NONE = 0,
	IPU_FW_PROXY_ERROR_INVALID_WRITE_REGION,
	IPU_FW_PROXY_ERROR_INVALID_WRITE_OFFSET,
	N_IPU_FW_PROXY_ERROR
};

struct ipu_isys;

/**
 * struct ipu_fw_isys_buffer_partition_abi - buffer partition information
 * @num_gda_pages: Number of virtual gda pages available for each virtual stream
 */
struct ipu_fw_isys_buffer_partition_abi {
	u32 num_gda_pages[IPU_STREAM_ID_MAX];
};

/**
 * struct ipu_fw_isys_fw_config - contains the parts from
 * ia_css_isys_device_cfg_data we need to transfer to the cell
 */
struct ipu_fw_isys_fw_config {
	struct ipu_fw_isys_buffer_partition_abi buffer_partition;
	u32 num_send_queues[N_IPU_FW_ISYS_QUEUE_TYPE];
	u32 num_recv_queues[N_IPU_FW_ISYS_QUEUE_TYPE];
};

/**
 * struct ipu_fw_isys_resolution_abi: Generic resolution structure.
 * @Width
 * @Height
 */
struct ipu_fw_isys_resolution_abi {
	u32 width;
	u32 height;
};

/**
 * struct ipu_fw_isys_output_pin_payload_abi
 * @out_buf_id: Points to output pin buffer - buffer identifier
 * @addr: Points to output pin buffer - CSS Virtual Address
 * @compress: Request frame compression (1), or  not (0)
 */
struct ipu_fw_isys_output_pin_payload_abi {
	u64 out_buf_id;
	u32 addr;
	u32 compress;
};

/**
 * struct ipu_fw_isys_output_pin_info_abi
 * @output_res: output pin resolution
 * @stride: output stride in Bytes (not valid for statistics)
 * @watermark_in_lines: pin watermark level in lines
 * @payload_buf_size: minimum size in Bytes of all buffers that will be
 *			supplied for capture on this pin
 * @send_irq: assert if pin event should trigger irq
 * @pt: pin type -real format "enum ipu_fw_isys_pin_type"
 * @ft: frame format type -real format "enum ipu_fw_isys_frame_format_type"
 * @input_pin_id: related input pin id
 * @reserve_compression: reserve compression resources for pin
 */
struct ipu_fw_isys_output_pin_info_abi {
	struct ipu_fw_isys_resolution_abi output_res;
	u32 stride;
	u32 watermark_in_lines;
	u32 payload_buf_size;
	u8 send_irq;
	u8 input_pin_id;
	u8 pt;
	u8 ft;
	u8 reserved;
	u8 reserve_compression;
};

/**
 * struct ipu_fw_isys_param_pin_abi
 * @param_buf_id: Points to param port buffer - buffer identifier
 * @addr: Points to param pin buffer - CSS Virtual Address
 */
struct ipu_fw_isys_param_pin_abi {
	u64 param_buf_id;
	u32 addr;
};

/**
 * struct ipu_fw_isys_input_pin_info_abi
 * @input_res: input resolution
 * @dt: mipi data type ((enum ipu_fw_isys_mipi_data_type)
 * @mipi_store_mode: defines if legacy long packet header will be stored or
 *		     discarded if discarded, output pin pin type for this
 *		     input pin can only be MIPI
 *		     (enum ipu_fw_isys_mipi_store_mode)
 * @bits_per_pix: native bits per pixel
 * @mapped_dt: actual data type from sensor
#if !defined(CONFIG_VIDEO_INTEL_IPU4) && !defined(CONFIG_VIDEO_INTEL_IPU4P)
 * @crop_first_and_last_lines    Control whether to crop the
 *                              first and last line of the
 *                              input image. Crop done by HW
 *                              device.
#endif
 */
struct ipu_fw_isys_input_pin_info_abi {
	struct ipu_fw_isys_resolution_abi input_res;
	u8 dt;
	u8 mipi_store_mode;
	u8 bits_per_pix;
	u8 mapped_dt;
#if !defined(CONFIG_VIDEO_INTEL_IPU4) && !defined(CONFIG_VIDEO_INTEL_IPU4P)
	u8 crop_first_and_last_lines;
#endif
};

/**
 * struct ipu_fw_isys_isa_cfg_abi. Describes the ISA cfg
 */
struct ipu_fw_isys_isa_cfg_abi {
	struct ipu_fw_isys_resolution_abi
	 isa_res[N_IPU_FW_ISYS_RESOLUTION_INFO];
	struct {
		unsigned int blc:1;
		unsigned int lsc:1;
		unsigned int dpc:1;
		unsigned int downscaler:1;
		unsigned int awb:1;
		unsigned int af:1;
		unsigned int ae:1;
		unsigned int paf:8;
		unsigned int send_irq_stats_ready:1;
		unsigned int send_resp_stats_ready:1;
	} cfg;
};

/**
 * struct ipu_fw_isys_cropping_abi - cropping coordinates
 */
struct ipu_fw_isys_cropping_abi {
	s32 top_offset;
	s32 left_offset;
	s32 bottom_offset;
	s32 right_offset;
};

/**
 * struct ipu_fw_isys_stream_cfg_data_abi
 * ISYS stream configuration data structure
 * @isa_cfg: details about what ACCs are active if ISA is used
 * @crop: defines cropping resolution for the
 * maximum number of input pins which can be cropped,
 * it is directly mapped to the HW devices
 * @input_pins: input pin descriptors
 * @output_pins: output pin descriptors
 * @compfmt: de-compression setting for User Defined Data
 * @nof_input_pins: number of input pins
 * @nof_output_pins: number of output pins
 * @send_irq_sof_discarded: send irq on discarded frame sof response
 *		- if '1' it will override the send_resp_sof_discarded
 *		  and send the response
 *		- if '0' the send_resp_sof_discarded will determine
 *		  whether to send the response
 * @send_irq_eof_discarded: send irq on discarded frame eof response
 *		- if '1' it will override the send_resp_eof_discarded
 *		  and send the response
 *		- if '0' the send_resp_eof_discarded will determine
 *		  whether to send the response
 * @send_resp_sof_discarded: send response for discarded frame sof detected,
 *			     used only when send_irq_sof_discarded is '0'
 * @send_resp_eof_discarded: send response for discarded frame eof detected,
 *			     used only when send_irq_eof_discarded is '0'
 * @src: Stream source index e.g. MIPI_generator_0, CSI2-rx_1
 * @vc: MIPI Virtual Channel (up to 4 virtual per physical channel)
 * @isl_use: indicates whether stream requires ISL and how
 */
struct ipu_fw_isys_stream_cfg_data_abi {
	struct ipu_fw_isys_isa_cfg_abi isa_cfg;
	struct ipu_fw_isys_cropping_abi crop[N_IPU_FW_ISYS_CROPPING_LOCATION];
	struct ipu_fw_isys_input_pin_info_abi input_pins[IPU_MAX_IPINS];
	struct ipu_fw_isys_output_pin_info_abi output_pins[IPU_MAX_OPINS];
	u32 compfmt;
	u8 nof_input_pins;
	u8 nof_output_pins;
	u8 send_irq_sof_discarded;
	u8 send_irq_eof_discarded;
	u8 send_resp_sof_discarded;
	u8 send_resp_eof_discarded;
	u8 src;
	u8 vc;
	u8 isl_use;
};

/**
 * struct ipu_fw_isys_frame_buff_set - frame buffer set
 * @output_pins: output pin addresses
 * @process_group_light: process_group_light buffer address
 * @send_irq_sof: send irq on frame sof response
 *		- if '1' it will override the send_resp_sof and
 *		  send the response
 *		- if '0' the send_resp_sof will determine whether to
 *		  send the response
 * @send_irq_eof: send irq on frame eof response
 *		- if '1' it will override the send_resp_eof and
 *		  send the response
 *		- if '0' the send_resp_eof will determine whether to
 *		  send the response
 * @send_resp_sof: send response for frame sof detected,
 *		   used only when send_irq_sof is '0'
 * @send_resp_eof: send response for frame eof detected,
 *		   used only when send_irq_eof is '0'
 */
struct ipu_fw_isys_frame_buff_set_abi {
	struct ipu_fw_isys_output_pin_payload_abi output_pins[IPU_MAX_OPINS];
	struct ipu_fw_isys_param_pin_abi process_group_light;
	u8 send_irq_sof;
	u8 send_irq_eof;
#if defined(CONFIG_VIDEO_INTEL_IPU4) || defined(CONFIG_VIDEO_INTEL_IPU4P)
	u8 send_irq_capture_ack;
	u8 send_irq_capture_done;
#endif
	u8 send_resp_sof;
	u8 send_resp_eof;
	u8 reserved;
};

/**
 * struct ipu_fw_isys_error_info_abi
 * @error: error code if something went wrong
 * @error_details: depending on error code, it may contain additional error info
 */
struct ipu_fw_isys_error_info_abi {
	enum ipu_fw_isys_error error;
	u32 error_details;
};

/**
 * struct ipu_fw_isys_resp_info_comm
 * @pin: this var is only valid for pin event related responses,
 *     contains pin addresses
 * @process_group_light: this var is valid for stats ready related responses,
 *			contains process group addresses
 * @error_info: error information from the FW
 * @timestamp: Time information for event if available
 * @stream_handle: stream id the response corresponds to
 * @type: response type (enum ipu_fw_isys_resp_type)
 * @pin_id: pin id that the pin payload corresponds to
 * @acc_id: this var is valid for stats ready related responses,
 *	   contains accelerator id that finished producing
 *	   all related statistics
 */
struct ipu_fw_isys_resp_info_abi {
	u64 buf_id;
	struct ipu_fw_isys_output_pin_payload_abi pin;
	struct ipu_fw_isys_param_pin_abi process_group_light;
	struct ipu_fw_isys_error_info_abi error_info;
	u32 timestamp[2];
	u8 stream_handle;
	u8 type;
	u8 pin_id;
	u8 acc_id;
	u16 reserved;
};

/**
 * struct ipu_fw_isys_proxy_error_info_comm
 * @proxy_error: error code if something went wrong
 * @proxy_error_details: depending on error code, it may contain additional
 *			error info
 */
struct ipu_fw_isys_proxy_error_info_abi {
	enum ipu_fw_proxy_error error;
	u32 error_details;
};

struct ipu_fw_isys_proxy_resp_info_abi {
	u32 request_id;
	struct ipu_fw_isys_proxy_error_info_abi error_info;
};

/**
 * struct ipu_fw_proxy_write_queue_token
 * @request_id: update id for the specific proxy write request
 * @region_index: Region id for the proxy write request
 * @offset: Offset of the write request according to the base address
 *	    of the region
 * @value: Value that is requested to be written with the proxy write request
 */
struct ipu_fw_proxy_write_queue_token {
	u32 request_id;
	u32 region_index;
	u32 offset;
	u32 value;
};

/* From here on type defines not coming from the ISYSAPI interface */

/**
 * struct ipu_fw_resp_queue_token
 */
struct ipu_fw_resp_queue_token {
	struct ipu_fw_isys_resp_info_abi resp_info;
};

/**
 * struct ipu_fw_send_queue_token
 */
struct ipu_fw_send_queue_token {
	u64 buf_handle;
	u32 payload;
	u16 send_type;
	u16 stream_id;
};

/**
 * struct ipu_fw_proxy_resp_queue_token
 */
struct ipu_fw_proxy_resp_queue_token {
	struct ipu_fw_isys_proxy_resp_info_abi proxy_resp_info;
};

/**
 * struct ipu_fw_proxy_send_queue_token
 */
struct ipu_fw_proxy_send_queue_token {
	u32 request_id;
	u32 region_index;
	u32 offset;
	u32 value;
};

void ipu_fw_isys_set_params(struct ipu_fw_isys_stream_cfg_data_abi *stream_cfg);

void ipu_fw_isys_dump_stream_cfg(struct device *dev,
				 struct ipu_fw_isys_stream_cfg_data_abi
				 *stream_cfg);
void ipu_fw_isys_dump_frame_buff_set(struct device *dev,
				     struct ipu_fw_isys_frame_buff_set_abi *buf,
				     unsigned int outputs);
int ipu_fw_isys_init(struct ipu_isys *isys, unsigned int num_streams);
int ipu_fw_isys_close(struct ipu_isys *isys);
int ipu_fw_isys_simple_cmd(struct ipu_isys *isys,
			   const unsigned int stream_handle,
			   enum ipu_fw_isys_send_type send_type);
int ipu_fw_isys_complex_cmd(struct ipu_isys *isys,
			    const unsigned int stream_handle,
			    void *cpu_mapped_buf,
			    dma_addr_t dma_mapped_buf,
			    size_t size, enum ipu_fw_isys_send_type send_type);
int ipu_fw_isys_send_proxy_token(struct ipu_isys *isys,
				 unsigned int req_id,
				 unsigned int index,
				 unsigned int offset, u32 value);
void ipu_fw_isys_cleanup(struct ipu_isys *isys);
struct ipu_fw_isys_resp_info_abi *ipu_fw_isys_get_resp(void *context,
						       unsigned int queue,
						       struct
						       ipu_fw_isys_resp_info_abi
						       *response);
void ipu_fw_isys_put_resp(void *context, unsigned int queue);
#endif
