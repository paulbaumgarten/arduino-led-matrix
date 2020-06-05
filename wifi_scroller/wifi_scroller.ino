#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid = "SCWiFi";
const char* password = "wifi1234";

int ledPin = 13; // GPIO13
WiFiServer server(4242);
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
  wifiSerial.print("{Joining ");
  wifiSerial.print(ssid);
  wifiSerial.print("}\n");
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
  String message = "{" + ipaddress.toString() + "}\n";
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
  bool capture = false;
  bool transmit = false;
  while (client.available() > 0) {
    char c = client.read();
    if (c == '{') {
      capture = true;
      request[requestLength++] = '{';
    } else if (c == '}') {
      capture = false;
      request[requestLength++] = '}';
      transmit = true;
    }
    else if (isPrintable(c) && requestLength < 252 && capture) {
      request[requestLength++] = c;
    }
    delay(1);
  }
  if (transmit) {
    Serial.print("SENT: ");
    Serial.println(request);
    wifiSerial.println(request);
    wifiSerial.println();  
    wifiSerial.flush();
    client.println("Message successfully received.\n\n");
    client.flush();
  } else {
    client.println("Malformed message. Enclose your message curly braces, eg {Hello world}. Max message size 250 characters.\n\n");
    client.flush();
  }
  delay(1);
}
