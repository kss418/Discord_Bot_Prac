#pragma once
#include "Custom_Class.h"
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

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

inline void from_json(const nlohmann::json& j, Option& o) {
    j.at("str").get_to(o.str);
    j.at("dex").get_to(o.dex);
    j.at("int").get_to(o.int_);          
    j.at("luk").get_to(o.luk);
    j.at("max_hp").get_to(o.max_hp);
    j.at("attack_power").get_to(o.attack_power);
    j.at("magic_power").get_to(o.magic_power);
    j.at("boss_damage").get_to(o.boss_damage);
    j.at("ignore_monster_armor").get_to(o.ignore_monster_armor);
    j.at("all_stat").get_to(o.all_stat);
    j.at("max_hp_rate").get_to(o.max_hp_rate);
}

inline void to_json(nlohmann::json& j, const Option& o) {
    j = nlohmann::json{
        { "str", o.str },
        { "dex", o.dex },
        { "int", o.int_ }, 
        { "luk", o.luk },
        { "max_hp", o.max_hp },
        { "attack_power", o.attack_power },
        { "magic_power", o.magic_power },
        { "boss_damage",  o.boss_damage },
        { "ignore_monster_armor", o.ignore_monster_armor },
        { "all_stat", o.all_stat },
        { "max_hp_rate", o.max_hp_rate }
    };
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Equipment_Info,
    item_equipment_part, item_equipment_slot, item_name,
    item_icon, scroll_upgrade, cuttable_count,
    golden_hammer_flag, soul_name, soul_option, starforce,
    item_base_option, item_total_option, item_add_option,
    item_starforce_option, item_exceptional_option,
    item_etc_option, potential_option_grade,
    additional_potential_option_grade,
    potential_option_1, potential_option_2, potential_option_3,
    additional_potential_option_1, additional_potential_option_2,
    additional_potential_option_3
);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Equipment_Set,
    item_equipment, item_equipment_preset_1,
    item_equipment_preset_2, item_equipment_preset_3
);

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

