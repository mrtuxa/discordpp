#ifndef INTERACTION_COMMAND_H
#define INTERACTION_COMMAND_H

#include <string>
#include "dispatcher.h"

struct interaction_command {
    class member {
    public:
        static std::string get_avatar(const dpp::slashcommand_t &event) {
            return event.command.member.get_avatar_url();
        }

        static std::string get_name(const dpp::slashcommand_t &event) {
            return event.command.member.get_user()->global_name;
        }
    };
};

#endif // INTERACTION_COMMAND_H
