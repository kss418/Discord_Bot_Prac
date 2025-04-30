#pragma once
#include "Custom_Class.h"
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>
#include <optional>

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Symbol::Info,
    symbol_name, symbol_icon,
    symbol_level, symbol_growth_count,
    symbol_require_growth_count
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Symbol,
    symbol
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Popularity,
    popularity
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Union,
    union_level, union_grade, union_artifact_level
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character,
    character_image, world_name, character_level,
    character_exp_rate, character_guild_name, character_class
);

#define JSON_SAFE_FIELD(j,obj,field,default_val)         \
  do {                                                   \
    auto it = (j).find(#field);                         \
    if (it != (j).end() && !it->is_null())              \
      it->get_to((obj).field);                          \
    else                                                 \
      (obj).field = (default_val);                      \
  } while(0)

inline void from_json(const nlohmann::json& j, Option& o) {
    JSON_SAFE_FIELD(j, o, str, "0");
    JSON_SAFE_FIELD(j, o, dex, "0");
    JSON_SAFE_FIELD(j, o, int_, "0");
    JSON_SAFE_FIELD(j, o, luk, "0");
    JSON_SAFE_FIELD(j, o, max_hp, "0");
    JSON_SAFE_FIELD(j, o, attack_power, "0");
    JSON_SAFE_FIELD(j, o, magic_power, "0");
    JSON_SAFE_FIELD(j, o, boss_damage, "0");
    JSON_SAFE_FIELD(j, o, ignore_monster_armor, "0");
    JSON_SAFE_FIELD(j, o, all_stat, "0");
    JSON_SAFE_FIELD(j, o, max_hp_rate, "0");
  }

inline void from_json(const nlohmann::json& j, Equipment_Info& e) {
    JSON_SAFE_FIELD(j, e, item_equipment_part, std::string{"0"});
    JSON_SAFE_FIELD(j, e, item_equipment_slot, std::string{"0"});
    JSON_SAFE_FIELD(j, e, item_name, std::string{"0"});
    JSON_SAFE_FIELD(j, e, item_icon, std::string{"0"});
    JSON_SAFE_FIELD(j, e, scroll_upgrade, std::string{"0"});
    JSON_SAFE_FIELD(j, e, cuttable_count, std::string{"0"});
    JSON_SAFE_FIELD(j, e, golden_hammer_flag, std::string{"0"});
    JSON_SAFE_FIELD(j, e, soul_name, std::string{"0"});
    JSON_SAFE_FIELD(j, e, soul_option, std::string{"0"});
    JSON_SAFE_FIELD(j, e, starforce, std::string{"0"});
  
    JSON_SAFE_FIELD(j, e, item_base_option, Option{});
    JSON_SAFE_FIELD(j, e, item_total_option, Option{});
    JSON_SAFE_FIELD(j, e, item_add_option, Option{});
    JSON_SAFE_FIELD(j, e, item_starforce_option, Option{});
    JSON_SAFE_FIELD(j, e, item_exceptional_option, Option{});
    JSON_SAFE_FIELD(j, e, item_etc_option, Option{});
  
    JSON_SAFE_FIELD(j, e, potential_option_grade, std::string{"0"});
    JSON_SAFE_FIELD(j, e, additional_potential_option_grade, std::string{"0"});
    JSON_SAFE_FIELD(j, e, potential_option_1, std::string{"0"});
    JSON_SAFE_FIELD(j, e, potential_option_2, std::string{"0"});
    JSON_SAFE_FIELD(j, e, potential_option_3, std::string{"0"});
    JSON_SAFE_FIELD(j, e, additional_potential_option_1, std::string{"0"});
    JSON_SAFE_FIELD(j, e, additional_potential_option_2, std::string{"0"});
    JSON_SAFE_FIELD(j, e, additional_potential_option_3, std::string{"0"});
  
    JSON_SAFE_FIELD(j, e, special_ring_level, int64_t{0});
  }

inline void from_json(const nlohmann::json& j, Equipment_Set& es) {
    JSON_SAFE_FIELD(j, es, item_equipment, std::vector<Equipment_Info>{});
    JSON_SAFE_FIELD(j, es, item_equipment_preset_1,std::vector<Equipment_Info>{});
    JSON_SAFE_FIELD(j, es, item_equipment_preset_2,std::vector<Equipment_Info>{});
    JSON_SAFE_FIELD(j, es, item_equipment_preset_3,std::vector<Equipment_Info>{});
}

class Maple_API{
private:
    std::string API_Key;
    mutable std::unordered_map <std::string, std::string> OCID_Map;

    using ParserFn = std::function<API_Type(const nlohmann::json&)>;
    const std::unordered_map<std::string, ParserFn> Parser_List = {
        { "character/skill", [this](auto& j){ return Parse_Json<Character_Skill>(j); }},
        { "character/hexamatrix-stat", [this](auto& j){ return Parse_Json<Hexa_Stat>(j); }},
        { "character/symbol-equipment", [this](auto& j){ return Parse_Json<Symbol>(j); }},
        { "character/item-equipment", [this](auto& j){ return Parse_Json<Equipment_Set>(j); }},
        { "character/popularity", [this](auto& j){ return Parse_Json<Popularity>(j); }},
        { "user/union", [this](auto& j){ return Parse_Json<Union>(j); }},
        { "character/basic", [this](auto& j){ return Parse_Json<Character>(j); }}
    };

public:
    Maple_API(const std::string& API_Key);

    const std::string Get_OCID(const std::string& Character_Name) const;
    API_Result Get_Info(const std::string& Character_Name, const std::string& Url, const std::vector<std::string>& Parameters = {}) const;

    void Parse_Equipment_Info(std::vector <Equipment_Info>& Info, const nlohmann::json& data) const;
    void Parse_Option_Info(Option& Info, const nlohmann::json& data) const;

    template <typename T>
    T Parse_Json(const nlohmann::json& Json) const;
};

template <typename T>
T Maple_API::Parse_Json(const nlohmann::json& Json) const{
    return Json.get<T>();
}

