# 通用网络通讯服务端
这是一款通用的网络通信服务器，可以实现 控制端<->服务器<->受控端 之间的双向通讯，数据采用 JSON 进行传输

服务器/客户端数据交换范例：

    {
        "cliName" : "",
        "Type"    : int,   //0: Unknown; 1: Controller; 2: Client
        "Command" : "",    //Command
        "Content" : ""     //Encoded with BASE64
    }
服务器支持进行指令的自定义，自定义命令定义于程序同一目录下的 **CustumAction.json** 中，范例如下:

    {
        0 : [{
                "cmdName": "", //Custom Command Name
                "realCmd": "" //Real Command
            }],
        1 : [{
                "cmdName": "",
                "realCmd": ""
            }],
        ...
    }
目前程序预置的指令仅有 **sendTo \<Target Name\>**，对于带有空格的指令参数，需要将该参数放到 **""** 内

## 目前程序的主要功能的开发情况
- [x] 基本的TCP/IP通讯功能
- [x] 对JSON的解析功能
- [x] 对指令的分解识别功能
- [ ] 文件的收发功能
- [ ] 数据的封装功能
- [ ] 文件的压缩功能
- [ ] 哈希校验功能

# 外部库使用

[JsonCpp][1]

[base64.c][2]

[1]: https://github.com/open-source-parsers/jsoncpp.git
[2]: https://github.com/joedf/base64.c
