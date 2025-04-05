#include "Maple_API.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

//"https://open.api.nexon.com/maplestory/v1";

Maple_API::Maple_API(const std::string& API_Key) : API_Key(API_Key){}

// API에 OCID 요청
const std::string& Maple_API::Get_OCID(const std::string& Character_Name) const {
    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/id"},
        cpr::Parameters{{"character_name", Character_Name}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    if(Response.status_code != 200) return "-1";

    nlohmann::json data = nlohmann::json::parse(Response.text);
    return data["ocid"].get<std::string>();
}

// 캐릭터 이미지 URL 반환
std::string Maple_API::Get_Character_Image_URL(const std::string& Character_Name) const{
    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/basic"},
        cpr::Parameters{{"ocid", Get_OCID(Character_Name)}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );
    
    if(Response.status_code != 200) return "-1";
    
    nlohmann::json data = nlohmann::json::parse(Response.text);
    return data["character_image"];
}