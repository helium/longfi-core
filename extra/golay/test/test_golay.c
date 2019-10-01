#include <golay/golay.h>
#include <string.h>

int
main(int argc, char * argv[]) {
    uint8_t const REF[6] = {1, 2, 3, 4, 5, 6};
    uint8_t       encoded[12];
    uint8_t       decoded[6];

    golay_encode(sizeof(REF), REF, encoded);
    golay_decode(sizeof(encoded), encoded, decoded);

    return memcmp(REF, decoded, sizeof(decoded));
}
