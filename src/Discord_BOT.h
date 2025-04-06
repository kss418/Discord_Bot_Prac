#pragma once
#include "Maple_API.h"
#include <dpp/dpp.h>
#include <string>

class Discord_BOT{
public:
    Discord_BOT(const std::string& API_Key, const Maple_API& Maple_API);
    void Run();
private:
    dpp::cluster BOT;
    std::string API_Key;
    Maple_API M_API;

    void Setup_Command();
    void Create_Command();
    void Get_Info(const dpp::slashcommand_t& Event);
};