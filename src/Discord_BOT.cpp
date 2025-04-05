#include "Discord_BOT.h"
#include <iostream>

Discord_BOT::Discord_BOT(const std::string& API_Key, const Maple_API& M_API) : BOT(API_Key), M_API(M_API){
    Setup_Events();
}

void Discord_BOT::Run(){
    BOT.start(dpp::st_wait);
}

void Discord_BOT::Setup_Events(){
    BOT.on_ready([this](const dpp::ready_t& Event){
        std::cout << "봇이 성공적으로 켜졌습니다" << std::endl;
    });
    
    BOT.on_slashcommand([this](const dpp::slashcommand_t& Event){

    });
}
