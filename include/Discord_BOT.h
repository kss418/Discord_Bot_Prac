#pragma once
#include "Maple_API.h"
#include <dpp/dpp.h>
#include <string>
#include <unordered_map>
#include <variant>

class Discord_BOT{
public:
    Discord_BOT(const std::string& API_Key, const Maple_API& Maple_API);
    void Run();
private:
    using Map_Type = std::variant<Equipment_Set, Character_Skill, Hexa_Stat, Symbol>;
    dpp::cluster BOT;
    std::string API_Key;
    Maple_API M_API;
    std::unordered_map<dpp::snowflake, size_t> Message_Page;
    std::unordered_map<dpp::snowflake, Map_Type> Message_Map;
    std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, dpp::snowflake>> Message_Info;

    using Member_Ptr = std::string Option::*;
    const std::unordered_map<std::string, std::pair<std::string, Member_Ptr>> Option_List = {
        {"str", {"STR", &Option::str}}, {"dex", {"DEX", &Option::dex}},           
        {"int_", {"INT", &Option::int_}}, {"luk", {"LUK", &Option::luk}},           
        {"max_hp", {"최대 HP", &Option::max_hp}},
        {"attack_power",{"공격력", &Option::attack_power}},
        {"magic_power", {"마력", &Option::magic_power}},
        {"boss_damage", {"보스 몬스터 공격 시 데미지", &Option::boss_damage}},
        {"ignore_monster_armor",{"몬스터 방어율 무시",&Option::ignore_monster_armor}},
        {"all_stat",  {"올스탯", &Option::all_stat}},
        {"max_hp_rate",{"최대 HP", &Option::max_hp_rate}}
    };

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
    std::string Get_Option_Value(const Option& option, const std::string& Key);

    dpp::message Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page);
    dpp::message Generate_Hexa_Skill_Embed(const Character_Skill& Skill, int page);
    dpp::message Generate_Hexa_Stat_Embed(const Hexa_Stat& Stat, int page);
    dpp::message Generate_Symbol_Embed(const Symbol& Symbol, int page);

    void Create_Message(dpp::message& Msg, const dpp::slashcommand_t& Event, const Map_Type& Map);

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
    std::string Get_Symbol_Progress(const Symbol::Info& Symbol) const;
    dpp::component Create_Move_Page_Component(const std::string& Custom_ID) const;

    const int32_t Skill_Per_Page = 4;
    const int32_t Hexa_Core_Count = 3;
};