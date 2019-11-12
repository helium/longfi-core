#ifndef FINGERPRINT_7B3AE236
#define FINGERPRINT_7B3AE236

/**
 * @file
 *
 * API for generating and verification of LongFi fingerprints.
 *
 * @addtogroup Fingerprinting
 * @{
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * Computes a payload hash of sizes up to 32 bytes.
 *
 * @param[in]     pay     Payload.
 * @param         pay_len Length of payload in bytes.
 * @param[out]    out     Pointer to resultant hash buffer.
 * @param         out_len Desired length of hash in bytes.
 *
 * @pre           `out_len <= 32`.
 */
enum lfc_res
lfc_payload_hash(uint8_t const * pay, size_t pay_len, uint8_t * out, size_t out_len);


/**
 * Calculates the fingerprint for a monolithic datagram.
 *
 * @param[in]     key     Private cryptographic key.
 * @param         key_len Length of `key` in bytes.
 * @param         hdr     Packed, golay-decoded, 12 bits of tag and flags.
 * @param         oui     Orginizationally Unique Identifier.
 * @param         did     Device Identifier.
 * @param         seq     Sequence number.
 * @param[in]     pay     Payload bytes.
 * @param         pay_len Payload length in bytes.
 * @param[out]    out     Pointer to resultant 32 bit fingerprint.
 *
 * @pre           `(hdr & 0b1111_0000_0000_0000) == 0`
 */
enum lfc_res
lfc_fingerprint_monolithic(uint8_t const * key,
                           size_t          key_len,
                           uint16_t        hdr,
                           uint32_t        oui,
                           uint32_t        did,
                           uint32_t        seq,
                           uint8_t const * pay,
                           size_t          pay_len,
                           uint32_t *      out);


/**
 * Verifies a datagram with a hash of its payload instead of the
 * actual payload.
 *
 * @param         fp_decl      Fingerprint we want to verify.
 * @param[in]     key          Private cryptographic key.
 * @param         key_len      Length of `key` in bytes.
 * @param[in]     pay_hash     Hash of payload, *NOT* the payload itself.
 * @param[in]     pay_hash_len Hash of payload, *NOT* the payload itself.
 * @param         hdr          Packed, golay-decoded, 12 bits of tag and flags.
 * @param         oui          Orginizationally Unique Identifier.
 * @param         did          Device Identifier.
 * @param         seq          Sequence number.
 *
 * @pre           `(hdr & 0b1111_0000_0000_0000) == 0`
 */
enum lfc_res
lfc_fingerprint_monolithic_verify(uint32_t        fp_decl,
                                  uint8_t const * key,
                                  size_t          key_len,
                                  uint8_t const * pay_hash,
                                  size_t          pay_hash_len,
                                  uint16_t        hdr,
                                  uint32_t        oui,
                                  uint32_t        did,
                                  uint32_t        seq);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FINGERPRINT_7B3AE236 */
