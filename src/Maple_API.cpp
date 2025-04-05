#include "Maple_API.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

//"https://open.api.nexon.com/maplestory/v1";

Maple_API::Maple_API(const std::string& Character_Name, const std::string& API_Key) :
    Character_Name(Character_Name), API_Key(API_Key){
    if(!Response_OCID()){
        throw std::runtime_error("OCID를 얻는 데 실패했습니다");
    }
}

// API에 OCID 요청
bool Maple_API::Response_OCID(){
    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/id"},
        cpr::Parameters{{"character_name", Character_Name}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    if(Response.status_code != 200) return 0;

    nlohmann::json data = nlohmann::json::parse(Response.text);
    OCID = data["ocid"].get<std::string>();
    return 1;
}

// OCID 반환
const std::string& Maple_API::Get_OCID() const{
    return OCID;
}

// 캐릭터 이미지 URL 반환
// 실패 시 -1 반환
std::string Maple_API::Get_Character_Image_URL() const{
    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/basic"},
        cpr::Parameters{{"ocid", Get_OCID()}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );
    
    if(Response.status_code != 200) return "-1";
    
    nlohmann::json data = nlohmann::json::parse(Response.text);
    return data["character_image"];
}