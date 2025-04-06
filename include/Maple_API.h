#pragma once
#include "Character.h"
#include <string>

class Maple_API{
public:
    Maple_API(const std::string& API_Key);

    const std::string Get_OCID(const std::string& Character_Name) const;
    Character Get_Character_Info(const std::string& Character_Name) const;

private:
    std::string API_Key;
};


