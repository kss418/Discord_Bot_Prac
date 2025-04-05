#pragma once
#include <string>

class Maple_API{
public:
    Maple_API(const std::string& Character_Name, const std::string& API_Key);

    const std::string& Get_OCID() const;
    std::string Get_Character_Image_URL() const;

private:
    std::string OCID;
    std::string Character_Name;
    std::string API_Key;

    bool Response_OCID();
};


