#include <iostream>
#include <Server.h>
#include <json/json.h>
#include <color_def.h>

int main() {
    std::cout << "General Network Communicate System" << std::endl;
    std::cout << YELLOW << "-> " << RESET << "Loading Configuration...." << std::endl;
    FILE *fp;
    try {
        fp = fopen("Config.json", "rb");
        if (fp == nullptr) {
            throw "ERROR! Config not found!";
        }
    }
    catch (const char *msg) {
        //std::cout << RED << "×  " << RESET << "ERROR! Config not found!" << std::endl;
        std::cerr << msg << std::endl;
        //exit(-1);
    }
    char buf[1024];
    std::string Config;
    while (!feof(fp)) {
        memset(buf, 0, sizeof(buf));
        int n = fread(buf, 1, 1024, fp);
        if (n == 0) {
            std::cout << YELLOW << "!  " << RESET << "Config Reader: Device or Resource busy." << std::endl;
            continue;
        }
        if (n < 0) {
            std::cout << RED << "×  " << RESET << "Config Reader: I/O Error!" << std::endl;
            break;
        }
        if (n > 0) {
            Config += buf;
        }
    }
    Json::Reader reader;
    Json::Value root;
    reader.parse(Config, root);
    std::cout << GREEN << "√  " << RESET << "Config Loaded." << std::endl;
    std::cout << YELLOW << "-> " << RESET << "Starting Server..." << std::endl;
    Server *srv = new Server(root["PORT"].asInt(), const_cast<char *>(root["IP"].asCString()));
    std::cout << GREEN << "√  " << RESET << "Done! Now Listening at " << const_cast<char *>(root["IP"].asCString())
              << ":"
              << root["PORT"].asInt() << std::endl;
    srv->WaitForExit();
    return 0;
}
