#include <canbus.h>
#include <globals.h>

uint32_t previous = 0;
uint32_t counter = 0;


void sendMsgBuf(uint32_t CAN_BUS_ID, int length, uint8_t *frame)
{

        CAN_FRAME txFrame;
        txFrame.rtr = 0;
        txFrame.id = CAN_BUS_ID;
        txFrame.extended = false;
        txFrame.length = length;
        //txFrame.data = frame;

        for(int i =0 ;i<length;i++   ){
              txFrame.data.uint8[i] = frame[i];
        }
        CAN0.sendFrame(txFrame);

}



void sendCanBus()
{
    uint32_t current = millis();
    if (current - previous >= 10)
    {
        sendIgnition();
        sendLightning();

        if (counter % 7 == 0)
        {
            sendSpeed();
        }
        
        if (counter % 20 == 0)
        {
            sendRpm();
            sendAbs();
            sendAirbag();
            sendEngineTemperature();
            sendFuel();
            sendHandbrake();
            sendSeatbelt();
            sendIndicators();
        }

        if (counter % 100 == 0)
        {
            sendTime();
        }

        counter++;
        previous = current;
    }
}
