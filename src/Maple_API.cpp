#include "Maple_API.h"
#include "Custom_Class.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

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

API_Result Maple_API::Get_Info(const std::string& Character_Name, const std::string& Url, const std::vector<std::string>& Param_Str) const{
    API_Result Info;
    std::string OCID = Get_OCID(Character_Name);
    if(OCID[0] == '-'){
        OCID = OCID.substr(1);
        Info.Status_Code = std::stoi(OCID);
        return Info;
    }

    cpr::Parameters Parameters{{"ocid", OCID}};
    for(auto const& Current_Parameter : Param_Str){
        auto pos = Current_Parameter.find("=");
        if(pos != std::string::npos){
            std::string Key = Current_Parameter.substr(0, pos);
            std::string Value = Current_Parameter.substr(pos + 1);
            Parameters.Add({Key, Value});
        }
    }

    cpr::Response Response = cpr::Get(
        cpr::Url{"https://open.api.nexon.com/maplestory/v1/" + Url},
        Parameters,
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << "정보 요청 응답 코드 : " << Response.status_code << std::endl;
    if(Response.status_code != 200){
        Info.Status_Code = Response.status_code;
        return Info;
    }

    nlohmann::json data = nlohmann::json::parse(Response.text);
    auto it = Parser_List.find(Url);
    Info.Data = it->second(data);
    Info.Status_Code = 200;
    return Info;
}
