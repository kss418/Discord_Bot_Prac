#pragma once
#include "Maple_API.h"
#include <dpp/dpp.h>
#include <string>
#include <unordered_map>

class Discord_BOT{
public:
    Discord_BOT(const std::string& API_Key, const Maple_API& Maple_API);
    void Run();
private:
    dpp::cluster BOT;
    std::string API_Key;
    Maple_API M_API;
    std::unordered_map<dpp::snowflake, size_t> User_Page;
    std::unordered_map<dpp::snowflake, Equipment_Set> User_Equipment_Map;

    void Setup_Command();
    void Create_Command();
    void Command_Create_Log(const dpp::confirmation_callback_t& cb, const dpp::slashcommand& cmd);

    void Get_Info(const dpp::slashcommand_t& Event);
    void Get_Union(const dpp::slashcommand_t& Event);
    void Get_Equipment(const dpp::slashcommand_t& Event);

    void Find_Error(const dpp::slashcommand_t& Event, int32_t Status_Code);
    void Not_Found_Name(const dpp::slashcommand_t& Event);
    void Internal_Server_Error(const dpp::slashcommand_t& Event);
    void Many_Requests(const dpp::slashcommand_t& Event);

    void Add_Command_Guild(const dpp::slashcommand& CMD);
    void Add_Command_Global(const dpp::slashcommand& CMD);

    dpp::message Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page);
};