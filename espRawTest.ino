//1.) Reset Module
//AT+RST\r\n
//
//2.) Join Access Point (Internet Router)
//AT+CWJAP="ATT409","5654665991"\r\n
//
//3.) Connec to website
//AT+CIPSTART="TCP","api.weatherbit.io",80\r\n
//
//4.) Specify number of bytes you'll send
//
//AT+CIPSEND=81\r\n
//
//AT+CIPSEND=154\r\n
//
//AT+CIPSEND=121\r\n
//
//5.) Send GET Request
//
//GET /receiver.php?apples=56&oranges=23 HTTP/1.1\r\nHost: esp8266.allaboutee.com\r\n\r\n
//
//GET /v2.0/current?&city=Albuquerque,NM&key=9f7290a5f40840acb81692a14e6f2c3e&units=I HTTP/1.1\r\nHost: api.weatherbit.io\r\n\r\n

//AT+CIPSTART="TCP","api.weatherapi.com",80\r\n
//AT+CIPSEND=115\r\n
//GET /v1/current.json?key=a38c63037d8746d18dd33450211609&q=Albuquerque&aqi=no HTTP/1.1\r\nHost: api.weatherapi.com


//AT+CIPSTART="SSL","hookb.in",443\r\n
//AT+CIPSEND=54\r\n
//GET /eKpRgRZBQNHlwQmmwkWO HTTP/1.1\r\nHost: hookb.in\r\n\r\n

//5.) send POST Request
//POST /receiver.php HTTP/1.1\r\nHost: esp8266.allaboutee.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 20\r\n\r\napples=12&oranges=45\r\n\r\n

//const char getRequest[] = "GET /eKpRgRZBQNHlwQmmwkWO HTTP/1.1\r\nhost: hookb.in\r\nuser-agent: espS1\r\n\r\n";


//Include statements for here
#include <SoftwareSerial.h>

//define some constants using the preprocessor
#define ESP_TX_ARD_RX 2 // equal to the RX of arduino
#define ESP_RX_ARD_TX 3  //equal to the TX of arduino

//stuff
String SSID_ESP = "snazzyhouse";         //Give EXACT name of your WIFI
String SSID_KEY = "Monkey3010"; 
String HOST = "api.weatherapi.com";
String PORT = "80";
boolean found = false; 
String URI = "/v1/current.json?key=a38c63037d8746d18dd33450211609&q=Albuquerque&aqi=no";
String data = "";
const long DEFAULT_TIMEOUT = 5000;
String getData = 
  "GET " + URI + " HTTP/1.0\n" +
  "Host: " + HOST + "\n" +
  "Accept: application/json\n" +
  "Content-Type: application/json\n" +
  "Connection: Keep-Alive\n" +
  "\n";
char keyMatch[] = "temp_f\":";
char getReply[2000];
SoftwareSerial ESP8266(ESP_TX_ARD_RX, ESP_RX_ARD_TX); //Arduino (rx pin, tx pin)

void setup() {

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
  ESP8266.begin(9600);
  ESP8266.setTimeout(DEFAULT_TIMEOUT);
  delay(2000);//delay before kicking things off
  ConnectToWifi();
  delay(2000);
  //sendCommand("AT+CIPSTART=\"TCP\",\"hookb.in\",443",1000,"OK",false) 
  
  
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
      if(isGetRequest){
        Serial.print(c);
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
      }
      Serial.print(c);
    }
  }
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
