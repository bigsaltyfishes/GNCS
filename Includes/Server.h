//
// Created by amocs on 2022/2/26.
//

#ifndef GNCS_SERVER_H
#define GNCS_SERVER_H

//
// Created by amocs on 2022/2/26.
//

#include <netinet/in.h>
#include <Client.h>
#include <Command.h>

class Server {
    friend void Command::Exec(std::string Content);

public:
    Server(int PORT, char IP[15]);

    static void getConn(); //获取客户端连接信息
    static void getData(); //从客户端接收数据
    static void sendMsg(std::string Msg, int conn); //向客户端发送信息
    static void sendMsg(char *Msg, std::string target_name);

    static void sendMsg(char *Msg, char target_ip[15]);

    void WaitForExit();

private:
    static int s;
    static sockaddr_in serverAddr;
    static socklen_t len;
    static std::list<Client> List; //储存客户端信息
    static void BroadcastMessage();
};

#endif //GNCS_SERVER_H
