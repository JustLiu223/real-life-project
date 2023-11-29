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
    uint uiPDULen;
    uint uiMsgType;
    char caData[64];
    uint uiMsgLen;
    int   caMsg[];
};
PDU *mkPDU(uint uiMsgLen);
#endif // PROTOCOL_H
