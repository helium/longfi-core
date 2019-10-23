#ifndef LFC_DG_SER_D145CE89
#define LFC_DG_SER_D145CE89
#include "cursor/cursor.h"
#include "lfc/datagram.h"
#include "lfc/lfc.h"
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
 * Serializes a `lfc_dg_monolithic` datagram.
 */
enum lfc_res
lfc_dg_monolithic__ser(struct lfc_dg_monolithic const * in, struct cursor * csr);


/**
 * Serializes a `lfc_dg_frame_start` datagram.
 */
enum lfc_res
lfc_dg_frame_start__ser(struct lfc_dg_frame_start const * in,
                        struct cursor *                   csr);


/**
 * Serializes a `lfc_dg_frame_data` datagram.
 */
enum lfc_res
lfc_dg_frame_data__ser(struct lfc_dg_frame_data const * in, struct cursor * csr);


/**
 * Serializes a `lfc_dg_ack` datagram.
 */
enum lfc_res
lfc_dg_ack__ser(struct lfc_dg_ack const * in, struct cursor * csr);

/**
 * Serializes monolithic flags into the lower 6 bits of a byte.
 */
uint8_t
lfc_dg_monolithic_flags__ser(struct lfc_dg_monolithic_flags const * flags);


/**
 * Serializes frame start flags into the lower 6 bits of a byte.
 */
uint8_t
lfc_dg_frame_start_flags__ser(struct lfc_dg_frame_start_flags const * flags);


/**
 * Serializes frame data flags into the lower 6 bits of a byte.
 */
uint8_t
lfc_dg_frame_data_flags__ser(struct lfc_dg_frame_data_flags const * flags);


/**
 * Serializes ack flags into the lower 6 bits of a byte.
 */
uint8_t
lfc_dg_ack_flags__ser(struct lfc_dg_ack_flags const * flags);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* LFC_DG_SER_D145CE89 */
