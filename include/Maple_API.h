#pragma once
#include "Custom_Class.h"
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

class Maple_API{
public:
    Maple_API(const std::string& API_Key);

    const std::string Get_OCID(const std::string& Character_Name) const;
    Character Get_Character_Info(const std::string& Character_Name) const;
    Union Get_Union_Info(const std::string& Character_Name) const;
    Popularity Get_Popularity_Info(const std::string& Character_Name) const;
    Equipment_Set Get_Equipment_Info(const std::string& Character_Name) const;
    Character_Skill Get_Hexa_Core_Info(const std::string& Character_Name) const;

    void Parse_Equipment_Info(std::vector <Equipment_Info>& Info, const nlohmann::json& data) const;
    void Parse_Option_Info(Option& Info, const nlohmann::json& data) const;
    void Parse_Potential_Option_Info(Potential_Option& Info, const nlohmann::json& data) const;
private:
    std::string API_Key;
    mutable std::unordered_map <std::string, std::string> OCID_Map;
};


