#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <fstream>
#include <queue>

#include <json.hpp>
#include <dpp/dpp.h>

void save_queue(const std::queue<std::string>& q, const std::string& filename);
std::queue<std::string> load_queue(const std::string& filename);

uint16_t get_offset(const std::string& filename);
void increment_offset(const std::string& filename);

std::string get_token(const std::string& filename);
dpp::snowflake get_channel(const std::string& filename);
std::string get_default(const std::string& filename);

#endif