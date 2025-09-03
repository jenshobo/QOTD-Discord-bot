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

    std::ofstream file(filename);
    file << j.dump(4);
}

std::queue<std::string> load_queue(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    std::queue<std::string> q;

    if (file >> j) {
        if (j.contains("queue") && j["queue"].is_array()) {
            for (const auto& item : j["queue"]) {
                q.push(item.get<std::string>());
            }
        }
    }

    return q;
}

uint16_t get_offset(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    if (file >> j) {
        if (j.contains("offset") && j["offset"].is_number_unsigned()) {
            return static_cast<uint16_t>(j["offset"]);
        }
    }

    return 0;
}

void increment_offset(const std::string& filename) {
    std::ifstream input_file(filename);
    nlohmann::json j;

    if (!(input_file >> j)) {
        j["queue"] = nlohmann::json::array();
        j["offset"] = 0;
    }
    input_file.close();

    uint16_t offset = 0;
    if (j.contains("offset") && j["offset"].is_number_unsigned()) {
        offset = static_cast<uint16_t>(j["offset"]);
    }

    offset = static_cast<uint16_t>((offset + 1) % 65536); // ~ 170 years from 2025, by then hardware should be good enough for at least 32 bit

    j["offset"] = offset;

    std::ofstream output_file(filename);
    output_file << j.dump(4);
}
