#include "Maple_API.h"
#include "Custom_Class.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

//"https://open.api.nexon.com/maplestory/v1";

std::string Get_Str(const nlohmann::json& Data, const std::string& Key) {
    if (!Data.contains(Key) || Data[Key].is_null()) return "";
    return Data[Key].get<std::string>();
}

int32_t Get_Int32(const nlohmann::json& Data, const std::string& Key) {
    if (!Data.contains(Key) || Data[Key].is_null()) return 0;
    return Data[Key].get<int32_t>();
}

int64_t Get_Int64(const nlohmann::json& Data, const std::string& Key) {
    if (!Data.contains(Key) || Data[Key].is_null()) return 0;
    return Data[Key].get<int64_t>();
}

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

void Maple_API::Parse_Potential_Option_Info(Potential_Option& Info, const nlohmann::json& Data) const {
    Info.Grade = Get_Str(Data, "potential_option_grade");
    Info.Option[0] = Get_Str(Data, "potential_option_1");
    Info.Option[1] = Get_Str(Data, "potential_option_2");
    Info.Option[2] = Get_Str(Data, "potential_option_3");
}

void Maple_API::Parse_Additional_Potential_Option_Info(Potential_Option& Info, const nlohmann::json& Data) const {
    Info.Additional_Grade = Get_Str(Data, "additional_potential_option_grade");
    Info.Additional_Option[0] = Get_Str(Data, "additional_potential_option_1");
    Info.Additional_Option[1] = Get_Str(Data, "additional_potential_option_2");
    Info.Additional_Option[2] = Get_Str(Data, "additional_potential_option_3");
}

void Maple_API::Parse_Option_Info(Option& Info, const nlohmann::json& Data) const {
    Info.Str = Get_Str(Data, "str");
    Info.Dex = Get_Str(Data, "dex");
    Info.Int = Get_Str(Data, "int");
    Info.Luk = Get_Str(Data, "luk");
    Info.Max_HP = Get_Str(Data, "max_hp");
    Info.Max_MP = Get_Str(Data, "max_mp");
    Info.Attack_Power = Get_Str(Data, "attack_power");
    Info.Magic_Power = Get_Str(Data, "magic_power");
    Info.Boss_Damage = Get_Str(Data, "boss_damage");
    Info.All_Stat = Get_Str(Data, "all_stat");
    Info.Damage = Get_Str(Data, "damage");
    Info.Max_HP_Rate = Get_Str(Data, "max_hp_rate");
    Info.Max_MP_Rate = Get_Str(Data, "max_mp_rate");
}

void Maple_API::Parse_Equipment_Info(std::vector<Equipment_Info>& Info, const nlohmann::json& Data) const {
    for (const auto& Current_Equipment : Data) {
        Equipment_Info Now;
        Now.Part_Name = Get_Str(Current_Equipment, "item_equipment_part");
        Now.Slot_Name = Get_Str(Current_Equipment, "item_equipment_slot");
        Now.Item_Name = Get_Str(Current_Equipment, "item_name");
        Now.Item_Icon = Get_Str(Current_Equipment, "item_icon");
        Now.Uprage_Count = Get_Str(Current_Equipment, "scroll_upgrade");
        Now.Golden_Hammer_Flag = Get_Str(Current_Equipment, "golden_hammer_flag");

        if (Now.Slot_Name == "무기") {
            Now.Soul_Name = Get_Str(Current_Equipment, "soul_name");
            Now.Soul_Option = Get_Str(Current_Equipment, "soul_option");
        }

        Now.Starforce = Get_Str(Current_Equipment, "starforce");

        if (Now.Part_Name == "반지")
            Now.Special_Ring_Level = Get_Int32(Current_Equipment, "special_ring_level");

        if (Current_Equipment.contains("item_total_option"))
            Parse_Option_Info(Now.Total_Option, Current_Equipment["item_total_option"]);
        if (Current_Equipment.contains("item_base_option"))
            Parse_Option_Info(Now.Base_Option, Current_Equipment["item_base_option"]);
        if (Current_Equipment.contains("item_additional_option"))
            Parse_Option_Info(Now.Additional_Option, Current_Equipment["item_additional_option"]);
        if (Current_Equipment.contains("item_etc_option"))
            Parse_Option_Info(Now.Exceptional_Option, Current_Equipment["item_etc_option"]);

        Now.Flag = Get_Str(Current_Equipment, "potential_option_flag");
        if (Now.Flag == "false")
            Parse_Potential_Option_Info(Now.Basic_Potential_Option, Current_Equipment);
        
        Now.Additional_Flag = Get_Str(Current_Equipment, "additional_potential_option_flag");
        if (Now.Additional_Flag == "false")
            Parse_Additional_Potential_Option_Info(Now.Additional_Potential_Option, Current_Equipment);

        Info.push_back(Now);
    }
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

    Equipment_Set.Status_Code = 200;
    return Equipment_Set;
}