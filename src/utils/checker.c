#include "../../include/utils/checker.h"
#include "../../include/utils/logger.h"

#include <stdlib.h>

const char *error_descriptions[] = {
        "NULL pointer in arguments",
        "NULL pointer was returned"
};

bool check_is_null_arg(const void *arg, const char *name) {
    if (arg == NULL) {
        LOG_ERROR("%s. Argument: %s", error_descriptions[NULL_POINTER_IN_ARGS], name);
        return true;
    }
    return false;
}

bool check_is_null_returned(const void *arg, const char *method_name) {
    if (arg == NULL) {
        LOG_DEBUG("%s. Argument returned from %s", error_descriptions[NULL_POINTER_RETURNED], method_name);
        return true;
    }
    return false;
}
