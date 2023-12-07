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

#define USR_NO "no  user"
#define USR_ONLINE "user online"
#define USR_OFFLINE "user offline"
#define UNKNOW_ERROR "unknow error"
#define USR_EXISTED "uer existed"

#define DEL_FRIEND_OK "delete friend ok"
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

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,  //添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,  //添加好友响应

    ENUM_MSG_TYPE_ADD_FRIEND_AGGREE,
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,  //刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,  //刷新好友响应

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,  //删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,  //删除好友响应

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,  //私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,  //私聊回复
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
