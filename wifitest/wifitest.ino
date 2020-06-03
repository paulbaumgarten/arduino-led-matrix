#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid = "SCWiFi";
const char* password = "wifi1234";

int ledPin = 13; // GPIO13
WiFiServer server(80);
SoftwareSerial wifiSerial(5, 4); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  wifiSerial.begin(1200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  wifiSerial.print("display=Joining ");
  wifiSerial.print(ssid);
  wifiSerial.print("\n");
  wifiSerial.println();
  wifiSerial.flush();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  IPAddress ipaddress = WiFi.localIP();
  String message = "display=" + ipaddress.toString() + "\n";
  Serial.println("SENT: "+message);
  wifiSerial.println(message);
  wifiSerial.flush();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  // Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
  char request[255];
  int requestLength = 0;
  while (client.available() > 0) {
    if (requestLength < 254) {
      request[requestLength++] = client.read();
    }
  }
  Serial.print("SENT: ");
  Serial.println(request);
  wifiSerial.println(request);
  wifiSerial.println();  
  wifiSerial.flush();
  client.println("ok");
  client.flush();

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  // client.println("HTTP/1.1 200 OK");
  // client.println("Content-Type: text/plain");
  // client.println(""); //  do not forget this one
  client.println("ok");
  delay(1);
}
