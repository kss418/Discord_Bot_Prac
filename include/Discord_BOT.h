#pragma once
#include "Maple_API.h"
#include <dpp/coro.h>
#include <dpp/dpp.h>
#include <string>
#include <unordered_map>
const int32_t Skill_Per_Page = 4;
const int32_t Hexa_Core_Count = 3;

class Discord_BOT{
public:
    Discord_BOT(const std::string& API_Key, const Maple_API& Maple_API);
    void Run();
private:
    dpp::cluster BOT;
    std::string API_Key;
    Maple_API M_API;
    std::unordered_map<dpp::snowflake, size_t> Message_Page;
    std::unordered_map<dpp::snowflake, Equipment_Set> Message_Equipment_Map;
    std::unordered_map<dpp::snowflake, Character_Skill> Message_Skill_Map;
    std::unordered_map<dpp::snowflake, Hexa_Stat> Message_Hexa_Stat_Map;
    std::unordered_map<dpp::snowflake, Symbol> Message_Symbol_Map;
    std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, dpp::snowflake>> Message_Info;

    void Setup_Command();
    void Create_Command();

    void Handle_Slash_Command(const dpp::slashcommand_t& Event);
    void Handle_Button_Click(const dpp::button_click_t& Event);
    void Handle_Select_Click(const dpp::select_click_t& Event);

    void Create_Command_Log(const dpp::confirmation_callback_t& cb, const dpp::slashcommand& cmd) const;
    bool Create_Message_Log(const dpp::confirmation_callback_t& cb) const;

    void Get_Info(const dpp::slashcommand_t& Event);
    void Get_Union(const dpp::slashcommand_t& Event);
    void Get_Equipment(const dpp::slashcommand_t& Event);
    void Get_Hexa_Skill(const dpp::slashcommand_t& Event);
    void Get_Hexa_Stat(const dpp::slashcommand_t& Event);
    void Get_Symbol(const dpp::slashcommand_t& Event);

    void Find_Error(const dpp::slashcommand_t& Event, int32_t Status_Code);
    void Not_Found_Name(const dpp::slashcommand_t& Event);
    void Internal_Server_Error(const dpp::slashcommand_t& Event);
    void Many_Requests(const dpp::slashcommand_t& Event);

    void Add_Command_Guild(const dpp::slashcommand& CMD);
    void Add_Command_Global(const dpp::slashcommand& CMD);

    void Edit_Prev_Message(dpp::message& Msg, const dpp::snowflake& UID);
    void Show_Equipment_Detail(const dpp::select_click_t& Event);
    uint32_t Get_Potential_Color(const std::string& Potential_Grade) const;
    std::string Get_Equipment_Detail_Message(const Equipment_Info& Equipment);
    std::string Get_Equipment_Detail_Option(const Equipment_Info& Equipment, const std::string& Key);
    std::string Get_Equipment_Name(const Equipment_Info& Equipment) const;
    std::string Get_Map_By_Key(const std::unordered_map<std::string, std::string>& Map, const std::string& Key);

    dpp::message Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page);
    dpp::message Generate_Hexa_Skill_Embed(const Character_Skill& Skill, int page);
    dpp::message Generate_Hexa_Stat_Embed(const Hexa_Stat& Stat, int page);
    dpp::message Generate_Symbol_Embed(const Symbol& Symbol, int page);

    void Create_Equipment_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Equipment_Set& Equipments);
    void Create_Skill_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Character_Skill& Skill);
    void Create_Hexa_Stat_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Hexa_Stat& Stat);
    void Create_Symbol_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Symbol& Symbol);

    void Move_Equipment_Page(const dpp::button_click_t& Event);
    void Move_Hexa_Skill_Page(const dpp::button_click_t& Event);
    void Move_Hexa_Stat_Page(const dpp::button_click_t& Event);
    void Move_Symbol_Page(const dpp::button_click_t& Event);
    void Back_Summary_Page(const dpp::button_click_t& Event);
    void Delete_Command_Message(const dpp::button_click_t& Event);

    bool Is_Percentage(const std::string& Key) const;
    bool Is_Starforce(const Equipment_Info& Equipment) const;
    bool Is_Scroll(const Equipment_Info& Equipment) const;
    bool Is_Additional_Option(const Equipment_Info& Equipment) const;
};