
//------------------Code----------------------------------------//
//------------------30/05/64------------------------------------//
//------------------Loggerfire.com------------------------------//
//------------------Esp32-Azure-IoT-----------------------------//
//--------------------------------------------------------------//

#include <BH1750.h>
#include <WiFi.h>
#include <Wire.h>
#include <FaBoHumidity_HTS221.h>
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
FaBoHumidity_HTS221 faboHumidity;

//-----BH1750--------------------------------------//
BH1750 lightMeter;


// Initialize the OLED display using Wire library
 SSD1306Wire  display(0x3c, 25, 26);

const char* ssid     = "Jedi_2.4G";
const char* password = "038587413";

const char* host = "loggerfire.com";


const char* Secret_Key1 = "jBTymLCtk5639ecDRproSnJU";
const char* Secret_Key2 = "s3KgelGryFiSmVofp9wWZnjt";
const char* Secret_Key3 = "14vdFCwMeDgy6ioJt9pPAIkx";

void setup() {



  display.init();
  display.flipScreenVertically();


  Wire.begin(25,26);
  Serial.begin(115200);
  Serial.println("RESET");
  Serial.println();
  Serial.println("configuring device.");

  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
  

  if (faboHumidity.begin()) {
    Serial.println("configured FaBo Humidity Brick");
  } else {
    Serial.println("device error");
    while(1);
  }

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
//------------------------Variable--------------------//
double valueTemp;
double valueHumidity;
float valueLux;

//----------------Main-Loop---------------------------//
void loop() {
  //--------------Connected---------------------------//
   
  
  // We start by connecting to a WiFi network
/*
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    */
    
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  
  //--------------Get_Data---------------------------//
  double temp = faboHumidity.getTemperature();
  double humidity = faboHumidity.getHumidity();
  Serial.print(temp);
  Serial.println(" C");
  Serial.print(humidity);
  Serial.println(" %");

  //-------------Get-BH1750--------------------------//
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  
  //---------------Display_OLED---------------------//
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Loggerfire.com");
  display.drawString(71, 0,"(Connected)");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, "Temp(C):");
  display.drawString(70, 20, String(valueTemp));
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "Humidity:");
  display.drawString(70, 40, String(valueHumidity));
  display.display();
  delay(5000);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Loggerfire.com");
  display.drawString(71, 0,"(Connected)");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, "Lux:");
  display.drawString(70, 20, String(lux));
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "       ");
  display.drawString(70, 40, "       ");
  display.display();
  delay(5000);
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Loggerfire.com");
  display.drawString(71, 0,"(Connected)");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, "Temp(C):");
  display.drawString(70, 20, String(valueTemp));
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "Humidity:");
  display.drawString(70, 40, String(valueHumidity));
  display.display();
  
  //-------Value---------------------------------//
  ++valueTemp;
  ++valueHumidity; 
  ++valueLux;
  valueTemp = temp-5;
  valueHumidity = humidity;
  valueLux = lux;

  
  //-----------First-Data----------------------------------//
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/connect";
  url += "?Secret_Key=";
  url += Secret_Key1;
  url += "&value=";
  url += valueTemp;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  delay(1000);


 //----------Second-Data-----------------------------------------//
                                                                  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  url = "/connect";
  url += "?Secret_Key=";
  url += Secret_Key2;
  url += "&value=";
  url += valueHumidity;
  Serial.print("Requesting URL: ");
  Serial.println(url);
                 // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }


 //----------Third-Data-----------------------------------------//
                                                                  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  url = "/connect";
  url += "?Secret_Key=";
  url += Secret_Key3;
  url += "&value=";
  url += valueLux;
  Serial.print("Requesting URL: ");
  Serial.println(url);
                 // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  
  
  Serial.println();
  Serial.println("closing connection");
  delay(500);
  
 //----------------------Humidity-------------------------------//

  
}
