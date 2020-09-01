#include <ESP8266WiFi.h>
 
const char* ssid = "namawifi"; // Ubah sesuai nama wifi yg digunakan
const char* password = "passwordwifi"; // Ubah sesuai password wifi yg digunakan
 #define RELAY 0 // relay harus terhubung ke GPIO0
WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200); // Baudrate harus sama dengan serial monitor
 
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("https://192.168.0.178/");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  int value = LOW;
  if (request.indexOf("/RELAY=ON") != -1)  
  {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY,LOW);
    value = LOW;
  }
  if (request.indexOf("/RELAY=OFF") != -1)  
  {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY,HIGH);
    value = HIGH;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE html><html>");
  client.println("<head><title>ESP8266 RELAY Control</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
  client.println("<body style=\"color: white;background-color: rgb(13, 26, 25);display:grid;place-items: center;font-family: Arial, Helvetica, sans-serif;\">");
client.println("<h1>LAMPU HOROR</h1>");
if(value == HIGH) 
  {
    client.print("<h1 style=\"filter: grayscale(1);font-size:8em;\">&#128161;</h1>");
  } 
  else 
  {
    client.print("<h1 style=\"font-size:8em;\">&#128161;</h1>");
  }
client.println("<span style=\"background-color:#1e4734;width: 150px;height: 3em;display:grid;place-items: center;border-radius: 10px;margin: 20px;\"><a href=\"/RELAY=ON\" style=\"text-decoration:none;color: white;font-weight: 600;font-size: 2em;\">ON</a></span>");
client.println("<span style=\"background-color:#1e4734;width: 150px;height: 3em;display:grid;place-items: center;border-radius: 10px;margin: 20px;\"><a href=\"/RELAY=OFF\" style=\"text-decoration:none;color: white;font-weight: 600;font-size: 2em;\">OFF</a></span>");
client.println("<p>&copy RUANG SERVER</p></body>");
    client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
