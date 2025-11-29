#include <Arduino.h>

void sendCanBus();
void sendMsgBuf(uint32_t CAN_BUS_ID, int length, uint8_t *frame);

void sendIgnition();
void sendLightning();
void sendRpm();
void sendSpeed();
void sendFuel();
void sendEngineTemperature();
void sendHandbrake();
void sendAbs();
void sendAirbag();
void sendSeatbelt();
void sendTime();
void sendIndicators();