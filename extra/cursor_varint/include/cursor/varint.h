#ifndef VARINT_C73FB845
#define VARINT_C73FB845

#include <cursor/cursor.h>

#ifdef __cplusplus
extern "C" {
#endif

enum cursor_res
cursor_unpack_var_le_u32(struct cursor * csr, uint32_t * dst);

enum cursor_res
cursor_pack_var_le_u32(struct cursor * csr, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif /* VARINT_C73FB845 */
