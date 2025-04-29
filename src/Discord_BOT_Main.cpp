#include "Discord_BOT.h"
#include <iostream>
#include <vector>

Discord_BOT::Discord_BOT(const std::string& API_Key, const Maple_API& M_API) : BOT(API_Key), M_API(M_API){
    Create_Command();
    Setup_Command();
}

void Discord_BOT::Run(){
    BOT.on_log(dpp::utility::cout_logger());
    BOT.start(dpp::st_wait);
}

void Discord_BOT::Create_Command(){
    BOT.on_ready([this](const dpp::ready_t& Event){
        std::cout << "봇이 성공적으로 켜졌습니다" << std::endl;
        //if(dpp::run_once<struct register_bot_commands>()){
            std::vector<dpp::slashcommand> Commands = {
                dpp::slashcommand("정보", "캐릭터 정보 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)),
                dpp::slashcommand("유니온", "유니온 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)),
                dpp::slashcommand("장비", "장비 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)),
                dpp::slashcommand("헥사스킬", "헥사 스킬 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)),
                dpp::slashcommand("헥사스탯", "헥사 스탯 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)),
                dpp::slashcommand("심볼", "심볼 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true))
            };

            for(const auto& CMD : Commands) Add_Command_Guild(CMD);
        //}
    });
}

void Discord_BOT::Setup_Command(){
    BOT.on_slashcommand([this](const dpp::slashcommand_t& Event){
        Handle_Slash_Command(Event);
    });

    BOT.on_button_click([this](const dpp::button_click_t& Event){
        Handle_Button_Click(Event);
    });

    BOT.on_select_click([this](const dpp::select_click_t& Event){
        Handle_Select_Click(Event);
    });
}
