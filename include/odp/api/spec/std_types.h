/* Copyright (c) 2013, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */


/**
 * @file
 *
 * Standard C language types and definitions for ODP.
 *
 */

#ifndef ODP_API_STD_TYPES_H_
#define ODP_API_STD_TYPES_H_
#include <odp/visibility_begin.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup odp_system ODP SYSTEM
 *  @{
 */

/**
 * @typedef odp_bool_t
 * Use odp boolean type to have it well-defined and known size,
 * regardless which compiler is used as this facilities interoperability
 * between e.g. different compilers.
 */

/**
 * @typedef odp_percent_t
 * Use odp_percent_t for specifying fields that are percentages. It is a fixed
 * point integer whose units are expressed as one-hundredth of a percent.
 * Hence 100% is represented as integer value 10000.
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#include <odp/visibility_end.h>
#endif
