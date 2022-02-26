//
// Created by amocs on 2022/2/22.
//

#ifndef GNCS_BT_H
#define GNCS_BT_H

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

class Bluetooth
{
public:
    Bluetooth(char *_dest)
    {
        Connect(_dest);
    }
    Bluetooth()
    {
        Connect();
    }
    void Scan();
    int Connect(char *_dest);
    int Connect();
    int Recieve(std::string Output);
    void Send(std::string Input);
    void Close();

private:
    char dest[18] = {0};
    sockaddr_rc addr =  {0};
    int Sock, len = 0;
    bool Status = False;

};

#endif //GNCS_BT_H
