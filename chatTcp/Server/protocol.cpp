#include "protocol.h"

PDU *mkPDU(uint uiMsgLen)            //创建pdu对象用来发送数据
{
    uint uiPDULen = sizeof(PDU)+uiMsgLen;
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(NULL == pdu)
    {
        exit(EXIT_FAILURE);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;
    return pdu;
}
