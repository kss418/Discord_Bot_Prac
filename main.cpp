#include <dpp/dpp.h>
#include "Maple_API.h"
#include "Discord_BOT.h"

int main() {
    //메이플 API key 환경변수 설정하기
    std::string API_Key = std::getenv("MAPLEBOT_TOKEN");
    Maple_API Maple_API(API_Key);

    //디스코드 봇 토큰 환경변수 설정하기
    std::string Token = std::getenv("DISCORD_BOT_TOKEN");
    Discord_BOT BOT(Token, Maple_API);
    
    BOT.Run();

    return 0;
}
