#ifndef MANAGER_MAIN_H
#define MANAGER_MAIN_H

#include <sstream>
#include "common.h"

void add_to_queue(std::string& question);
void add_to_queue_priority(std::string& question);
void remove_from_queue(uint16_t index);

std::stringstream queue_tostring(uint16_t& offset, std::queue<std::string>& queue);

#endif