#include <dpp/dpp.h>
#include "Maple_API.h"

int main() {
    //디스코드 봇 토큰 환경변수 설정하기
    std::string Token = std::getenv("DISCORD_BOT_TOKEN");
    // dpp::cluster bot(Token);

    //메이플 API key 환경변수 설정하기
    std::string API_Key = std::getenv("MAPLEBOT_TOKEN");

    /*
    bot.on_ready([](const dpp::ready_t& event){
        std::cout << "봇이 정상적으로 작동 되었습니다" << std::endl;
    });

    bot.start(dpp::st_wait);
    */
    
    std::string Character_Name = "TRSwrD";
    Maple_API Maple_API(Character_Name, API_Key);
    std::cout << Maple_API.Get_Character_Image_URL() << std::endl;

    return 0;
}
