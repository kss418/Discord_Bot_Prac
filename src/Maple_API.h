#pragma once
#include <string>

class Maple_API{
public:
    Maple_API(const std::string& API_Key);

    const std::string& Get_OCID(const std::string& Character_Name) const;
    std::string Get_Character_Image_URL(const std::string& Character_Name) const;

private:
    std::string API_Key;
};


