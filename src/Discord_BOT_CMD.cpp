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
    if(Character_Info.Status_Code != 200) { Find_Error(Event, Character_Info.Status_Code); return; }

    Popularity Popularity_Info = M_API.Get_Popularity_Info(Character_Name);
    if(Popularity_Info.Status_Code != 200) { Find_Error(Event, Popularity_Info.Status_Code); return; }

    dpp::embed Image;
    Image.set_title(Character_Name)
        .set_image(Character_Info.Image_URL)
        .set_color(0xffcc00)
        .add_field("레벨", std::to_string(Character_Info.Level), true)
        .add_field("경험치", Character_Info.EXP_Rate + "%", true)
        .add_field("직업", Character_Info.Class, true)
        .add_field("월드", Character_Info.World_Name, true)
        .add_field("길드", Character_Info.Guild_Name.empty() ? "없음" : Character_Info.Guild_Name, true)
        .add_field("인기도", std::to_string(Popularity_Info.Popularity), true);
    Event.reply(dpp::message().add_embed(Image));
}

void Discord_BOT::Get_Union(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    Union Union_Info = M_API.Get_Union_Info(Character_Name);
    if(Union_Info.Status_Code != 200) { Find_Error(Event, Union_Info.Status_Code); return; }

    dpp::embed Image;
    Image.set_title(Character_Name)
        .set_color(0xffcc00)
        .add_field("유니온 레벨", std::to_string(Union_Info.Union_Level), true)
        .add_field("등급", Union_Info.Union_Grade, true)
        .add_field("아티팩트 레벨", std::to_string(Union_Info.Artifact_Level), true);
    Event.reply(dpp::message().add_embed(Image));
}

void Discord_BOT::Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page, const dpp::slashcommand_t& Event){
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

    dpp::message msg(page ? page + "번 장비" : "현재 장착 장비");
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
    
    Event.reply(msg);
}

void Discord_BOT::Get_Equipment(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    Equipment_Set Equipment_Set = M_API.Get_Equipment_Info(Character_Name);
    if(Equipment_Set.Status_Code != 200){ Find_Error(Event, Equipment_Set.Status_Code); return; }
    
    size_t index = 0;
    User_Page[Event.command.get_issuing_user().id] = index;

    Generate_Equipment_Embed(Equipment_Set.Info[index], index, Event);
}