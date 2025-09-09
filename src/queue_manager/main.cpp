#include "main.h"

int main() {
    dpp::cluster bot(get_token(QUEUE_FILE_NAME));
	 
    bot.on_log(dpp::utility::cout_logger());
 
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "help") {
            std::stringstream ss;
            ss
            << "You can add and remove questions to the QOTD (Question Of The Day) queue using the following commands" << std::endl
            << "```" << std::endl
            << "+----------+-----------------------------------------------+" << std::endl
            << "| /add     | Add a question to the back of the queue       |" << std::endl
            << "| /addprio | Add a question to the front of the queue      |" << std::endl
            << "| /remove  | Remove a question BY INDEX                    |" << std::endl
            << "| /list    | List the current queue with index             |" << std::endl
            << "| /help    | You just used this command, don't be dumb now |" << std::endl
            << "+----------+-----------------------------------------------+" << std::endl
            << "```" << std::endl
            << "The question at the front of the queue will automatically be posted to the QOTD channel and removed from the queue every day at 12:00." << std::endl
            << "Please make sure the queue is alway filled with items." << std::endl
            << std::endl
            << "Adding an index to the question is not needed, the program does that automatically." <<std::endl
            << "For further questions regarding this software, please contact the software commission." << std::endl;
            event.reply(ss.str());
        }

        if (event.command.get_command_name() == "add") {
            std::string question_input = std::get<std::string>(event.get_parameter("question"));
            std::stringstream ss;
            ss << "Adding " << question_input << " to the queue.";
            event.reply(ss.str());

            add_to_queue(question_input);
        }

        if (event.command.get_command_name() == "addprio") {
            std::string question_input = std::get<std::string>(event.get_parameter("question"));
            std::stringstream ss;
            ss << "Adding " << question_input << " to the front of the queue.";
            event.reply(ss.str());

            add_to_queue_priority(question_input);
        }

        if (event.command.get_command_name() == "remove") {
            uint16_t index_input = static_cast<uint16_t>(std::get<int64_t>(event.get_parameter("index")));
            std::stringstream ss;
            ss << "Removing question " << index_input << " from queue";
            event.reply(ss.str());

            remove_from_queue(index_input);
        }

        if (event.command.get_command_name() == "list") {
            std::stringstream ss;

            auto queue = load_queue(QUEUE_FILE_NAME);
            uint16_t offset = get_offset(QUEUE_FILE_NAME);

            if (queue.empty()) {
                event.reply("Queue is empty.");
            }
            else {
                while (!queue.empty()) {
                    ss << "[" << offset << "]. " << queue.front() << std::endl;
                    queue.pop();
                    offset++;
                }

                event.reply(ss.str());
            }
        }
    });
	 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("help", "I can't help you with stupidity", bot.me.id));
            bot.global_command_create(dpp::slashcommand("add", "Add question to the QOTD queue", bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "question", "The question to add", true)
                )
            );
            bot.global_command_create(dpp::slashcommand("addprio", "Add priority question to the front of the QOTD queue", bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_string, "question", "The priority question to add", true)
                )
            );
            bot.global_command_create(dpp::slashcommand("remove", "Remove item from queue by index (use /list for indexes)", bot.me.id)
                .add_option(
                    dpp::command_option(dpp::co_integer, "index", "The index of the item in queue to remove", true)
                )
            );
            bot.global_command_create(dpp::slashcommand("list", "List all questions in the queue", bot.me.id));
        }
    });
	 
    bot.start(dpp::st_wait);
}

void add_to_queue(std::string& question) {
    auto queue = load_queue(QUEUE_FILE_NAME);

    queue.push(question);

    save_queue(queue, QUEUE_FILE_NAME);
}

void add_to_queue_priority(std::string& question) {
    auto queue = load_queue(QUEUE_FILE_NAME);

    std::queue<std::string> newQueue;
    newQueue.push(question);

    while (!queue.empty()) {
        newQueue.push(queue.front());
        queue.pop();
    }

    save_queue(newQueue, QUEUE_FILE_NAME);
}

void remove_from_queue(uint16_t index)
{
    auto queue = load_queue(QUEUE_FILE_NAME);
    uint16_t offset = get_offset(QUEUE_FILE_NAME);

    std::queue<std::string> newQueue;
    while (!queue.empty()) {
        if (offset != index) {
            newQueue.push(queue.front());
        }
        queue.pop();
        offset++;
    }

    save_queue(newQueue, QUEUE_FILE_NAME);
}