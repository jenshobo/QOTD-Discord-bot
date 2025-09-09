#include "json.h"

void write_queue(
    const std::string& filename,
    std::optional<std::string> default_val = std::nullopt,
    std::optional<std::string> bot_token = std::nullopt,
    std::optional<uint16_t> offset = std::nullopt,
    std::optional<dpp::snowflake> channel_id = std::nullopt,
    std::optional<std::queue<std::string>> queue = std::nullopt
) {
    std::string default_field = default_val.value_or(get_default(filename));
    std::string token_field = bot_token.value_or(get_token(filename));
    uint16_t offset_field = offset.value_or(get_offset(filename));
    dpp::snowflake channel_field = channel_id.value_or(get_channel(filename));
    std::queue<std::string> queue_field = queue.value_or(load_queue(filename));

    std::vector<std::string> queue_vec;
    std::queue<std::string> temp_queue = queue_field;
    while (!temp_queue.empty()) {
        queue_vec.push_back(temp_queue.front());
        temp_queue.pop();
    }

    nlohmann::json j;
    j["default"] = default_field;
    j["discord-bot-token"] = token_field;
    j["offset"] = offset_field;
    j["qotd-channel-id"] = static_cast<uint64_t>(channel_field);
    j["queue"] = queue_vec;

    std::ofstream file(filename);
    file <<j.dump(4);
}

void save_queue(const std::queue<std::string>& queue, const std::string& filename) {
    write_queue(filename, std::nullopt, std::nullopt, std::nullopt, std::nullopt, queue);
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
    std::ifstream file(filename);
    nlohmann::json j;

    uint16_t offset = 0;
    std::string fieldName = "offset";
    if (file >> j) {
        if (j.contains(fieldName) && j[fieldName].is_number_unsigned()) {
            offset = static_cast<uint16_t>(j[fieldName]);
        }
    }

    offset = static_cast<uint16_t>((offset + 1) % 65536); // ~ 170 years from 2025, by then hardware should be good enough for at least 32 bit

    write_queue(filename, std::nullopt, std::nullopt, offset);
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

    std::string fieldName = "qotd-channel-id";
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
