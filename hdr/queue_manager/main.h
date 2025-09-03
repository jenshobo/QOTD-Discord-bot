#ifndef MANAGER_MAIN_H
#define MANAGER_MAIN_H

#include <iostream>
#include <sstream>

#include "json_queue.h"
#include "credentials.h"

void add_to_queue(std::string& question);
void add_to_queue_priority(std::string& question);
void remove_from_queue(uint16_t index);

#endif