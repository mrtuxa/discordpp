//
// Created by mrtuxa on 7/16/23.
//

#include "include/secrets.hpp"
#include "cluster.h"
#include "once.h"
#include "commands/ping.hpp"
#include "commands/info.hpp"
#include <sstream>
std::string token = TOKEN;

// Global flag to indicate if Ctrl+C was pressed
volatile sig_atomic_t gSignalFlag = 0;

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT) {
        gSignalFlag = 1;
    }
}

int main() {

    dpp::cluster client(token, dpp::i_all_intents);

    client.on_log(dpp::utility::cout_logger());


    client.on_slashcommand([&client](const dpp::slashcommand_t &event) {
        if (event.command.get_command_name() == "ping") {
            handle_ping(client, event);
        } else if (event.command.get_command_name() == "info") {
            handle_info(client, event);
        }
    });

    client.on_ready([&client](const dpp::ready_t &event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            std::vector<dpp::slashcommand> commands = {
                    command_ping(),
                    command_info()
            };

            /**
             * b
             * IT'S not working
             * ERROR: error creating slash commands: {"message": "Missing Access", "code": 50001}
            client.guild_bulk_command_create(commands, GUILD_ID, [&client](const dpp::confirmation_callback_t &event) {
                if (event.is_error()) {
                    client.log(dpp::ll_error, "error creating slash commands: " + event.http_info.body);
                } else {
                    client.log(dpp::ll_info, "success creating slash commands");
                }
            });
            */
            client.global_bulk_command_create(commands, [&client](const dpp::confirmation_callback_t &event) {
                if (event.is_error()) {
                    client.log(dpp::ll_error, "error creating slash commands: " + event.http_info.body);
                } else {
                    client.log(dpp::ll_info, "success creating slash commands");
                }
            });
        }
    });

    client.on_guild_member_add([&client](const dpp::guild_member_add_t &event) {
        // Now you can use the 'event' parameter inside the lambda function.
        std::string name = event.added.get_user()->global_name;
    });


    client.start(dpp::st_wait);

    std::signal(SIGINT, signalHandler);

    std::cout << "Press Ctrl+C to trigger the event. Press 'q' and Enter to quit.\n";

    // Main loop
    while (true) {
        // Check if Ctrl+C was pressed
        if (gSignalFlag) {
            std::cout << "Ctrl+C was pressed. Handling the event..." << std::endl;
            // Do your desired actions here when Ctrl+C is pressed.
            // For example, cleanup, saving data, etc.

            // Reset the flag
            gSignalFlag = 0;


        }
    }

    std::cout << "Exiting the program." << std::endl;
}