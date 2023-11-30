#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)      //创建pdu对象
{
    uint uiPDULen = sizeof(PDU)+uiMsgLen;
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(NULL == pdu)
    {
        exit(EXIT_FAILURE);            //申请失败退出
    }
    memset(pdu,0,uiPDULen);      //内存清零
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;
    return pdu;
}
