#include "json_queue.h"

void save_queue(const std::queue<std::string>& q, const std::string& filename) {
    std::queue<std::string> temp = q;
    nlohmann::json j;
    j["queue"] = nlohmann::json::array();

    while (!temp.empty()) {
        j["queue"].push_back(temp.front());
        temp.pop();
    }

    j["offset"] = get_offset(filename);
    j["discord-bot-token"] = get_token(filename);
    j["default"] = get_default(filename);
    j["qoft-channel-id"] = static_cast<uint64_t>(get_channel(filename));

    std::ofstream file(filename);
    file << j.dump(4);
}

std::queue<std::string> load_queue(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    std::queue<std::string> q;

    std::string fieldName = "queue";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_array()) {
            for (const auto& item : j[fieldName]) {
                q.push(item.get<std::string>());
            }
        }
    }

    return q;
}

uint16_t get_offset(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string fieldName = "offset";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_number_unsigned()) {
            return static_cast<uint16_t>(j[fieldName]);
        }
    }

    return 0;
}

void increment_offset(const std::string& filename) {
    std::ifstream input_file(filename);
    nlohmann::json j;

    std::string fieldName = "offset";

    if (!(input_file >> j)) {
        j["queue"] = nlohmann::json::array();
        j[fieldName] = 0;
    }
    input_file.close();

    uint16_t offset = 0;
    if (j.contains(fieldName) && j[fieldName].is_number_unsigned()) {
        offset = static_cast<uint16_t>(j[fieldName]);
    }

    offset = static_cast<uint16_t>((offset + 1) % 65536); // ~ 170 years from 2025, by then hardware should be good enough for at least 32 bit

    j[fieldName] = offset;
    j["discord-bot-token"] = get_token(filename);
    j["default"] = get_default(filename);
    j["qoft-channel-id"] = static_cast<uint64_t>(get_channel(filename));

    std::ofstream output_file(filename);
    output_file << j.dump(4);
}

std::string get_token(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string fieldName = "discord-bot-token";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_string()) {
            return j[fieldName];
        }
    }

    return "";
}

dpp::snowflake get_channel(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string fieldName = "qoft-channel-id";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_number_integer()) {
            return static_cast<dpp::snowflake>(j[fieldName].get<uint64_t>());
        }
    }

    return 0;
}

std::string get_default(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string fieldName = "default";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_string()) {
            return j[fieldName];
        }
    }

    return "";
}
