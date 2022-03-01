//
// Created by amocs on 2022/2/26.
//

#ifndef GNCS_COMMAND_H
#define GNCS_COMMAND_H

#include <iostream>
#include <list>

class Command
{
public:
    Command(std::string cmd);
    ~Command();
    std::string *ToString();
    std::string cmd;
    std::list<std::string*> args;
};

#endif //GNCS_COMMAND_H
