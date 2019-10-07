#ifndef LFE_DG_SER_A9AEF0D1
#define LFE_DG_SER_A9AEF0D1

#include "cursor/cursor.h"
#include "lfe/datagram.h"
#include "lfe/lfe.h"
#include <stdbool.h>

/**
 * @file
 *
 * Private API for serializing datagrams.
 *
 * @addtogroup Private
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returned by serializer to represent success or failure.
 */
enum lfe_dg_ser_res {
    /** Operation successful. */
    lfe_dg_ser_res_ok,
    /** Reached end of provided data before complete serialization. */
    lfe_dg_ser_res_err_nomem,
};


/**
 * Serializes a `lfe_dg_monolithic` datagram.
 */
enum lfe_dg_ser_res
lfe_dg_monolithic__ser(struct lfe_dg_monolithic const * in, struct cursor * csr);


/**
 * Serializes a `lfe_dg_frame_start` datagram.
 */
enum lfe_dg_ser_res
lfe_dg_frame_start__ser(struct lfe_dg_frame_start const * in,
                        struct cursor *                   csr);


/**
 * Serializes a `lfe_dg_frame_data` datagram.
 */
enum lfe_dg_ser_res
lfe_dg_frame_data__ser(struct lfe_dg_frame_data const * in, struct cursor * csr);


/**
 * Serializes a `lfe_dg_ack` datagram.
 */
enum lfe_dg_ser_res
lfe_dg_ack__ser(struct lfe_dg_ack const * in, struct cursor * csr);

/**
 * Serializes monolithic flags into the lower 6 bits of a byte.
 */
uint8_t
lfe_dg_monolithic_flags__ser(struct lfe_dg_monolithic_flags const * flags);


/**
 * Serializes frame start flags into the lower 6 bits of a byte.
 */
uint8_t
lfe_dg_frame_start_flags__ser(struct lfe_dg_frame_start_flags const * flags);


/**
 * Serializes frame data flags into the lower 6 bits of a byte.
 */
uint8_t
lfe_dg_frame_data_flags__ser(struct lfe_dg_frame_data_flags const * flags);


/**
 * Serializes ack flags into the lower 6 bits of a byte.
 */
uint8_t
lfe_dg_ack_flags__ser(struct lfe_dg_ack_flags const * flags);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LFE_DG_SER_A9AEF0D1 */
