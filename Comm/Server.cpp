//
// Created by amocs on 2022/2/26.
//

#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <list>
#include <arpa/inet.h>
#include <Server.h>
#include <Client.h>
#include <json/json.h>
#include <Command.h>

int Server::sock, Server::client_fd;
sockaddr_in Server::serverAddr;
socklen_t Server::len;
std::list<Client> Server::List; //储存客户端信息

void Server::getConn() {
    while (1) {
        Client conn(accept(sock, (struct sockaddr *) &serverAddr, &len), List.size() + 1);
        List.push_back(conn);
        std::cout << conn.getConnection() << std::endl;
    }
}

void Server::getData() {
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    while (1) {
        std::list<Client>::iterator it;
        for (it = List.begin(); it != List.end(); ++it) {
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = 0;
            int retval = 0;
            FD_SET(*it->getConnection(), &rfds);
            if (maxfd < *it->getConnection()) {
                maxfd = *it->getConnection();
            }
            retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);//实现非阻塞式的通信，即需要等待时间的发生，一旦执行一定返回，返回的结果不同以表示函数执行的结果
            if (retval == -1) {
                printf("select error\n");
            } else if (retval == 0) {}
            else {
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                long len = recv(*it->getConnection(), buf, sizeof(buf), 0);
                // 对接收的 Json 数据进行解析
                Json::Reader reader;
                Json::Value root;
                if (reader.parse(buf, root)) {
                    if (((*it).getType() == 0) && ((*it).getName().empty())) {
                        (*it).setProperties(root["Type"].asInt(), root["cliName"].asString());
                    }
                }
                Command cmd(root["Command"].asString());
                cmd.Exec(const_cast<char *>(root["Content"].asCString()));
            }
        }
        sleep(1);
    }
}

void Server::sendMsg(char *Msg, int target_id) {
    fgets(Msg, sizeof(*Msg), stdin);//从文件流读取一行，送到缓冲区
    std::list<Client>::iterator it;
    for (it = List.begin(); it != List.end(); ++it) {
        if ((*it).getID() == target_id) {
            send(*it->getConnection(), Msg, sizeof(*Msg), 0);
            break;
        }
    }
}

Server::Server(int PORT, char IP[15]) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = PORT;
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(sock, 20) == -1) {
        perror("listen");
        exit(1);
    }
    len = sizeof(serverAddr);
    std::thread t(getConn);
    t.detach();
    std::thread t1(getData);
    t1.detach();
}

void Server::WaitForExit() {
    std::cout << "Press Ctrl+C to exit." << std::endl;
    while (1) {};
}