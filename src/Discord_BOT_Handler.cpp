#include "Discord_BOT.h"
#include <iostream>

void Discord_BOT::Handle_Slash_Command(const dpp::slashcommand_t& Event){
    if(Event.command.get_command_name() == "정보") Get_Info(Event);
    if(Event.command.get_command_name() == "유니온") Get_Union(Event);
    if(Event.command.get_command_name() == "장비") Get_Equipment(Event);
}

void Discord_BOT::Handle_Button_Click(const dpp::button_click_t& Event){
    if(Event.custom_id == "prev_page") Move_Page(Event);
    if(Event.custom_id == "next_page") Move_Page(Event);
}

void Discord_BOT::Handle_Select_Click(const dpp::select_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    if(Event.values.empty()) return;
    int Index = std::stoi(Event.values[0]);
    Equipment_Info& Equipment = User_Equipment_Map[UID].Info[User_Page[UID]][Index];
}