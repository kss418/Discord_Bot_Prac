#include "Discord_BOT.h"
#include <iostream>

void Discord_BOT::Handle_Slash_Command(const dpp::slashcommand_t& Event){
    if(Event.command.get_command_name() == "정보") Get_Info(Event);
    else if(Event.command.get_command_name() == "유니온") Get_Union(Event);
    else if(Event.command.get_command_name() == "장비") Get_Equipment(Event);
    else if(Event.command.get_command_name() == "헥사스킬") Get_Hexa_Skill(Event);
    else if(Event.command.get_command_name() == "헥사스탯") Get_Hexa_Stat(Event);
    else if(Event.command.get_command_name() == "심볼") Get_Symbol(Event);
}

void Discord_BOT::Handle_Button_Click(const dpp::button_click_t& Event){
    auto& CID = Event.custom_id;
    if(CID == "prev_equipment_page" || CID == "next_equipment_page") Move_Equipment_Page(Event);
    else if(CID == "prev_hexa_skill_page" || CID == "next_hexa_skill_page") Move_Hexa_Skill_Page(Event);
    else if(CID == "prev_hexa_stat_page" || CID == "next_hexa_stat_page") Move_Hexa_Stat_Page(Event);
    else if(CID == "prev_symbol_page" || CID == "next_symbol_page") Move_Symbol_Page(Event);
    else if(CID == "back_summary") Back_Summary_Page(Event);
    else if(CID == "delete_command_message") Delete_Command_Message(Event);
}

void Discord_BOT::Handle_Select_Click(const dpp::select_click_t& Event){
    if(Event.custom_id == "select_equipment") Show_Equipment_Detail(Event);
}