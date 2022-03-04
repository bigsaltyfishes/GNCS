//
// Created by amocs on 2022/2/26.
//

#include <Command.h>

Command::Command(std::string _cmd) {
    std::string *tmp = new std::string;
    for (std::string::iterator it = _cmd.begin(); it != _cmd.end(); it++) {
        if ((*it) != ' ') {
            *tmp += *it;
        } else {
            args.push_back(tmp);
            tmp = new std::string;
        }
    }
    if (args.back() != tmp)
        args.push_back(tmp);
    cmd = *args.front();
    args.pop_front();
    std::cout << cmd << std::endl;
    for (std::list<std::string *>::iterator it = args.begin(); it != args.end(); it++) {
        std::cout << **it << std::endl;
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