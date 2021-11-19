/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-one-to-many-esp8266-nodemcu/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/




#include <ESP8266WiFi.h>
#include <espnow.h>

int gre = 14;
int yel = 12;
int red = 13;

// REPLACE WITH RECEIVER MAC Address
//                             CC:50:E3:0C:73:88
//                             0xDF, 0xDB, 0x84, 0xE0, 0xCD, 0x07
//E8:DB:84:E0:CD:73    CC:50:E3:0C:74:0E

uint8_t broadcastAddress1[] = {0xE8, 0xDB, 0x84, 0xE0, 0xCD, 0x73}; //matinho
uint8_t broadcastAddress2[] = {0xCC, 0x50, 0xE3, 0x0C, 0x74, 0x0E}; //marquinhos
uint8_t broadcastAddress3[] = {0xE0, 0x98, 0x06, 0x85, 0xFA, 0xD9}; //carlinhos

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int state;
} struct_message;

// Create a struct_message called test to store variables to be sent
struct_message semaphore;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(gre, OUTPUT);
  pinMode(yel, OUTPUT);
  pinMode(red, OUTPUT);
   
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}
 
void loop() {

    digitalWrite(red, LOW);
    digitalWrite(gre, HIGH);
    semaphore.state = 1; // verde
    esp_now_send(0, (uint8_t *) &semaphore, sizeof(semaphore));
    delay(10000);
    digitalWrite(gre, LOW);
    digitalWrite(yel, HIGH);
    semaphore.state = 2; // amarelo
    esp_now_send(0, (uint8_t *) &semaphore, sizeof(semaphore));
    delay(4000);
    digitalWrite(yel, LOW);
    digitalWrite(red, HIGH);
    semaphore.state = 3; // vermhlo
    esp_now_send(0, (uint8_t *) &semaphore, sizeof(semaphore));
    delay(10000);
    
   
}
