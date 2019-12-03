#include <lfc/lfc.h>

int
main(int argc, char * argv[]) {
    (void)argc;
    (void)argv;
    struct lfc_version v = lfc_version();
    if (v.major == 0 && v.minor == 2 && v.patch == 0) {
        return 0;
    }
    return 1;
}
