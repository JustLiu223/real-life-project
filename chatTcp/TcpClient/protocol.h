#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef  unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed: name existed!"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed"

#define SEARCH_USR_NO "no such people"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN =0,
    ENUM_MSG_TYPE_REGIST_REQUEST,  //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,  //注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,   //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,   //登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,   //在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,  //在线用户回复
    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,  //搜索用户
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,  //搜索用户回复
    ENUM_MSG_TYPE_MAX =0x00ffffff
};

struct PDU
{
    uint uiPDULen;           //协议总大小
    uint uiMsgType;         //消息类型
    char caData[64];         //基本数据
    uint uiMsgLen;            //消息长度
    int   caMsg[];               //实际消息内容
};
PDU *mkPDU(uint uiMsgLen);    //申请内存
#endif // PROTOCOL_H
