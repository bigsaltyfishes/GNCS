//
// Created by amocs on 2022/2/26.
//

#include <Command.h>
#include <Server.h>
#include <list>
#include <json/json.h>

Json::Value Command::Actions;

void Command::CustomExecInit() {
    Json::Reader reader;
    FILE *fp;
    try {
        fp = fopen("CustomAction.json", "rb");
        if (fp == NULL) {
            throw std::exception();
        }
        std::string Action;
        char buf[1024];
        while (!feof(fp)) {
            memset(buf, 0, sizeof(buf));
            int n = fread(buf, 1, 1024, fp);
            if (n == 0) {
                std::cout << "Config Reader: Device or Resouce busy." << std::endl;
                continue;
            }
            if (n < 0) {
                std::cerr << "Config Reader: I/O Error!" << std::endl;
                break;
            }
            if (n > 0) {
                Action += buf;
            }
        }
        try {
            reader.parse(Action, Actions);
        }
        catch (std::exception e) {
            std::cerr << "ERROR! Incorrect Custom Action definition file!" << std::endl;
        }
        fclose(fp);
    }
    catch (std::exception e) {
        std::cerr << "WARNING! CustomAction definition not found!" << std::endl;
    }
}

void Command::Exec(std::string Content) {
    if (cmd == "sendTo") //内置指令
    {
        for (std::list<Client>::iterator it = Server::List.begin(); it != Server::List.end(); it++) {
            if ((*it).getName() == *args.back()) {
                std::cout << "Sending Message to " << (*it).getName() << std::endl;
                Server::sendMsg(Content, (*it).getConnection());
            }
        }
    } else {
        if (Actions.empty()) CustomExecInit();
        for (int i = 0; i < Actions.size(); i++) {
            if (cmd == Actions[i]["cmdName"].asString()) {
                Command *cmd = new Command(Actions[i]["realCmd"].asString());
                cmd->Exec(Content);
            }
        }
    }
}

