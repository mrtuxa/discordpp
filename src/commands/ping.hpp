//
// Created by mrtuxa on 7/17/23.
//



#include "appcommand.h"
#include "dispatcher.h"
#include "colors.h"
#include "../include/slash_command.hpp"

dpp::slashcommand command_ping() {
    return dpp::slashcommand()
        .set_name("ping")
        .set_description("Pong!");
}

dpp::embed embed(dpp::cluster& client, const dpp::slashcommand_t& event) {
    return dpp::embed()
            .set_color(dpp::colors::blurple)
            .set_title("Hello " + slash_command::member::get_name(event));
}

void handle_ping(dpp::cluster& client, const dpp::slashcommand_t& event) {
    event.reply(dpp::message().add_embed(embed(client, event)));
};