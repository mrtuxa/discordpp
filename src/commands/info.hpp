#include <fstream>
#include "appcommand.h"
#include "colors.h"

dpp::slashcommand command_info() {
    return dpp::slashcommand()
            .set_name("info")
            .set_description("Get info about the bot");
}


// return linux kernel for embed
std::string get_kernel() {
    std::string kernel = "Linux ";
    std::ifstream file("/proc/version");
    std::string str;
    std::getline(file, str);
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    kernel += results[2];
    return kernel;
}

// return cpu name for embed
std::string get_cpu() {
    std::string cpu = "CPU: ";
    std::ifstream file("/proc/cpuinfo");
    std::string str;
    std::getline(file, str);
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    cpu += results[3] + " " + results[4] + " " + results[5] + " " + results[6];
    return cpu;
}

// return ram for embed
std::string get_ram() {
    std::string ram = "RAM: ";
    std::ifstream file("/proc/meminfo");
    std::string str;
    std::getline(file, str);
    std::istringstream iss(str);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    ram += results[1] + " " + results[2];
    return ram;
}


dpp::embed embed_info(dpp::cluster &client, const dpp::slashcommand_t &event) {
    return dpp::embed()
            .set_color(dpp::colors::blurple)
            .set_title("Hello " + interaction_command::member::get_name(event))
            .set_description("i am a bot made by mrtuxa")
            .add_field("Functions",
                       "i can do a lot of things, like play music, or send memes, or even send messages to other servers!",
                       true)
            .add_field("Linux kernel", get_kernel(), true)
            .add_field("CPU", get_cpu(), true)
            .add_field("RAM", get_ram(), true);

}

void handle_info(dpp::cluster &client, const dpp::slashcommand_t &event) {
    event.reply(dpp::message().add_embed(embed_info(client, event)));
};