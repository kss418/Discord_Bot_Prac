#include <iostream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <string>
#include <cstdlib>

int main() {
    //메이플 API key 환경변수 설정하기
    std::string API_Key = std::getenv("MAPLEBOT_TOKEN");
    std::string URL = "https://open.api.nexon.com/maplestory/v1";
    std::string Character_Name = "TRSwrD";
  
    cpr::Response Response = cpr::Get(
        cpr::Url{URL + "/id?character_name=" + Character_Name},
        cpr::Header{{"x-nxopen-api-key", API_Key}, {"Accept", "application/json"}}
    );

    std::cout << Response.status_code << std::endl;
    if(Response.status_code == 200){
        nlohmann::json parsed_json = nlohmann::json::parse(Response.text);
        std::cout << parsed_json["ocid"] << std::endl;
    }

    int n; std::cin >> n;
    return 0;
}
