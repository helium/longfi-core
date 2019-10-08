#ifndef LFC_DG_EQ_4FDCDF78
#define LFC_DG_EQ_4FDCDF78

#include "lfc/datagram.h"
#include <stdbool.h>

/**
 * @file
 *
 * Private API for testing datagram equality..
 *
 * @addtogroup Private
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

bool
lfc_dg_monolithic__eq(struct lfc_dg_monolithic const * l,
                      struct lfc_dg_monolithic const * r);

bool
lfc_dg_frame_start__eq(struct lfc_dg_frame_start const * l,
                       struct lfc_dg_frame_start const * r);

bool
lfc_dg_frame_data__eq(struct lfc_dg_frame_data const * l,
                      struct lfc_dg_frame_data const * r);

bool
lfc_dg_ack__eq(struct lfc_dg_ack const * l, struct lfc_dg_ack const * r);

#ifdef __cplusplus
}
#endif

/** @} */


#endif /* LFC_DG_EQ_4FDCDF78 */
