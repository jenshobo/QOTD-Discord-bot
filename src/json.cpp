#include "json.h"

/// <summary>
/// Update the queues in json in the new state unless stated std::nullopt
/// </summary>
void write_queue(
    const std::string& filename,
    std::optional<std::string> default_val = std::nullopt,
    std::optional<std::string> bot_token = std::nullopt,
    std::optional<uint64_t> offset = std::nullopt,
    std::optional<dpp::snowflake> channel_id = std::nullopt,
    std::optional<dpp::snowflake> alert_channel_id = std::nullopt,
    std::optional<std::queue<std::string>> prioqueue = std::nullopt,
    std::optional<std::queue<std::string>> queue = std::nullopt
) {
    std::string default_field = default_val.value_or(get_default(filename));
    std::string token_field = bot_token.value_or(get_token(filename));
    uint64_t offset_field = offset.value_or(get_offset(filename));
    dpp::snowflake channel_field = channel_id.value_or(get_channel(filename));
    dpp::snowflake alert_channel_field = alert_channel_id.value_or(get_alert_channel(filename));
    std::queue<std::string> prioqueue_field = prioqueue.value_or(load_prioqueue(filename));
    std::queue<std::string> queue_field = queue.value_or(load_queue(filename));

    std::vector<std::string> prioqueue_vec;
    std::queue<std::string> temp_prioqueue = prioqueue_field;
    while (!temp_prioqueue.empty()) {
        prioqueue_vec.push_back(temp_prioqueue.front());
        temp_prioqueue.pop();
    }

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
    j["qotd-alert-channel-id"] = static_cast<uint64_t>(alert_channel_field);
    j["prioqueue"] = prioqueue_vec;
    j["queue"] = queue_vec;

    std::ofstream file(filename);
    file << j.dump(4);
}

/// <summary>
/// Save queue's in new state
/// </summary>
void save_queue(const std::queue<std::string>& prioqueue, const std::queue<std::string>& queue, const std::string& filename) {
    write_queue(filename, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, prioqueue, queue);
}

/// <summary>
/// Get the regualr queue from json
/// </summary>
std::queue<std::string> load_queue(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    std::queue<std::string> q;

    std::string field_name = "queue";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_array()) {
            for (const auto& item : j[field_name]) {
                q.push(item.get<std::string>());
            }
        }
    }

    return q;
}

/// <summary>
/// Get the priority queue from json
/// </summary>
std::queue<std::string> load_prioqueue(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;
    std::queue<std::string> q;

    std::string field_name = "prioqueue";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_array()) {
            for (const auto& item :j[field_name]) {
                q.push(item.get<std::string>());
            }
        }
    }

    return q;
}

/// <summary>
/// Get current offset from json used as index for questions in queue
/// </summary>
uint64_t get_offset(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string field_name = "offset";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_number_unsigned()) {
            return static_cast<uint64_t>(j[field_name]);
        }
    }

    return 0;
}

/// <summary>
/// Increment offset in json used as question index
/// </summary>
void increment_offset(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    uint64_t offset = 0;
    std::string field_name = "offset";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_number_unsigned()) {
            offset = static_cast<uint64_t>(j[field_name]);
        }
    }
    
    offset++; // max 0xFFFFFFFFFFFFFFFF
    // ~ 50498110278627 years from now, 
    // I do believe that would cover the total length of time the student association will be around, 
    // considering the sun goes a little earlier...

    write_queue(filename, std::nullopt, std::nullopt, offset);
}

/// <summary>
/// Get Discord bot token from json
/// </summary>
std::string get_token(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string field_name = "discord-bot-token";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_string()) {
            return j[field_name];
        }
    }

    return "";
}

/// <summary>
/// Get QOTD channel id from json
/// </summary>
dpp::snowflake get_channel(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string field_name = "qotd-channel-id";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_number_integer()) {
            return static_cast<dpp::snowflake>(j[field_name].get<uint64_t>());
        }
    }

    return 0;
}

/// <summary>
/// Get alert channel id from json
/// </summary>
dpp::snowflake get_alert_channel(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string field_name = "qotd-alert-channel-id";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_number_integer()) {
            return static_cast<dpp::snowflake>(j[field_name].get<uint64_t>());
        }
    }

    return 0;
}

/// <summary>
/// Get default question from json
/// </summary>
std::string get_default(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j;

    std::string field_name = "default";
    if (file >> j) {
        if (j.contains(field_name) && j[field_name].is_string()) {
            return j[field_name];
        }
    }

    return "";
}
