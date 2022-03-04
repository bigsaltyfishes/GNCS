//
// Created by amocs on 2022/3/4.
//

#include <Command.h>
#include <Server.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <json/json.h>

int Server::s;
struct sockaddr_in Server::serverAddr;
socklen_t Server::len;
std::list<Client> Server::List;


Server::Server(int PORT, char *IP) {
    Server::s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    if (bind(Server::s, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(Server::s, 20) == -1) {
        perror("listen");
        exit(1);
    }
    len = sizeof(serverAddr);
    std::thread t(getConn);
    t.detach();
    //std::thread t1(sendMess);
    //t1.detach();
    std::thread t2(getData);
    t2.detach();
}

void Server::getConn() {
    while (1) {
        Client conn(accept(s, (struct sockaddr *) &serverAddr, &len), List.size() + 1);//第二个参数保存客户端套接字对应的IP地址和port 端口信息
        List.push_back(conn);
        printf("%d\n", conn.getConnection());
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
            FD_SET((*it).getConnection(), &rfds);
            if (maxfd < (*it).getConnection()) {
                maxfd = (*it).getConnection();
            }
            retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);//实现非阻塞式的通信，即需要等待时间的发生，一旦执行一定返回，返回的结果不同以表示函数执行的结果
            if (retval == -1) {
                printf("select error\n");
            } else if (retval == 0) {
            } else {
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                long len = recv((*it).getConnection(), buf, sizeof(buf), 0);
                if (len == 0 && retval == 1) {
                    std::cerr << (*it).getName() << " has disconnected" << std::endl;
                    List.erase(it);
                    continue;
                }
                // 对接收的 Json 数据进行解析
                Json::Reader reader;
                Json::Value root;
                try { //截断默认的异常处理，防止程序因非JSON或异常JSON数据退出
                    if (reader.parse(buf, root)) {
                        if (root["cliName"].asString().empty()) {
                            throw std::exception();
                        }
                        if (((*it).getType() == 0) && ((*it).getName().empty())) {
                            (*it).setProperties(root["Type"].as<int>(),
                                                const_cast<char *>(root["cliName"].asCString()));
                        }
                        Command cmd(root["Command"].asString());
                        cmd.Exec(root["Content"].asString());
                    } else
                        throw std::exception();
                }
                catch (std::exception e) {
                    std::cout << "Received unknown content from " << (*it).getName() << " :" << buf << std::endl;
                }
            }
        }
        sleep(1);

    }
}

void Server::BroadcastMessage() {
    while (1) {
        char buf[1024];
        fgets(buf, sizeof(buf), stdin);//从文件流读取一行，送到缓冲区，使用时注意以下几点
        std::list<Client>::iterator it;
        for (it = List.begin(); it != List.end(); ++it) {
            send((*it).getConnection(), buf, sizeof(buf), 0);
        }
    }
}

void Server::sendMsg(std::string Msg, int conn) {
    //fgets(Msg, sizeof(*Msg), stdin);//从文件流读取一行，送到缓冲区
    send(conn, (const void *) Msg.c_str(), Msg.length(), 0);
}

void Server::WaitForExit() {
    std::cout << "Press Ctrl+C to exit." << std::endl;
    while (1) {};
}