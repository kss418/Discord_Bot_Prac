#include "Discord_BOT.h"
#include <iostream>
const dpp::snowflake Guild_ID = 408601520951656448;

void Discord_BOT::Add_Command_Guild(const dpp::slashcommand& CMD){
    BOT.guild_command_create(CMD, Guild_ID,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

void Discord_BOT::Add_Command_Global(const dpp::slashcommand& CMD){
    BOT.global_command_create(CMD,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

dpp::message Discord_BOT::Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page){
    dpp::component menu;
    menu.set_type(dpp::cot_selectmenu)
        .set_placeholder("장비 선택")
        .set_id("select_equipment");

    for(int i = 0;i < Info.size();i++){
        Equipment_Info Current_Equipment = Info[i];
        std::string description = "★" + Current_Equipment.Starforce;
        if(!Current_Equipment.Potential_Option_Info.Grade.empty()) description += " | " + Current_Equipment.Potential_Option_Info.Grade;
        if(!Current_Equipment.Potential_Option_Info.Additional_Grade.empty()) description += " | " + Current_Equipment.Potential_Option_Info.Additional_Grade;

        menu.add_select_option(
            dpp::select_option(Current_Equipment.Item_Name, std::to_string(i + 1))
                .set_description(description)
        );
    }

    dpp::component row;
    row.add_component(menu);

    dpp::message msg(page ? std::to_string(page) + "번 프리셋 장비" : "현재 장착 장비");
    msg.add_component(row);
    msg.add_component(dpp::component()
        .add_component(dpp::component()
            .set_label("◀️")
            .set_id("prev_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("▶️")
            .set_id("next_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
    );
    
    return msg;
}

void Discord_BOT::Edit_Prev_Message(dpp::message& Msg, const dpp::snowflake& UID){
    Msg.id = Message_Info[UID].first;
    Msg.channel_id = Message_Info[UID].second;
    BOT.message_edit(Msg);
}


