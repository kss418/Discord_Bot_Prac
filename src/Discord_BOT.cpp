#include "Discord_BOT.h"
#include <iostream>
#include <vector>

const dpp::snowflake Guild_ID = 408601520951656448;

Discord_BOT::Discord_BOT(const std::string& API_Key, const Maple_API& M_API) : BOT(API_Key), M_API(M_API){
    Create_Command();
    Setup_Command();
}

void Discord_BOT::Run(){
    BOT.on_log(dpp::utility::cout_logger());
    BOT.start(dpp::st_wait);
}

void Discord_BOT::Command_Create_Log(const dpp::confirmation_callback_t& cb, const dpp::slashcommand& cmd){
    if(cb.is_error()) std::cout << cmd.name << " 등록 실패: " << cb.get_error().message << std::endl;
    else std::cout << cmd.name << " 등록 성공" << std::endl;
}

void Discord_BOT::Add_Command_Guild(const dpp::slashcommand& CMD){
    BOT.guild_command_create(CMD, Guild_ID,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Command_Create_Log(cb, CMD);
    });
}

void Discord_BOT::Create_Command(){
    BOT.on_ready([this](const dpp::ready_t& Event){
        std::cout << "봇이 성공적으로 켜졌습니다" << std::endl;
        //if(dpp::run_once<struct register_bot_commands>()){
            std::vector<dpp::slashcommand> Commands = {
                dpp::slashcommand("정보", "사진 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true))
            };

            for(const auto& CMD : Commands) Add_Command_Guild(CMD);
        //}
    });
}

void Discord_BOT::Setup_Command(){
    BOT.on_slashcommand([this](const dpp::slashcommand_t& Event){
        if(Event.command.get_command_name() == "정보") Get_Info(Event);
        if(Event.command.get_command_name() == "유니온") Get_Union(Event);
    });
}
