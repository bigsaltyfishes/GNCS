//
// Created by amocs on 2022/2/26.
//

#ifndef GNCS_COMMAND_H
#define GNCS_COMMAND_H

#include <iostream>
#include <list>
#include <json/value.h>

class Command {
public:
    Command(std::string cmd);

    ~Command();

    std::string *ToString();

    std::string cmd;
    std::list<std::string *> args;
    static Json::Value Actions;

    void Exec(char *Content);

private:
    void CustomExecInit();

};

#endif //GNCS_COMMAND_H
