#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
typedef  unsigned int uint;

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed: name existed!"
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN =0,
    ENUM_MSG_TYPE_REGIST_REQUEST,
    ENUM_MSG_TYPE_REGIST_RESPOND,
    // ENUM_MSG_TYPE_REQUEST,
    // ENUM_MSG_TYPE_RESPOND,
    // ENUM_MSG_TYPE_REQUEST,
    // ENUM_MSG_TYPE_RESPOND,
    // ENUM_MSG_TYPE_REQUEST,
    // ENUM_MSG_TYPE_RESPOND,
    ENUM_MSG_TYPE_MAX =0x00ffffff
};

struct PDU
{
    uint uiPDULen;           //协议总大小
    uint uiMsgType;         //消息类型
    char caData[64];         //文件大小
    uint uiMsgLen;            //消息长度
    int   caMsg[];               //实际消息内容
};
PDU *mkPDU(uint uiMsgLen);    //申请内存
#endif // PROTOCOL_H
