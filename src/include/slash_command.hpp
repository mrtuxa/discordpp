#include <string>
#include "dispatcher.h"

struct slash_command {
    struct member {
        static std::string get_name(const dpp::slashcommand_t& event) {
            return event.command.member.get_user()->global_name;
        }
        static std::string get_avatar(const dpp::slashcommand_t& event) {
            return event.command.member.get_avatar_url();
        }
    };
};
