#include "Discord_BOT.h"
#include <iostream>

std::string Discord_BOT::Get_Equipment_Detail_Option(const Equipment_Info& Equipment, const std::string& Key){
    std::string Msg;
    Msg += Get_Option_Value(Equipment.item_total_option, Key);
    if(Is_Percentage(Key)) Msg += "%";

    if(Get_Option_Value(Equipment.item_total_option, Key) == Get_Option_Value(Equipment.item_base_option, Key)){
        return Msg;
    }
    
    Msg += " (";
    Msg += Get_Option_Value(Equipment.item_base_option, Key);
    if(Is_Percentage(Key)) Msg += "%";

    if(Is_Additional_Option(Equipment)){
        Msg += "+" + Get_Option_Value(Equipment.item_add_option, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }
    
    if(Is_Scroll(Equipment)){
        Msg += (std::stoi(Get_Option_Value(Equipment.item_etc_option, Key)) < 0 ? "-" : "+");
        Msg += Get_Option_Value(Equipment.item_etc_option, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }

    if(Is_Starforce(Equipment)){
        Msg += "+" + Get_Option_Value(Equipment.item_starforce_option, Key);
        if(Is_Percentage(Key)) Msg += "%";
    }
    Msg += ")";
    return Msg;
}

dpp::message Discord_BOT::Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page){
    dpp::component menu;
    menu.set_type(dpp::cot_selectmenu)
        .set_placeholder("장비 선택")
        .set_id("select_equipment");

    for(int i = 0;i < Info.size();i++){
        Equipment_Info Current_Equipment = Info[i];
        std::string description = (Is_Starforce(Current_Equipment) ? "★" + Current_Equipment.starforce + " | " : "");
        if(Current_Equipment.potential_option_grade != "0") description += Current_Equipment.potential_option_grade;
        if(Current_Equipment.additional_potential_option_grade != "0") description += " | " + Current_Equipment.additional_potential_option_grade;
        if(Current_Equipment.special_ring_level && Current_Equipment.item_equipment_part == "반지"){
            description += " Lv." + std::to_string(Current_Equipment.special_ring_level);
        }

        menu.add_select_option(
            dpp::select_option(Current_Equipment.item_name, std::to_string(i))
                .set_description(description)
        );
    }

    dpp::component row;
    row.add_component(menu);

    dpp::message msg(page ? std::to_string(page) + "번 프리셋 장비" : "현재 장착 장비");
    msg.add_component(row);
    msg.add_component(Create_Move_Page_Component("equipment"));
    
    return msg;
}

dpp::message Discord_BOT::Generate_Hexa_Skill_Embed(const Character_Skill& Skill, int Page){
    std::vector <Character_Skill::Skill_Info> Skill_List = Skill.character_skill;
    dpp::message Msg;
    dpp::embed Embed[4], Main_Embed;

    int32_t Start = Page * Skill_Per_Page;
    Main_Embed.set_title("헥사 스킬 목록\n" + std::to_string(Page + 1) + " / " + 
        std::to_string((Skill_List.size() - 1) / Skill_Per_Page + 1) + "페이지")
        .set_color(0xFFFF);
    
    for(int i = 0;i < Skill_Per_Page && Start + i < Skill_List.size();i++){
        const Character_Skill::Skill_Info& Skill = Skill_List[Start + i];
        Embed[i].set_thumbnail(Skill.skill_icon);
        Embed[i].add_field(Skill.skill_name, std::to_string(Skill.skill_level) + "레벨", false);
    }
    Msg.add_embed(Main_Embed);
    for(int i = 0;i < Skill_Per_Page;i++) Msg.add_embed(Embed[i]);

    Msg.add_component(Create_Move_Page_Component("hexa_skill"));
    return Msg;
}

dpp::message Discord_BOT::Generate_Hexa_Stat_Embed(const Hexa_Stat& Stat, int page){
    std::vector <std::vector<Hexa_Stat::Core>> Core_List;
    Core_List.push_back(Stat.character_hexa_stat_core);
    Core_List.push_back(Stat.character_hexa_stat_core_2);
    Core_List.push_back(Stat.character_hexa_stat_core_3);
    const Hexa_Stat::Core& Current_Core = Core_List[page][0];

    dpp::message Msg;
    dpp::embed Embed;
    Embed.set_title(std::to_string(page + 1) + "번 헥사 스탯\n" + 
        std::to_string(page + 1) + " / " + std::to_string(Hexa_Core_Count) + "페이지")
        .set_color(0x00CCFF)                                       
        .add_field("메인 레벨", std::to_string(Current_Core.main_stat_level), true)
        .add_field("스탯", Current_Core.main_stat_name, true)
        .add_field("\u200B", "\u200B", true)
        .add_field("에디셔널1 레벨", std::to_string(Current_Core.sub_stat_level_1), true)
        .add_field("스탯", Current_Core.sub_stat_name_1, true)
        .add_field("\u200B", "\u200B", true)
        .add_field("에디셔널2 레벨", std::to_string(Current_Core.sub_stat_level_2), true)
        .add_field("스탯", Current_Core.sub_stat_name_2, true)
        .add_field("\u200B", "\u200B", true);

    Msg.add_embed(Embed);
    Msg.add_component(Create_Move_Page_Component("hexa_stat"));

    return Msg;
}

dpp::message Discord_BOT::Generate_Symbol_Embed(const Symbol& Symbol, int page){
    dpp::message Msg;
    dpp::embed Embed[6], Main_Embed;
    std::vector<std::string> Symbol_Name = {"아케인", "어센틱", "그랜드 어센틱"};
    Main_Embed.set_title(Symbol_Name[page] + "심볼\n" + 
        std::to_string(page + 1) + " / " + std::to_string(Symbol.symbol.size() / 6 + (Symbol.symbol.size() % 6 ? 1 : 0)) + "페이지")
        .set_color(0x00CCFF);

    const auto& Symbol_Info = Symbol.symbol;
    for(size_t i = 0;i < 6;i++){
        if(i + 6 * page >= Symbol_Info.size()) break;
        const auto& Current_Symbol = Symbol_Info[i + 6 * page];
        Embed[i].set_thumbnail(Current_Symbol.symbol_icon);
        Embed[i].add_field(Current_Symbol.symbol_name, 
            std::to_string(Current_Symbol.symbol_level) + "레벨" + Get_Symbol_Progress(Current_Symbol), true);
        Embed[i].set_color(0x00CCFF);
    }

    Msg.add_embed(Main_Embed);
    for(size_t i = 0;i < 6;i++){
        if(i + 6 * page >= Symbol_Info.size()) break;
        Msg.add_embed(Embed[i]);
    }

    Msg.add_component(Create_Move_Page_Component("symbol"));
    return Msg;
}