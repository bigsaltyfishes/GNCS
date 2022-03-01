//
// Created by amocs on 2022/2/26.
//

#include <Command.h>

Command::Command(std::string cmd) {
    std::string *tmp = new std::string;
    for (std::string::iterator it = cmd.begin(); it != cmd.end(); it++) {
        if (cmd.empty()) {
            while (*it++ != ' ') {
                cmd += *it;
            }
        }
        if (*it == '\"') {
            it++;
            while (*it != '\"') {
                *tmp += *it;
                it++;
            }
            args.push_back(tmp);
            tmp = new std::string;
            it++;
        }
        if (*it == ' ') {
            it++;
            while (*it != ' ') {
                *tmp += *it;
                it++;
            }
            args.push_back(tmp);
            tmp = new std::string;
        }
    }
    if (cmd.empty()) {
        delete tmp;
    }
}

Command::~Command() {
    for (std::list<std::string *>::iterator it = args.begin(); it != args.end(); it++) {
        delete *it;
    }
}

std::string *Command::ToString() {
    std::string *full_cmd = new std::string;
    *full_cmd += cmd;
    for (std::list<std::string *>::iterator it = args.begin(); it != args.end(); it++) {
        *full_cmd += ' ';
        *full_cmd += **it;
    }
    return full_cmd;
}