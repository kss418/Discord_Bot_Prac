#pragma once
#include "Maple_API.h"
#include <dpp/dpp.h>
#include <string>

class Discord_BOT{
public:
    Discord_BOT(const std::string& API_Key, const Maple_API& Maple_API);
    void Run();
    void Send_Image(std::string& Image_URL);
private:
    dpp::cluster BOT;
    std::string API_Key;
    Maple_API M_API;

    void Setup_Events();
};