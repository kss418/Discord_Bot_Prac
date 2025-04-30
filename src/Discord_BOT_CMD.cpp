#include "Discord_BOT.h"
#include <iostream>
const dpp::snowflake Guild_ID = 408601520951656448;

void Discord_BOT::Get_Info(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Character_Result = M_API.Get_Info(Character_Name, "character/basic");
    if(Character_Result.Status_Code != 200) { Find_Error(Event, Character_Result.Status_Code); return; }

    API_Result Popularity_Result = M_API.Get_Info(Character_Name, "character/popularity");
    if(Popularity_Result.Status_Code != 200) { Find_Error(Event, Popularity_Result.Status_Code); return; }
    Character Character_Info = std::get<Character>(Character_Result.Data);
    Popularity Popularity_Info = std::get<Popularity>(Popularity_Result.Data);

    dpp::embed Image;
    Image.set_title(Character_Name)
        .set_image(Character_Info.character_image)
        .set_color(0xffcc00)
        .add_field("레벨", std::to_string(Character_Info.character_level), true)
        .add_field("경험치", Character_Info.character_exp_rate + "%", true)
        .add_field("직업", Character_Info.character_class, true)
        .add_field("월드", Character_Info.world_name, true)
        .add_field("길드", Character_Info.character_guild_name.empty() ? "없음" : Character_Info.character_guild_name, true)
        .add_field("인기도", std::to_string(Popularity_Info.popularity), true);
    Event.reply(dpp::message().add_embed(Image));
}

void Discord_BOT::Get_Union(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Union_Result = M_API.Get_Info(Character_Name, "user/union");
    if(Union_Result.Status_Code != 200) { Find_Error(Event, Union_Result.Status_Code); return; }
    Union Union_Info = std::get<Union>(Union_Result.Data);

    dpp::embed Image;
    Image.set_title(Character_Name)
        .set_color(0xffcc00)
        .add_field("유니온 레벨", std::to_string(Union_Info.union_level), true)
        .add_field("등급", Union_Info.union_grade, true)
        .add_field("아티팩트 레벨", std::to_string(Union_Info.union_artifact_level), true);
    Event.reply(dpp::message().add_embed(Image));
}

void Discord_BOT::Get_Equipment(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Equipment_Result = M_API.Get_Info(Character_Name, "character/item-equipment");
    if(Equipment_Result.Status_Code != 200){ Find_Error(Event, Equipment_Result.Status_Code); return; }
    Equipment_Set Equipment = std::get<Equipment_Set>(Equipment_Result.Data);

    Event.reply(dpp::message("정보를 받는 중 입니다."));
    dpp::message Msg = Generate_Equipment_Embed(Equipment.item_equipment, 0);
    Create_Message(Msg, Event, Equipment);
    Event.delete_original_response();
}

void Discord_BOT::Get_Hexa_Skill(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Skill_Result = M_API.Get_Info(Character_Name, "character/skill");
    if(Skill_Result.Status_Code != 200){ Find_Error(Event, Skill_Result.Status_Code); return; }
    Character_Skill Skill_Info = std::get<Character_Skill>(Skill_Result.Data);

    Event.reply(dpp::message("정보를 받는 중 입니다."));
    dpp::message Msg = Generate_Hexa_Skill_Embed(Skill_Info, 0);
    Create_Message(Msg, Event, Skill_Info);
    Event.delete_original_response();
}

void Discord_BOT::Get_Hexa_Stat(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Stat_Result = M_API.Get_Info(Character_Name, "character/hexamatrix-stat");
    if(Stat_Result.Status_Code != 200){ Find_Error(Event, Stat_Result.Status_Code ); return; }
    Hexa_Stat Stat_Info = std::get<Hexa_Stat>(Stat_Result.Data);

    Event.reply(dpp::message("정보를 받는 중 입니다."));
    dpp::message Msg = Generate_Hexa_Stat_Embed(Stat_Info, 0);
    Create_Message(Msg, Event, Stat_Info);
    Event.delete_original_response();
}

void Discord_BOT::Get_Symbol(const dpp::slashcommand_t& Event){
    std::string Character_Name = std::get<std::string>(Event.get_parameter("character_name"));
    API_Result Symbol_Result = M_API.Get_Info(Character_Name, "character/symbol-equipment");
    if(Symbol_Result.Status_Code != 200){ Find_Error(Event, Symbol_Result.Status_Code ); return; }
    Symbol Symbol_Info = std::get<Symbol>(Symbol_Result.Data);

    Event.reply(dpp::message("정보를 받는 중 입니다."));
    dpp::message Msg = Generate_Symbol_Embed(Symbol_Info, 0);
    Create_Message(Msg, Event, Symbol_Info);
    Event.delete_original_response();
}