#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef  unsigned int uint;
//消息提示宏定义
#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed: name existed!"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed"

#define SEARCH_USR_NO "no such people"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"

//消息类型穷举
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN =0,

    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPOND,

    ENUM_MSG_TYPE_LOGIN_REQUEST,
    ENUM_MSG_TYPE_LOGIN_RESPOND,

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,   //在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,  //在线用户回复

    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,  //搜索用户
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,  //搜索用户回复

    ENUM_MSG_TYPE_MAX =0x00ffffff
};
struct PDU
{
    uint uiPDULen;
    uint uiMsgType;
    char caData[64];
    uint uiMsgLen;
    int   caMsg[];
};
PDU *mkPDU(uint uiMsgLen);
#endif // PROTOCOL_H
