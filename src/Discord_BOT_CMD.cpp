#include "Discord_BOT.h"
#include <iostream>
const dpp::snowflake Guild_ID = 408601520951656448;

void Discord_BOT::Add_Command_Guild(const dpp::slashcommand& CMD){
    BOT.guild_command_create(CMD, Guild_ID,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Command_Create_Log(cb, CMD);
    });
}

void Discord_BOT::Add_Command_Global(const dpp::slashcommand& CMD){
    BOT.global_command_create(CMD,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Command_Create_Log(cb, CMD);
    });
}

void Discord_BOT::Get_Info(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    Character Character_Info = M_API.Get_Character_Info(Character_Name);
    if(Character_Info.Status_Code == 400){
        dpp::embed error;
        error.set_title("닉네임 오류")
            .set_description("존재하지 않는 캐릭터입니다.")
            .set_color(0xff0000);
        Event.reply(dpp::message().add_embed(error));
    }
    else{
        dpp::embed Image;
        Image.set_title(Character_Name + "의 캐릭터")
            .set_image(Character_Info.Image_URL)
            .set_color(0xffcc00)
            .add_field("레벨", std::to_string(Character_Info.Level), true)
            .add_field("경험치", Character_Info.EXP_Rate + "%", true)
            .add_field("직업", Character_Info.Class, true)
            .add_field("월드", Character_Info.World_Name, true)
            .add_field("길드", Character_Info.Guild_Name.empty() ? "없음" : Character_Info.Guild_Name, true);
    
        Event.reply(dpp::message().add_embed(Image));
    }
}

void Discord_BOT::Get_Union(const dpp::slashcommand_t& Event){
    
}
