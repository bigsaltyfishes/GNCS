//
// Created by amocs on 2022/2/22.
//

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bt.h>

void Bluetooth::Scan()
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }
    len = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
                                 name, 0) < 0)
            strcpy(name, "[unknown]");
        printf("%s %s\n", addr, name);
        std::cout<<addr<<" "<<name<<std::endl;
        }

    free( ii );
    close( sock );
    }

int Bluetooth::Connect(char *_dest)
{
    if (!Status)
    {
        std::cout<<"Already Connected!"<<std::endl;
        return -1;
    }
    // Allocate a socket
    Sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( _dest, &addr.rc_bdaddr );

    // connect to server
    Status = connect(Sock, (struct sockaddr *)&addr, sizeof(addr));

    if(Status){
        printf(" failed to connect the device!\n");
        return -1;
    }
    strcpy(dest, _dest);
    return 1;
}

int Bluetooth::int Connect()
{
    Scan();
    char dst[18] = {0};
    std::cout<<"Please enter target address: ";
    std::cin>>dst;
    Connect(dst);
    return 1;
}

int Bluetooth::Recieve(std::string Output)
{
    char buf[CHAR_MAX];
    memset(buf, 0, sizeof(buf));
    int len = read(Sock, buf, sizeof(buf));
    return len;
}

void Bluetooth::Send(std::string Input)
{
    write(Sock, Input.data(), Input.length());
}

void Bluetooth::Close()
{
    close(Sock);
}