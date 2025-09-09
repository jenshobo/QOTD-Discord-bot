#include "main.h"

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

                bot.shutdown();
            }
        );
    });

    bot.start(dpp::st_wait);
    return 0;
}

std::string get_question(void) {
    auto queue = load_queue(QUEUE_FILE_NAME);

    std::stringstream ss;
    if (!queue.empty()) {
        ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << queue.front();
        queue.pop();
    }
    else {
        ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << get_default(QUEUE_FILE_NAME);
    }

    increment_offset(QUEUE_FILE_NAME);
    save_queue(queue, QUEUE_FILE_NAME);

    return ss.str();
}
