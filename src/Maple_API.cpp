#include "Maple_API.h"
#include "Custom_Class.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character_Skill::Skill_Info,
    skill_name, skill_level, skill_description, skill_icon
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character_Skill,
    character_class, character_skill_grade, character_skill
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Hexa_Stat::Core,
    slot_id, main_stat_name, sub_stat_name_1, sub_stat_name_2,
    main_stat_level, sub_stat_level_1, sub_stat_level_2, stat_grade
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Hexa_Stat,
    character_hexa_stat_core,
    character_hexa_stat_core_2,
    character_hexa_stat_core_3
);

//"https://open.api.nexon.com/maplestory/v1";
Maple_API::Maple_API(const std::string& API_Key) : API_Key(API_Key){}

// API에 OCID 요청
const std::string Maple_API::Get_OCID(const std::string& Character_Name) const {
    if(OCID_Map.find(Character_Name) != OCID_Map.end()){
        return OCID_Map[Character_Name];
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/id"},
        cpr::Parameters{{"character_name", Character_Name}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "OCID 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200) return "-" + std::to_string(Response.status_code);

    nlohmann::json data = nlohmann::json::parse(Response.text);
    OCID_Map[Character_Name] = data["ocid"].get<std::string>();
    return OCID_Map[Character_Name];
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
        cpr::Parameters{{"ocid", OCID}},
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
        cpr::Parameters{{"ocid", OCID}},
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

// 인기도 정보 반환
Popularity Maple_API::Get_Popularity_Info(const std::string& Character_Name) const{
    Popularity Popularity_Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Popularity_Info.Status_Code = std::stoi(OCID);
        return Popularity_Info;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/popularity"},
        cpr::Parameters{{"ocid", OCID}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "인기도 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Popularity_Info.Status_Code = Response.status_code;
        return Popularity_Info;
    }

    nlohmann::json data = nlohmann::json::parse(Response.text);
    Popularity_Info.Popularity = data["popularity"];
    Popularity_Info.Status_Code = 200;
    return Popularity_Info;
}

// 아이템 정보 반환
Equipment_Set Maple_API::Get_Equipment_Info(const std::string& Character_Name) const{
    Equipment_Set Equipment_Set;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Equipment_Set.Status_Code = std::stoi(OCID);
        return Equipment_Set;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/item-equipment"},
        cpr::Parameters{{"ocid", OCID}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "장비 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Equipment_Set.Status_Code = Response.status_code;
        return Equipment_Set;
    }

    nlohmann::json data = nlohmann::json::parse(Response.text);
    Parse_Equipment_Info(Equipment_Set.Info[0], data["item_equipment"]);
    Parse_Equipment_Info(Equipment_Set.Info[1], data["item_equipment_preset_1"]);
    Parse_Equipment_Info(Equipment_Set.Info[2], data["item_equipment_preset_2"]);
    Parse_Equipment_Info(Equipment_Set.Info[3], data["item_equipment_preset_3"]);

    Equipment_Set.Status_Code = 200;
    return Equipment_Set;
}

Character_Skill Maple_API::Get_Hexa_Skill_Info(const std::string& Character_Name) const{
    Character_Skill Skill_Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Skill_Info.Status_Code = std::stoi(OCID);
        return Skill_Info;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/skill"},
        cpr::Parameters{{"ocid", OCID}, {"character_skill_grade", "6"}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "헥사 스킬 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Skill_Info.Status_Code = Response.status_code;
        return Skill_Info;
    }

    nlohmann::json data = nlohmann::json::parse(Response.text);
    Skill_Info = data.get<Character_Skill>();
    Skill_Info.Status_Code = 200;
    return Skill_Info;
}

Hexa_Stat Maple_API::Get_Hexa_Stat_Info(const std::string& Character_Name) const{
    Hexa_Stat Stat_Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Stat_Info.Status_Code = std::stoi(OCID);
        return Stat_Info;
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/character/hexamatrix-stat"},
        cpr::Parameters{{"ocid", OCID}},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "헥사 스탯 정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Stat_Info.Status_Code = Response.status_code;
        return Stat_Info;
    }

    nlohmann::json data = nlohmann::json::parse(Response.text);
    Stat_Info = data.get<Hexa_Stat>();
    Stat_Info.Status_Code = 200;
    return Stat_Info;
}