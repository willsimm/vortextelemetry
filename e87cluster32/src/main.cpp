#include <Arduino.h>

#include <esp32_can.h>
#include <globals.h>
#include <canbus.h>
#include <serial.h>
#include <esp_now.h>
#include <WiFi.h>


//MCP_CAN CAN(SPI_CS_PIN);
//bool cansuccess=false;

//read stuff
//long unsigned int rxId;
//unsigned char len = 0;
//unsigned char rxBuf[8];
//char msgString[128];                        // Array to store serial string


/*// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;*/
static unsigned long lastDataTime =0;
bool wifi = true;

typedef struct struct_message {
  bool can = false;
  uint16_t rpm =0;
  uint8_t temp=0;
  uint8_t speed=0;
  uint16_t odo=0;
  uint8_t fuel=0;
  bool obdii = false;
} struct_message;

// Create a struct_message called myData
struct_message carStatus;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  lastDataTime=millis();
  wifi=true;
  memcpy(&carStatus, incomingData, sizeof(carStatus));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("RPM: ");
  Serial.println(carStatus.rpm);
  Serial.print("Temp: ");
  Serial.println(carStatus.temp);
  Serial.println();

  g_rpm = carStatus.rpm;
  g_engine_temperature = carStatus.temp;
  g_fuel=carStatus.fuel;
  g_speed=carStatus.speed;
}

void setup()
{
    //delay(2000);
    //Serial.begin(SERIAL_BAUD_RATE);
    //Serial.setTimeout(250);
    
    /*byte b = CAN.begin(MCP_ANY, CAN_500KBPS, CAN_CLOCK);
      


    if ( b == CAN_OK)
    {
        Serial.println("MCP2515 Initialized successfully");
        cansuccess=true;

    }else {

        Serial.println(b);
    }

    CAN.setMode(MCP_NORMAL);*/


 Serial.begin(115200);
 Serial.setTimeout(250);
  Serial.println(" CAN...............INIT");
  CAN0.setCANPins(GPIO_NUM_4, GPIO_NUM_5); //config for shield v1.3+, see important note above!
  CAN0.begin(500000); // 500Kbps
  Serial.println(" CAN............500Kbps");
  //delay(2000);

 // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}

void loop()
{
    sendCanBus();
    readSerial();

    //if no wifi message in 2000ms set wifi=false for display
    if (wifi && (millis() - lastDataTime >= 2000)) {
      wifi =false;
      Serial.println("no data from Engine Bay ESP32");
    }

          


/*
    //send a message every 2000ms if there no wifi messages
    static unsigned long noDataTime;
    if ((millis() - noDataTime >= 2000) && (millis() - lastDataTime >= 2000)) {
      noDataTime += 2000;
      Serial.println("no data from Engine Bay ESP32");
    }
*/


    //if (!cansuccess){
    //Serial.println("fail");

    //}
    //g_rpm = millis();
    //g_speed = 7000/240 * millis();
    //delay(500);
    /*SH;1;1;1;1;1;1;1;3;2000;100;50;50;1;1;1;1;12;34;56;01;01;2025;*/

/*

//read
 CAN.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    if(rxId>0){
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
       sprintf(msgString, " 0x%.3lX,%1d,", rxId, len);
 
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();

    }*/




}

