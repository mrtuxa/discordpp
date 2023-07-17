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

    client.on_message_create([&client](const dpp::message_create_t & event) {

        std::stringstream ss(event.msg.content);
        std::string command;

        ss >> command;

        /* Switch to or join the vc the user is on. Syntax: .join  */
        if (command == ".join") {
            dpp::guild * g = dpp::find_guild(event.msg.guild_id);
            auto current_vc = event.from->get_voice(event.msg.guild_id);
            bool join_vc = true;
            /* Check if we are currently on any vc */
            if (current_vc) {
                /* Find the channel id  that the user is currently on */
                auto users_vc = g->voice_members.find(event.msg.author.id);
                /* See if we currently share a channel with the user */
                if (users_vc != g->voice_members.end() && current_vc->channel_id == users_vc->second.channel_id) {
                    join_vc = false;
                    /* We are on this voice channel, at this point we can send any audio instantly to vc:

                     * current_vc->send_audio_raw(...)
                     */
                } else {
                    /* We are on a different voice channel. Leave it, then join the new one
                     * by falling through to the join_vc branch below.
                     */
                    event.from->disconnect_voice(event.msg.guild_id);
                    join_vc = true;
                }
            }
            /* If we need to join a vc at all, join it here if join_vc == true */
            if (join_vc) {
                if (!g->connect_member_voice(event.msg.author.id)) {
                    /* The user issuing the command is not on any voice channel, we can't do anything */
                    client.message_create(dpp::message(event.msg.channel_id, "You don't seem to be on a voice channel! :("));
                } else {
                    /* We are now connecting to a vc. Wait for on_voice_ready
                     * event, and then send the audio within that event:
                     *
                     * event.voice_client->send_audio_raw(...);
                     *
                     * NOTE: We can't instantly send audio, as we have to wait for
                     * the connection to the voice server to be established!
                     */
                }
            }
        }
    });

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

            client.guild_bulk_command_create(commands, GUILD_ID, [&client](const dpp::confirmation_callback_t &event) {
                if (event.is_error()) {
                    client.log(dpp::ll_error, "error creating slash commands: " + event.http_info.body);
                } else {
                    client.log(dpp::ll_info, "success creating slash commands");
                }
            });
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