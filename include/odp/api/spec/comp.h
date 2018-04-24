/* Copyright (c) 2018, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

/**
 * @file
 *
 * ODP Compression
 */

#ifndef ODP_API_COMP_H_
#define ODP_API_COMP_H_

#include <odp/visibility_begin.h>
#include <odp/api/support.h>
#include <odp/api/packet.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup odp_compression ODP COMP
 *  Operations for Compression and Decompression API.
 *  Hash calculation may be combined with comp/decomp operations
 *
 *  @{
 */

/**
 * @def ODP_COMP_SESSION_INVALID
 * Invalid session handle
 */

/**
 * @typedef odp_comp_session_t
 * Compression/Decompression session handle
 */

/**
 * Compression API operation mode
 */
typedef enum {
	/** Synchronous operation
	 * Application uses synchronous operation,
	 * which outputs all results immediately */
	ODP_COMP_SYNC,

	/** Asynchronous mode
	 * Application uses asynchronous operation,
	 * which return results via event queue */
	ODP_COMP_ASYNC
} odp_comp_op_mode_t;

/**
 * Comp API operation type.
 *
 */
typedef enum {
	/** Compress */
	ODP_COMP_OP_COMPRESS,

	/** Decompress */
	ODP_COMP_OP_DECOMPRESS
} odp_comp_op_t;

/**
 * Comp API hash algorithm
 *
 */
typedef enum {
	/** No hash algorithm selected. */
	ODP_COMP_HASH_ALG_NONE,

	/** SHA-1 hash algorithm. */
	ODP_COMP_HASH_ALG_SHA1,

	/** SHA-2 hash algorithm 256-bit digest length. */
	ODP_COMP_HASH_ALG_SHA256
} odp_comp_hash_alg_t;

/**
 * Comp API compression algorithm
 *
 */
typedef enum {
	/** No algorithm specified. Added for testing purpose. */
	ODP_COMP_ALG_NULL,

	/** DEFLATE - RFC1951 */
	ODP_COMP_ALG_DEFLATE,

	/** ZLIB - RFC1950 */
	ODP_COMP_ALG_ZLIB,

	/** LZS */
	ODP_COMP_ALG_LZS
} odp_comp_alg_t;

/**
 * Comp API operation return codes
 *
 */
typedef enum {
	/** Operation completed successfully*/
	ODP_COMP_STATUS_SUCCESS,

	/** Operation terminated due to insufficient output buffer */
	ODP_COMP_STATUS_OUT_OF_SPACE_TERM,

	/** Compression status failure */
	ODP_COMP_STATUS_FAILURE,

	/**  Compression Algo failure */
	ODP_COMP_STATUS_ALGO_FAIL,

	/** Error if operation has been requested in an invalid state */
	ODP_COMP_STATUS_INV_STATE,
} odp_comp_status_t;

/**
 * Hash algorithms in a bit field structure
 *
 */
typedef union odp_comp_hash_algos_t {
	/** hash algorithms */
	struct {
		/** ODP_COMP_HASH_ALG_NONE */
		uint32_t none	: 1,

		/** ODP_COMP_HASH_ALG_SHA1 */
		uint32_t sha1	: 1;

		/** ODP_COMP_HASH_ALG_SHA256 */
		uint32_t sha256	: 1;

	} bit;

	/** All bits of the bit field structure
	 *
	 * This field can be used to set/clear all flags, or bitwise
	 * operations over the entire structure.
	 */
	uint32_t all_bits;
} odp_comp_hash_algos_t;

/**
 * Comp algorithms in a bit field structure
 *
 */
typedef union odp_comp_algos_t {
	/** Compression algorithms */
	struct {
		/** ODP_COMP_ALG_NULL */
		uint32_t null       : 1;

		/** ODP_COMP_ALG_DEFLATE */
		uint32_t deflate    : 1;

		/** ODP_COMP_ALG_ZLIB */
		uint32_t zlib       : 1;

		/** ODP_COMP_ALG_LZS */
		uint32_t lzs        : 1;
	} bit;

	/** All bits of the bit field structure
	 * This field can be used to set/clear all flags, or bitwise
	 * operations over the entire structure.
	 */
	uint32_t all_bits;
} odp_comp_algos_t;

/**
 * Compression Interface Capabilities
 *
 */
typedef struct odp_comp_capability_t {
	/** Maximum number of  sessions */
	uint32_t max_sessions;

	/** Supported compression algorithms */
	odp_comp_algos_t comp_algos;

	/** Supported hash algorithms. */
	odp_comp_hash_algos_t hash_algos;

	/** Support type for synchronous operation mode (ODP_COMP_SYNC).
	 *  User should set odp_comp_session_param_t:mode based on
	 *  support level as indicated by this param.
	 */
	odp_support_t sync;

	/** Support type for asynchronous operation mode (ODP_COMP_ASYNC).
	 *  User should set odp_comp_session_param_t:mode param based on
	 *  support level as indicated by this param.
	 */
	odp_support_t async;
} odp_comp_capability_t;

/**
 * Hash algorithm capabilities
 *
 */
typedef struct odp_comp_hash_alg_capability_t {
	/** Digest length in bytes */
	uint32_t digest_len;
} odp_comp_hash_alg_capability_t;

/**
 * Compression algorithm capabilities structure for each algorithm.
 *
 */
typedef struct odp_comp_alg_capability_t {
	/** Enumeration indicating algorithm support for dictionary load */
	odp_support_t support_dict;

	/** Optional Maximum length of dictionary supported
	 *  by implementation of an algorithm.
	 *
	 *  Invalid if support_dict == ODP_SUPPORT_NO.
	 *
	 *  Implementation use dictionary of length less than or equal to value
	 *  indicated by dict_len. if set to 0 and if support_dict ==
	 *  ODP_SUPPORT_YES, then implementation will use dictionary length
	 *  less than or equal to user input length in odp_comp_set_dict()
	 *  and update used dictionary length at output of the call.
	 *
	 */
	uint32_t dict_len;

	/** Maximum compression level supported by implementation of this
	 * algorithm. Indicates number of compression levels supported by
	 * implementation
	 */
	uint32_t max_level;

	/** Supported hash algorithms */
	odp_comp_hash_algos_t hash_algo;
} odp_comp_alg_capability_t;

typedef enum odp_comp_huffman_code {
	/** Fixed Huffman code */
	ODP_COMP_HUFFMAN_FIXED,

	/** Dynamic Huffman code */
	ODP_COMP_HUFFMAN_DYNAMIC,

	/** Default huffman code selected by implementation */
	ODP_COMP_HUFFMAN_DEFAULT,
} odp_comp_huffman_code_t;

/**
 * Comp API dictionary type
 * Consists of pointer to byte buffer. Length of dictionary
 * indicated by length parameter.
 */
typedef struct odp_comp_dict_t {
	/** pointer to byte array */
	uint8_t *buf;
	/** length of the dictionary */
	uint32_t len;
} odp_comp_dict_t;

/**
 * Struct for defining deflate algorithm parameters.
 * Also initialized by other deflate based algorithms , ex. ZLIB
 */
typedef struct odp_comp_alg_def_param {
	/**
	 * Compression level
	 * Valid range is integer between 0 and max compression
	 * level supported by the implementation. where,
 	 * 1 - fastest compression i.e. output produced at
 	 * best possible speed at the expense of compression quality.
	 * max_value - High quality compression.
	 * 0 - implemention default
	 */
	uint32_t comp_level;

	/** huffman code to use */
	odp_comp_huffman_code_t comp_code;
} odp_comp_alg_def_param_t;

/**
 * Comp API algorithm specific parameters
 *
 */
typedef union odp_comp_alg_param_t {

	/** deflate parameter */
	odp_comp_alg_def_param_t deflate;

	/** Struct for defining zlib algorithm parameters.*/
	struct comp_alg_zlib_param {
		/** deflate algo params */
		odp_comp_alg_def_param_t def;
	} zlib; /** zlib parameter */
} odp_comp_alg_param_t;

 /**
 * Comp API session creation parameters
 *
 */
typedef struct odp_comp_session_param_t {
	/** Compress vs. Decompress operation */
	odp_comp_op_t op;

	/** Operation mode
	 *
	 * Application selects which mode will be used for
	 * compression/decompression operation.
	 *
	 * @see odp_comp_op(), odp_comp_op_enq()
	 */
	odp_comp_op_mode_t mode;

	/** Compression algorithm
	 *
	 *  @see odp_comp_capability()
	 */
	odp_comp_alg_t comp_algo;

	/** Hash algorithm
	 *
	 *  @see odp_comp_alg_capability()
	 */
	odp_comp_hash_alg_t hash_algo;

	/** parameters specific to compression */
	odp_comp_alg_param_t algo_param;

	/** Session packet enqueue ordering
	 * Boolean to indicate if packet enqueue ordering is required per session
	 * If packet_order is false, packet order is maintained on
	 * operation completion order and not in session enqueue order.
	 * By disabling packet order requirement, performance oriented
	 * application can leverage HW offered parallelism to increase operation
	 * performance.
	 */
	odp_bool_t packet_order;

	/** Destination queue for de/compression completion result
	 *
	 * Operations in asynchronous mode enqueue resulting events into this
	 * queue. Results are enqueued as ODP_EVENT_PACKT with subtype
	 * ODP_EVENT_PACKET_COMP
	 */
	odp_queue_t compl_queue;
} odp_comp_session_param_t;

/**
 * Comp packet operation result
 */
typedef struct odp_comp_packet_result_t {
	/** Operation Return Code */
	odp_comp_status_t status;

	/** Output packet data range */
	odp_packet_data_range_t out_data_range;
} odp_comp_packet_result_t;

/**
 * Comp API per packet operation parameters
 */
typedef struct odp_comp_packet_op_param_t {
	/** Session handle from creation */
	odp_comp_session_t session;

	/** Input data range to process */
	odp_packet_data_range_t in_data_range;

	/** Output packet data range.
	 * Indicates where processed packet will be written
	 * where length specifies, data available for output.
	 */
	odp_packet_data_range_t out_data_range;
} odp_comp_packet_op_param_t;

/**
 * Query comp capabilities
 *
 * Output comp capabilities on success.
 *
 * @param[out] capa   Pointer to capability structure for output
 *
 * @retval 0 on success
 * @retval <0 on failure
 */
int odp_comp_capability(odp_comp_capability_t *capa);

/**
 * Query supported compression algorithm capabilities
 *
 * Output algorithm capabilities.
 *
 * @param	comp	Compression algorithm
 * @param[out]	capa	Compression algorithm capability
 *
 * @retval <0 on failure
 */
int odp_comp_alg_capability(odp_comp_alg_t comp,
			    odp_comp_alg_capability_t capa);

/**
 * Query supported hash algorithm capabilities
 *
 * Outputs all supported configuration options for the algorithm.
 *
 * @param	hash     Hash algorithm
 * @param	capa     Hash algorithm capability
 *
 * @retval <0 on failure
 */
int odp_comp_hash_alg_capability(odp_comp_hash_alg_t hash,
				 odp_comp_hash_alg_capability_t capa);

/**
 * Initialize comp session parameters
 *
 * Initialize an odp_comp_session_param_t to its default values for
 * all fields.
 *
 * @param param   Pointer to odp_comp_session_param_t to be initialized
 */
void odp_comp_session_param_init(odp_comp_session_param_t *param);

/**
 * Comp session creation
 *
 * Create a comp session according to the session parameters. Use
 * odp_comp_session_param_init() to initialize parameters into their
 * default values.
 *
 * @param	param	Session parameters
 *
 * @retval		Comp session handle
 * @retval		ODP_COMP_SESSION_INVALID on failure
 */
odp_comp_session_t odp_comp_session_create(const odp_comp_session_param_t *
					   param);

/**
 * Comp session destroy
 *
 * Destroy an unused session. Result is undefined if session is being used
 * (i.e. asynchronous operation is in progress).
 *
 * @param session           Session handle
 *
 * @retval 0 on success
 * @retval <0 on failure
 */
int odp_comp_session_destroy(odp_comp_session_t session);

/**
 * Comp set dictionary
 *
 * Should be called when there is no operation in progress i.e.
 * before initiating processing of first chunk of data and
 * after processing of last chunk of data is complete.
 *
 * @param		session		Session handle
 * @param[in, out]	dict		Pointer to dictionary.
 *					Implementation should update
 *					length of dictionary at output.
 * @retval 0 on success
 * @retval <0 on failure
 *
 * @note:
 * Application should call odp_comp_alg_capability() to query 'support_dict'
 * before making call to this API.
 */
int odp_comp_set_dict(odp_comp_session_t session,
		      odp_comp_dict_t *dict);

/**
 * Synchronous packet comp
 *
 * This operation does packet compression/decompression in synchronous mode.
 * A successful processed entry means input packet has been completely consumed
 * and whole output has been written into output packet and corresponding
 * result entry is updated with offset:length information and error code set to
 * ODP_COMP_ERR_NONE.
 *
 * For every input packet entry in in_pkt array, user should pass
 * corresponding valid output packet handle. If during processing of packet,
 * corresponding buffer goes out of space, then API returns with number of
 * entries successfully processed.
 *
 * User should compute processed data len = output_data_range.len - digest_len,
 * where digest_len queried through odp_comp_hash_alg_capability().
 *
 * For compression + hash, call returns with hash appended at the end of
 * packet.
 *
 *
 * @param         pkt_in   Input Packets
 * @param         pkt_out  Output Packet to store output of corresponding entry
 * @param         param    Operation parameters array
 * @param[out]    result   Result array corresponding to each entry
 * @param         num_pkt  Number of packets to be processed
 *
 * @return Number of input packets consumed (0 ... num_pkt)
 * @retval <0 on failure
 */
int odp_comp_op(const odp_packet_t pkt_in[], int num,
		odp_packet_t pkt_out[],
		const odp_comp_packet_op_param_t param[]);

/**
 * Asynchronous packet comp
 *
 * This operation does packet compression/decompression in asynchronous mode.
 * A successful submitted entry means it has been queued for processing but
 * not fully processed. Each submitted entry results will be notified via
 * event after operation on that entry is complete. Application should call
 * odp_comp_result() to retrieve operational result.
 * For every input packet entry in in_pkt array, user should pass
 * corresponding valid output packet handle.On return, API returns with
 * number of entries successfully submitted for operation.
 * In case of partially accepted array i.e. when
 * number of packets returned < num_pkt, application my attempt to resubmit
 * subsequent entries via calling any of the operation API.
 *
 * User should compute processed data len = output_data_range.len - digest_len,
 * where digest_len queried through odp_comp_hash_alg_capability().
 * Application should monitor ODP_EVENT_PACKET
 * be queued with subtype ODP_EVENT_PACKET_COMP on queue.
 *
 * Please note it is always recommended that application using async mode,
 * provide sufficiently large buffer size to avoid
 * ODP_COMP_ERR_OUT_OF_SPACE_TERM.
 *
 * @param	pkt_in		Input Packet array
 * @param	pkt_out		Output Packet array
 * @param	param		Operation parameters array
 * @param	num_pkt		Number of packets to be processed
 *
 * @return	Number of input packets enqueued (0 ... num_pkt)
 * @retval	<0 on failure
 */
int odp_comp_op_enq(const odp_packet_t pkt_in[], int num,
		    odp_packet_t pkt_out[],
		    const odp_comp_packet_op_param_t param[]);

/**
 * Get Comp operation results from processed packet.
 *
 * Successful comp operation on async type produce packet which contain
 * comp result metadata. This function copies operation results from an Comp
 * processed packet. Event subtype of this kind of packet is
 * ODP_EVENT_PACKET_COMP. Results are undefined if non-comp processed packet is
 * passed as input.
 *
 * @param[out]    result  Pointer to operation result for output
 * @param	  packet  Comp processed packet (ODP_EVENT_PACKET_COMP)
 *
 * @retval  0     On success
 * @retval <0     On failure
 */
int odp_comp_result(odp_comp_packet_result_t *result, odp_packet_t packet);

 /**
  * Convert processed packet event to packet handle
  *
  * Get packet handle corresponding to processed packet event. Event subtype
  * must be ODP_EVENT_PACKET_COMP. compression/decompression operation
  * results can be examined with odp_comp_result().
  *
  * @param	event	Event handle
  *
  * @return	Valid Packet handle on success,
  * @retval	ODP_PACKET_INVALID on failure
  *
  * @see odp_event_subtype(), odp_comp_result()
  *
  */
odp_packet_t odp_comp_packet_from_event(odp_event_t event);

 /**
  * Convert processed packet handle to event
  *
  * The packet handle must be an output of an compression/decompression
  * operation.
  *
  * @return Event handle
  */
odp_event_t odp_comp_packet_to_event(odp_packet_t pkt);

/**
 * Get printable value for an odp_comp_session_t
 *
 * @param hdl  odp_comp_session_t handle to be printed
 * @return     uint64_t value that can be used to print/display this
 *             handle
 *
 * @note This routine is intended to be used for diagnostic purposes
 * to enable applications to generate a printable value that represents
 * an odp_comp_session_t handle.
 */
uint64_t odp_comp_session_to_u64(odp_comp_session_t hdl);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#include <odp/visibility_end.h>
#endif

