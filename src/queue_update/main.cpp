#include "main.h"

int main(void) {
    dpp::cluster bot(DISCORD_BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::string question = get_question();

        bot.message_create(dpp::message(DISCORD_CHANNEL_ID, question),
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

    if (queue.empty()) {
        std::cerr << "Queue is empty, cannot get next question." << std::endl;
    }

    std::stringstream ss;
    ss << "[" << get_offset(QUEUE_FILE_NAME) << "]. " << queue.front();
    increment_offset(QUEUE_FILE_NAME);
    queue.pop();

    save_queue(queue, QUEUE_FILE_NAME);

    return ss.str();
}
