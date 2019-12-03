#include "lfc_version.h"
#include "lfc/lfc.h"

struct lfc_version
lfc_version(void) {
    return (struct lfc_version){
        .major = PROJECT_VERSION_MAJOR,
        .minor = PROJECT_VERSION_MINOR,
        .patch = PROJECT_VERSION_PATCH,
    };
}
