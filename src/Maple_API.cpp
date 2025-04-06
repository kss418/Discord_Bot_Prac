#include "Maple_API.h"
#include "Custom_Class.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

//"https://open.api.nexon.com/maplestory/v1";

Maple_API::Maple_API(const std::string& API_Key) : API_Key(API_Key){}

// API에 OCID 요청
const std::string Maple_API::Get_OCID(const std::string& Character_Name) const {
    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/id"},
        cpr::Parameters{{"character_name", Character_Name}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "OCID 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200) return "-" + std::to_string(Response.status_code);

    nlohmann::json data = nlohmann::json::parse(Response.text);
    return data["ocid"].get<std::string>();
}

// 캐릭터 정보 반환
Character Maple_API::Get_Character_Info(const std::string& Character_Name) const{
    Character Character_Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Character_Info.Status_Code = std::stoi(OCID);
        return Character_Info;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/basic"},
        cpr::Parameters{{"ocid", Get_OCID(Character_Name)}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );
    
    std::cout << "캐릭터 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Character_Info.Status_Code = Response.status_code;
        return Character_Info;
    }
    
    nlohmann::json data = nlohmann::json::parse(Response.text);
    Character_Info.Image_URL = data["character_image"];
    Character_Info.World_Name = data["world_name"];
    Character_Info.Class = data["character_class"];
    Character_Info.Level = data["character_level"];
    Character_Info.EXP_Rate = data["character_exp_rate"];
    Character_Info.Guild_Name = data["character_guild_name"].is_null() ? "" : data["character_guild_name"];
    Character_Info.Status_Code = 200;

    std::cout << "이미지 URL : " << data["character_image"] << std::endl;

    return Character_Info;
}

// 유니온 정보 반환
Union Maple_API::Get_Union_Info(const std::string& Character_Name) const{
    Union Union_Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Union_Info.Status_Code = std::stoi(OCID);
        return Union_Info;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/user/union"},
        cpr::Parameters{{"ocid", Get_OCID(Character_Name)}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );
    
    std::cout << "유니온 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Union_Info.Status_Code = Response.status_code;
        return Union_Info;
    }
    
    nlohmann::json data = nlohmann::json::parse(Response.text);
    Union_Info.Union_Level = data["union_level"];
    Union_Info.Union_Grade = data["union_grade"];
    Union_Info.Artifact_Level = data["union_artifact_level"];
    Union_Info.Status_Code = 200;

    std::cout << "유니온 레벨 : " << data["union_level"] << std::endl;

    return Union_Info;
}