#include "Discord_BOT.h"
#include <iostream>
const dpp::snowflake Guild_ID = 408601520951656448;

void Discord_BOT::Add_Command_Guild(const dpp::slashcommand& CMD){
    BOT.guild_command_create(CMD, Guild_ID,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

void Discord_BOT::Add_Command_Global(const dpp::slashcommand& CMD){
    BOT.global_command_create(CMD,
        [this, CMD](const dpp::confirmation_callback_t& cb) {
            this->Create_Command_Log(cb, CMD);
    });
}

dpp::message Discord_BOT::Generate_Equipment_Embed(const std::vector<Equipment_Info>& Info, int page){
    dpp::component menu;
    menu.set_type(dpp::cot_selectmenu)
        .set_placeholder("장비 선택")
        .set_id("select_equipment");

    for(int i = 0;i < Info.size();i++){
        Equipment_Info Current_Equipment = Info[i];
        std::string description = "★" + Current_Equipment.Starforce;
        if(!Current_Equipment.Potential_Option_Info.Grade.empty()) description += " | " + Current_Equipment.Potential_Option_Info.Grade;
        if(!Current_Equipment.Potential_Option_Info.Additional_Grade.empty()) description += " | " + Current_Equipment.Potential_Option_Info.Additional_Grade;

        menu.add_select_option(
            dpp::select_option(Current_Equipment.Item_Name, std::to_string(i))
                .set_description(description)
        );
    }

    dpp::component row;
    row.add_component(menu);

    dpp::message msg(page ? std::to_string(page) + "번 프리셋 장비" : "현재 장착 장비");
    msg.add_component(row);
    msg.add_component(dpp::component()
        .add_component(dpp::component()
            .set_label("◀️")
            .set_id("prev_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
        .add_component(dpp::component()
            .set_label("▶️")
            .set_id("next_page")
            .set_style(dpp::cos_secondary)
            .set_type(dpp::cot_button))
    );
    
    return msg;
}

void Discord_BOT::Edit_Prev_Message(dpp::message& Msg, const dpp::snowflake& UID){
    Msg.id = Message_Info[UID].first;
    Msg.channel_id = Message_Info[UID].second;
    BOT.message_edit(Msg);
}

void Discord_BOT::Move_Page(const dpp::button_click_t& Event){
    dpp::snowflake UID = Event.command.get_issuing_user().id;
    size_t& Page = User_Page[UID];

    if(Event.custom_id == "prev_page" && Page > 0) --Page;
    if(Event.custom_id == "next_page" && Page < 3) ++Page;

    auto it = User_Equipment_Map.find(UID);
    if(it == User_Equipment_Map.end()){
        dpp::message Msg("장비 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    const auto& Equipment_Set = it->second;
    Event.reply("로딩 중 입니다...");
    dpp::message Msg = Generate_Equipment_Embed(Equipment_Set.Info[Page], Page);
    Edit_Prev_Message(Msg, UID);
    Event.delete_original_response();
}

void Discord_BOT::Show_Equipment_Detail(const dpp::select_click_t& Event){
    std::cout << "상세 정보 출력 함수 호출" << std::endl;
    const dpp::snowflake UID = Event.command.get_issuing_user().id;

    int Index = std::stoi(Event.values[0]);
    auto Equipment_It = User_Equipment_Map.find(UID);
    auto Page_It = User_Page.find(UID);
    if(Equipment_It == User_Equipment_Map.end() || Page_It == User_Page.end()){
        dpp::message Msg("장비 데이터를 찾을 수 없습니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    const std::vector<Equipment_Info>& Equipment_List = User_Equipment_Map[UID].Info[User_Page[UID]];
    if(Index < 0 || Index >= Equipment_List.size()){
        dpp::message Msg("잘못된 장비 선택입니다.");
        Edit_Prev_Message(Msg, UID);
        return;
    }

    const Equipment_Info& Equipment = Equipment_List[Index];
    dpp::message Msg;
    dpp::embed Image;
    Image.set_title(Get_Equipment_Name(Equipment))
        .set_thumbnail(Equipment.Item_Icon)
        .set_color(Get_Potential_Color(Equipment.Potential_Option_Info.Grade))
        .set_description(Get_Equipment_Detail_Message(Equipment));
        
    Msg.add_embed(Image);
    Edit_Prev_Message(Msg, UID);  
}

uint32_t Discord_BOT::Get_Potential_Color(const std::string& Potential_Grade) const{
    if(Potential_Grade == "레전드리") return 0x33FF33;
    else if(Potential_Grade == "유니크") return 0xFFFF00;
    else if(Potential_Grade == "에픽") return 0x7F00FF;
    else if(Potential_Grade == "레어") return 0x99CCFF;
    return 0xFFFFFF;
}

const std::string& Get_Map_By_Key(const std::unordered_map<std::string, std::string> Map, const std::string& Key){
    static const std::string Empty = "0";
    auto It = Map.find(Key);
    return (It != Map.end() ? It->second : Empty);
}

std::string Discord_BOT::Get_Equipment_Detail_Message(const Equipment_Info& Equipment) const{
    std::string Msg;
    Msg += "장비분류: " + Equipment.Part_Name + "\n";

    for(const auto& [Key, Name] : Option_List){
        if(Equipment.Total_Option.Map.find(Key) == Equipment.Total_Option.Map.end()) continue;
        if(Get_Map_By_Key(Equipment.Total_Option.Map, Key) == "0") continue;
        Msg += Name + ": +" + Get_Equipment_Detail_Option(Equipment, Key) + "\n";
    }

    if(Equipment.Cuttable_Count != "255"){
        Msg += "가위 사용 가능 횟수: " + Equipment.Cuttable_Count + "\n";
    }

    Msg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
    if(!Equipment.Potential_Option_Info.Grade.empty()){
        Msg += "\n" + Equipment.Potential_Option_Info.Grade + " 잠재옵션\n";
        Msg += Equipment.Potential_Option_Info.Option[0] + "\n";
        Msg += Equipment.Potential_Option_Info.Option[1] + "\n";
        if(!Equipment.Potential_Option_Info.Option[2].empty()){
            Msg += Equipment.Potential_Option_Info.Option[2] + "\n";
        }
    }

    Msg += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━";
    if(!Equipment.Potential_Option_Info.Additional_Grade.empty()){
        Msg += "\n" + Equipment.Potential_Option_Info.Additional_Grade + " 에디셔널 잠재옵션\n";
        Msg += Equipment.Potential_Option_Info.Additional_Option[0] + "\n";
        Msg += Equipment.Potential_Option_Info.Additional_Option[1] + "\n";
        if(!Equipment.Potential_Option_Info.Additional_Option[2].empty()){
            Msg += Equipment.Potential_Option_Info.Additional_Option[2] + "\n";
        }
    }

    return Msg;
}

bool Discord_BOT::Is_Starforce(const std::string& Key) const{
    
}

bool Discord_BOT::Is_Percentage(const std::string& Key) const{
    if(Key == "boss_damage" || Key == "ignore_monster_armor") return 1;
    if(Key == "all_stat" || Key == "max_hp_rate") return 1;
    return 0;
}

std::string Discord_BOT::Get_Equipment_Detail_Option(const Equipment_Info& Equipment, const std::string& Key) const{
    std::string Msg;
    Msg += Get_Map_By_Key(Equipment.Total_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";
    Msg += " (";

    Msg += Get_Map_By_Key(Equipment.Base_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";

    Msg += "+" + Get_Map_By_Key(Equipment.Additional_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";
    
    Msg += (std::stoi(Get_Map_By_Key(Equipment.Etc_Option.Map, Key)) < 0 ? "-" : "+");
    Msg += Get_Map_By_Key(Equipment.Etc_Option.Map, Key);
    if(Is_Percentage(Key)) Msg += "%";

    Msg += "+" + Get_Map_By_Key(Equipment.Starforce_Option.Map, Key) + ")";
    if(Is_Percentage(Key)) Msg += "%";
    return Msg;
}

std::string Discord_BOT::Get_Equipment_Name(const Equipment_Info& Equipment) const{
    std::string Msg;
    for(int i = 1;i <= 30;i++){
        if(i % 5 == 1 && i != 1) Msg += " ";
        if(i == 16) Msg += "\n";
        Msg += (i <= std::stoi(Equipment.Starforce) ? "★" : "☆");
    }
    Msg += "\n" + Equipment.Item_Name + (Equipment.Uprage_Count.empty() ? "" : " (+" + Equipment.Uprage_Count + ")");
    return Msg;
}