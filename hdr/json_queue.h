#ifndef JSON_QUEUE_H
#define JSON_QUEUE_H

#include <iostream>
#include <fstream>
#include <queue>

#include <json.hpp>

void save_queue(const std::queue<std::string>& q, const std::string& filename);
std::queue<std::string> load_queue(const std::string& filename);
uint16_t get_offset(const std::string& filename);
void increment_offset(const std::string& filename);

#endif