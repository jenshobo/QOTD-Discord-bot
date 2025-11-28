#ifndef UPDATE_MAIN_H
#define UPDATE_MAIN_H

#include <queue>
#include "common.h"

// 1 hour delay, should always be under 24 hours
#define SHUTDOWN_DELAY = (60 * 60)

std::string get_question(void);

#endif