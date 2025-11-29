#include <canbus.h>
#include <globals.h>
const uint32_t CAN_BUS_ID = 0x581;

uint8_t seatbeltFrame[8] = {0x40, 0x4D, 0x00, 0x28, 0xFF, 0xFF, 0xFF, 0xFF};

void sendSeatbelt()
{
    if (g_seatbelt)
    {
        seatbeltFrame[3] = 0x29;
    }
    else
    {
        seatbeltFrame[3] = 0x28;
    }

    sendMsgBuf(CAN_BUS_ID, 5, seatbeltFrame);
}