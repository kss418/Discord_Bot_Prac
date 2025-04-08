#include "Maple_API.h"
#include "Custom_Class.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

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

void Maple_API::Parse_Potential_Option_Info(Potential_Option& Info, const nlohmann::json& Data) const {
    Info.Grade = Get_Str(Data, "potential_option_grade");
    Info.Option[0] = Get_Str(Data, "potential_option_1");
    Info.Option[1] = Get_Str(Data, "potential_option_2");
    Info.Option[2] = Get_Str(Data, "potential_option_3");

    Info.Additional_Grade = Get_Str(Data, "additional_potential_option_grade");
    Info.Additional_Option[0] = Get_Str(Data, "additional_potential_option_1");
    Info.Additional_Option[1] = Get_Str(Data, "additional_potential_option_2");
    Info.Additional_Option[2] = Get_Str(Data, "additional_potential_option_3");
}

void Maple_API::Parse_Option_Info(Option& Info, const nlohmann::json& Data) const {
    for(auto it = Data.begin();it != Data.end();it++){
        Info.Map[it.key()] = it.value().get<std::string>();
    }
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
        Now.Cuttable_Count = Get_Str(Current_Equipment, "cuttable_count");

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
        if (Current_Equipment.contains("item_add_option"))
            Parse_Option_Info(Now.Additional_Option, Current_Equipment["item_add_option"]);
        if (Current_Equipment.contains("item_etc_option"))
            Parse_Option_Info(Now.Etc_Option, Current_Equipment["item_etc_option"]);
        if (Current_Equipment.contains("item_exceptional_option"))
            Parse_Option_Info(Now.Exceptional_Option, Current_Equipment["item_exceptional_option"]);
            if (Current_Equipment.contains("item_starforce_option"))
            Parse_Option_Info(Now.Starforce_Option, Current_Equipment["item_starforce_option"]);
        
        Parse_Potential_Option_Info(Now.Potential_Option_Info, Current_Equipment);

        Info.push_back(Now);
    }
}