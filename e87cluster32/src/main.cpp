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


typedef struct struct_message {
    int rpm;
    int temp;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  /*Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("Bool: ");
  Serial.println(myData.d);
  Serial.println();*/
  Serial.print("RPM: ");
  Serial.println(myData.rpm);
  Serial.print("Temp: ");
  Serial.println(myData.temp);
  Serial.println();
  
  g_rpm = myData.rpm;
  g_engine_temperature = myData.temp;
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

