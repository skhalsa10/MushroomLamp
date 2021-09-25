/*
 * I find it is best to send AT commands and not use a library.
 * I can not really find a great resource that covers ALL AT commands.
 * 
 * https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf
 * https://www.electronicshub.org/esp8266-at-commands/
 * https://create.arduino.cc/projecthub/neverofftheinternet/esp8266-setup-and-first-wifi-connection-76fc3c
 * 
 * will be getting data from:
 * http://api.weatherapi.com/v1/current.json?key=a38c63037d8746d18dd33450211609&q=Albuquerque&aqi=no 
 * 
 * The goal is to get temperature value from a weather API periodically. I will then map this number to a 
 * Color  on an LED mushroom lamp
 * 
 * Reset Module
 * AT+RST
 * 
 * Confirm communication
 * AT
 * 
 * set esp to only act as client
 * AT+CWMODE=1
 * 
 * Join Access Point (Internet Router)
 * AT+CWJAP="SSID","Password"\r\n
 * 
 * make TCP connection
 * "AT+CIPSTART="TCP","HOST",80"
 * 
 * send expected getpayload size
 * AT+CIPSEND=###
 * 
 * send get request
 * "GET " + URI + " HTTP/1.0\n" +
 * "Host: " + HOST + "\n" +
 *  "Accept: application/json\n" +
 *  "Content-Type: application/json\n" +
 *  "Connection: Keep-Alive\n" +
 *  "\n";
 */


//Include statements for here
#include <SoftwareSerial.h>

//define some constants using the preprocessor
#define ESP_TX_ARD_RX 2 // equal to the RX of arduino
#define ESP_RX_ARD_TX 3  //equal to the TX of arduino

//stuff
String SSID_ESP = "snazzyhouse";         //Give EXACT name of your WIFI
String SSID_KEY = "Monkey3010"; 

//the host to send get request to
String HOST = "api.weatherapi.com";
String PORT = "80";
boolean found = false; 
//this will send us the correct info
String URI = "/v1/current.json?key=a38c63037d8746d18dd33450211609&q=Albuquerque&aqi=no";
const long DEFAULT_TIMEOUT = 5000;

// Get request to send down the TCP pipe
String getData = 
  "GET " + URI + " HTTP/1.0\n" +
  "Host: " + HOST + "\n" +
  "Accept: application/json\n" +
  "Content-Type: application/json\n" +
  "Connection: Keep-Alive\n" +
  "\n";
  
//buffer to store Get Reply
char getReply[2000];

//serial connection emulated in software receive data on pin in first param
//send data out the second param.
SoftwareSerial ESP8266(ESP_TX_ARD_RX, ESP_RX_ARD_TX); //Arduino (rx pin, tx pin)

void setup() {
  //null out buffer
  for(int i = 0; i< 2000; i++){
    getReply[i] = '\0';
  }

  //Pin Modes for ESP TX/RX
  pinMode(ESP_TX_ARD_RX, INPUT);
  pinMode(ESP_RX_ARD_TX, OUTPUT);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600); //for status and debug
  while (!Serial) {;} 
  Serial.println("Started");
  ESP8266.begin(9600); //begin transmission on wires connected to wifi module
  ESP8266.setTimeout(DEFAULT_TIMEOUT);
  delay(2000);//delay before kicking things off
  
  ConnectToWifi();
  delay(2000);
   
  sendCommand("AT+CIPSTART=\"TCP\",\""+ HOST +"\","+ PORT,150,"OK",false);
  sendCommand("AT+CIPSEND=" +String(getData.length()+2),500,">",false);
  sendCommand(getData,50000,"OK",true);
  //sendCommand("AT+CIPCLOSE",500,"OK",false);
  Serial.println("ENDING SETUP");
  delay(1000);
}

void loop() {

  
}
bool ConnectToWifi(){

    sendCommand("AT",500,"OK",false);
    sendCommand("AT+CWMODE=1",500,"OK",false);
    boolean isConnectedToWifi;
    isConnectedToWifi = sendCommand("AT+CWJAP=\""+ SSID_ESP +"\",\""+ SSID_KEY +"\"",10000,"OK",false);
    if(isConnectedToWifi)
    {
      return true;
    }
    

} 

bool sendCommand(String command, long timeout, char expectedReply[], boolean isGetRequest) {
  
  ESP8266.setTimeout(timeout);
  long startTime = millis();
  int matchIndex = 0;
  char temp[3];
  temp[2] = '_';

  
  //Test Purpose
  Serial.print("Running Command: ");
  Serial.print(command);
  Serial.println();

  ESP8266.flush();
  ESP8266.println(command);
  ESP8266.flush();

  Serial.println(isGetRequest);
  int i = 0;
  while(millis()-startTime < timeout){
    
    while(ESP8266.available()){
      char c = ESP8266.read();

      //if I uncomment this IF than i start missing incoming data it gets lost
      
//      if(isGetRequest){
//        Serial.print(c);
//        getData[i] = c;
        i++;
//        if(matchIndex < 7){
//          Serial.println(String(c) + ":" + String(keyMatch[matchIndex]));
//          if(c == keyMatch[matchIndex]){
//            matchIndex++;
//          }else{
//            matchIndex = 0;
//          }
//        }else if(matchIndex >= 8 && matchIndex <10){
//          Serial.println("FOUND TEMP");
//          Serial.println(c);
//          temp[matchIndex-8] = c;
//          matchIndex++;
//        }
//      }
      //this will print the full get request as long as I dont try to do anything more but this
      Serial.print(c);
    }
  }
  // this is for debugging
  Serial.println(i);
  if(isGetRequest){
    for(int j = 0; j< i;j++){
      Serial.print(getData[j]);
    }
  }
  


  //String reply = ESP8266.readString();
  
  ESP8266.setTimeout(DEFAULT_TIMEOUT);
  Serial.println("\nEnding send command\n");
  return true;
}
