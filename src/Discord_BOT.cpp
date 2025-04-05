#include "Discord_BOT.h"
#include <iostream>

const dpp::snowflake Guild_ID = 1240679613072539709;

Discord_BOT::Discord_BOT(const std::string& API_Key, const Maple_API& M_API) : BOT(API_Key), M_API(M_API){
    Setup_Events();
}

void Discord_BOT::Run(){
    BOT.start(dpp::st_wait);
}

void Discord_BOT::Setup_Events(){
    BOT.on_ready([this](const dpp::ready_t& Event){
        std::cout << "봇이 성공적으로 켜졌습니다" << std::endl;
        if(dpp::run_once<struct register_bot_commands>()){
            dpp::slashcommand Get_Profile("정보_디버그", "사진 출력", BOT.me.id);
            Get_Profile.add_option(dpp::command_option(dpp::co_string, "Character_Name", "닉네임", true));
            BOT.guild_command_create(Get_Profile, Guild_ID);
        }
    });
    
    BOT.on_slashcommand([this](const dpp::slashcommand_t& Event){
        if(Event.command.get_command_name() == "정보_디버그"){
            Event.reply(std::get<std::string>(Event.get_parameter("Character_Name")));
        }
    });
}
