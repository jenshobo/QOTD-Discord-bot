#include "main.h"

/// <summary>
/// Post current question of the day in Discord in channel({qotd-channel-id})
/// Close program automatically after posting.
/// </summary>
int main(void) {
    dpp::cluster bot(get_token(QUEUE_FILE_NAME));

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::string question = get_question();

        bot.message_create(dpp::message(get_channel(QUEUE_FILE_NAME), question),
            [&bot](const dpp::confirmation_callback_t& callback) {
                if (!callback.is_error()) {
                    std::cout << "Question sent successfully" << std::endl;
                } else {
                    std::cerr << "Failed to send question: " << callback.get_error().message << std::endl;
                }

                // Keep program running for at least {SHUTDOWN_DELAY} time for shards to finish loading
                std::this_thread::sleep_for(std::chrono::seconds(SHUTDOWN_DELAY));

                bot.shutdown();
            }
        );
    });

    bot.start(dpp::st_wait);
    return 0;
}

/// <summary>
/// Get next question from current queue json file
/// Priority order queue:
/// 1. prioqueue
/// 2. queue
/// 3. default question
/// </summary>
std::string get_question(void) {
    auto prioqueue = load_prioqueue(QUEUE_FILE_NAME);
    auto queue = load_queue(QUEUE_FILE_NAME);

    std::stringstream ss;
    if (!prioqueue.empty()) {
        ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << prioqueue.front();
        prioqueue.pop();
    }
    else if (!queue.empty()) {
        ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << queue.front();
        queue.pop();
    }
    else {
        ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << get_default(QUEUE_FILE_NAME);
    }

    increment_offset(QUEUE_FILE_NAME);
    save_queue(prioqueue, queue, QUEUE_FILE_NAME);

    return ss.str();
}
