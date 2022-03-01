//
// Created by amocs on 2022/2/26.
//
#include <iostream>
#ifndef GNCS_CLIENT_TYPE_H
#define GNCS_CLIENT_TYPE_H

#define Unknown 0
#define Controller 1
#define Reciver 2

class Client
{
public:
    Client(int _Type, int _Connection, int _ID):Type(_Type), Connection(_Connection), ID(_ID) {};
    Client(int _Connection, int _ID):Type(Unknown), Connection(_Connection), ID(_ID) {};
    bool getType()
    {
        return Type;
    }
    int getID()
    {
        return ID;
    }
    const std::string& getName()
    {
        return Name;
    }
    int *getConnection()
    {
        return &Connection;
    }
    Client& operator = (int _Type)
    {
        Type = _Type;
    }
    void setProperties(int _Type, std::string _Name)
    {
        Type = _Type;
        Name = _Name;
    }
private:
    int Type;
    int ID;
    std::string Name;
    int Connection;
};

#endif //GNCS_CLIENT_TYPE_H
