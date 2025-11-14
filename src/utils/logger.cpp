#include "utils/logger.h"

void Logger::logExit(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}