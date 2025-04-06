#include "Discord_BOT.h"
#include <iostream>

const dpp::snowflake Guild_ID = 1240679613072539709;

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
            BOT.guild_command_create(
                dpp::slashcommand("정보", "사진 출력", BOT.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "character_name", "닉네임", true)), 
                Guild_ID, 
                [](const dpp::confirmation_callback_t& cb) {
                    if (cb.is_error()) std::cout << "명령어 등록 실패: " << cb.get_error().message << std::endl;
                    else std::cout << "명령어 등록 성공!" << std::endl;
            });
        //}
    });
}

void Discord_BOT::Setup_Command(){
    BOT.on_slashcommand([this](const dpp::slashcommand_t& Event){
        if(Event.command.get_command_name() == "정보") Get_Info(Event);
    });
}

void Discord_BOT::Get_Info(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    std::string Image_URL = M_API.Get_Character_Image_URL(Character_Name);
    if(Image_URL == "-400"){
        dpp::embed error;
        error.set_title("닉네임 오류")
            .set_description("존재하지 않는 캐릭터입니다.")
            .set_color(0xff0000);
        Event.reply(dpp::message().add_embed(error));
    }
    else{
        dpp::embed Image;
        Image.set_title(Character_Name + "의 캐릭터")
        .set_image(Image_URL)
        .set_color(0xffcc00);
    
        Event.reply(dpp::message().add_embed(Image));
    }
}
