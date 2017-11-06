/* Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

/**
 * @file
 *
 * ODP threshold descriptor
 */

#ifndef ODP_API_THRESHOLD_H_
#define ODP_API_THRESHOLD_H_
#include <odp/visibility_begin.h>
#include <odp/api/std_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Supported threshold types
 *
 * Supported threshold types in a bit field structure.
 */
typedef union odp_threshold_type_t {
	/** bitfields for different threshold types */
	struct {
		/** Percentage of the total size of pool or queue */
		uint8_t percent:1;

		/** Total number of all transient packets */
		uint8_t pkt_cnt:1;

		/** Total size of all transient packets in bytes */
		uint8_t pkt_size:1;
	};

	/** All bits of the bit field structure */
	uint8_t all_bits;
} odp_threshold_type_t;

/**
 * ODP Threshold types
 *
 * Different types of threshold measurements
 */
typedef	enum {
	/** Percentage of the total size of pool or queue */
	odp_percent_e,

	/** Total number of all transient packets */
	odp_pkt_count_e,

	/** Total size of all transient packets in bytes */
	odp_pkt_size_e
} odp_threshold_type_e;

/**
 * ODP Threshold
 *
 * Threshold configuration
 */
typedef struct odp_threshold_t {
	/** Type of threshold */
	odp_threshold_type_e type;

	/** Different threshold types */
	union {
		/** Percentage */
		struct {
			/** Max percentage value */
			odp_percent_t max;

			/** Min percentage value */
			odp_percent_t min;
		} percent;

		/** Packet count */
		struct {
			/** Max packet count */
			uint64_t max;

			/** Min packet count */
			uint64_t min;
		} pkt_count;

		/** Packet size */
		struct {
			/** Max size of all packets */
			uint64_t max;

			/** Min size of all packets */
			uint64_t min;
		} pkt_size;
	};
} odp_threshold_t;

#ifdef __cplusplus
}
#endif

#include <odp/visibility_end.h>
#endif
