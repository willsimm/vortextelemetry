#include <esp32_can.h> /* https://github.com/collin80/esp32_can */
//#include <esp32_obd2.h>


#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



#define  SHIELD_LED_PIN   26

void printFrame(CAN_FRAME *message)
{
  Serial.print(message->id, HEX);
  if (message->extended) Serial.print(" X ");
  else Serial.print(" S ");   
  Serial.print(message->length, DEC);
  for (int i = 0; i < message->length; i++) {
    Serial.print(message->data.byte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void twoZeroOne(CAN_FRAME *frame)
{
  Serial.print("RPM ");
  //frame->data.byte[0] = 0x1a;
  //frame->data.byte[1] = 0xfb;
  int RPM = ((frame->data.byte[0] << 8) | frame->data.byte[1]) * 4;
  Serial.print(RPM);
  Serial.print(" gas pedal ");
  int gas = (frame->data.byte[6] << 8) | frame->data.byte[7];
  Serial.println(gas);
}

void fourTwoZero(CAN_FRAME *frame)
{
  //printFrame(frame);
  Serial.print("coolant temp ");
  int temp = frame->data.byte[0];
  Serial.println(temp);
}
/* i think not supported
void obdResponse(CAN_FRAME *frame){

printFrame(frame);

}

void sendTempRequest()
{
Serial.println("send");
CAN_FRAME txFrame;
  txFrame.rtr = 0;
  txFrame.id = 0x7DF;
  txFrame.extended = false;
  txFrame.length = 8;
  txFrame.data.uint8[0] = 0x02;
  txFrame.data.uint8[1] = 0x01;
  txFrame.data.uint8[2] = 0x05;
  txFrame.data.uint8[3] = 0xAA;
  txFrame.data.uint8[4] = 0xAA;
  txFrame.data.uint8[5] = 0xAA;
  txFrame.data.uint8[6] = 0xAA;
  txFrame.data.uint8[7] = 0xAA; 
  CAN0.sendFrame(txFrame);
  if(CAN0.hasTXFault()){
    Serial.println("fault");
  }

}
void getSupportedPIDs(){


  while (true) {
    Serial.print(F("Attempting to connect to OBD2 CAN bus ... "));

    if (!OBD2.begin()) {
      Serial.println(F("failed!"));

      delay(1000);
    } else {
      Serial.println(F("success"));
      break;
    }
  }

  Serial.println();

  // loop through PIDs 0 to 95, reading and printing the names of the supported PIDs
  for (int pid = 0; pid < 96; pid++) {
    if (OBD2.pidSupported(pid)) {
      Serial.println(OBD2.pidName(pid));
    }
  }


}*/

void setup()
{
  Serial.begin(115200);
  Serial.println(" CAN...............INIT");
  CAN0.setCANPins(GPIO_NUM_4, GPIO_NUM_5); //config for shield v1.3+, see important note above!
  CAN0.begin(500000); // 500Kbps

  //CAN0.watchFor()
  CAN0.watchFor(0x201); //setup a special filter
  CAN0.watchFor(0x420);
  //CAN0.watchFor(0x7E8);
  //CAN0.watchFor(); //then let everything else through anyway
  CAN0.setCallback(0, twoZeroOne); //callback on that first special filter
  CAN0.setCallback(1, fourTwoZero); //callback on that first special filter
  //CAN0.setCallback(2, obdResponse);
  //CAN0.watchFor();
  Serial.println(" CAN............500Kbps");
  delay(2000);
  //getSupportedPIDs();


// Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


}


void loop()
{
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
  //sendTempRequest();
  
  
  /*
  CAN_FRAME can_message;

  if (CAN0.read(can_message))
  {
    Serial.print("CAN MSG: 0x");
    Serial.print(can_message.id, HEX);
    Serial.print(" [");
    Serial.print(can_message.length, DEC);
    Serial.print("] <");
    for (int i = 0; i < can_message.length; i++)
    {
      if (i != 0) Serial.print(":");
      Serial.print(can_message.data.byte[i], HEX);
    }
    Serial.println(">");
  }*/
}