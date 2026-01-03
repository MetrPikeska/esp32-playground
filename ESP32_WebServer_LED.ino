/*
 * ESP32 Web Server LED Control
 * 
 * This sketch demonstrates how to:
 * - Connect ESP32 to Wi-Fi
 * - Create a simple web server
 * - Control an LED through a web interface
 * 
 * Hardware Setup:
 * - ESP32 Dev Module
 * - LED connected to GPIO 2 through a resistor (220-330 ohm) to GND
 * 
 * Target Audience: Beginners learning ESP32, Wi-Fi, and IoT basics
 */

#include <WiFi.h>

// ===== CONFIGURATION =====
// Replace with your Wi-Fi network credentials
const char* ssid = "Tomsovsky";        // Your Wi-Fi network name
const char* password = "604246127"; // Your Wi-Fi password

// GPIO pin where the LED is connected
const int ledPin = 2;

// Create a web server on port 80 (standard HTTP port)
WiFiServer server(80);

// Variable to store the current LED state
String ledState = "OFF";

// ===== SETUP FUNCTION =====
// Runs once when ESP32 starts or resets
void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  delay(100);
  
  // Configure the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Start with LED off
  
  Serial.println("\n===== ESP32 Web Server LED Control =====");
  Serial.println("Starting up...\n");
  
  // Connect to Wi-Fi network
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  // Wait for connection with visual feedback
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // If connection fails after 20 seconds, restart ESP32
    if (attempts > 40) {
      Serial.println("\nFailed to connect to Wi-Fi. Please check credentials.");
      Serial.println("Restarting in 3 seconds...");
      delay(3000);
      ESP.restart();
    }
  }
  
  // Connection successful - print network details
  Serial.println("\n\n*** Wi-Fi Connected Successfully! ***");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  
  // Start the web server
  server.begin();
  Serial.println("Web server started on port 80");
  Serial.println("\nOpen your browser and navigate to the IP address above.");
  Serial.println("========================================\n");
}

// ===== MAIN LOOP =====
// Runs continuously after setup() completes
void loop() {
  // Listen for incoming clients (web browser requests)
  WiFiClient client = server.available();
  
  // If a client connects
  if (client) {
    Serial.println("New client connected");
    
    // String to store the incoming HTTP request
    String currentLine = "";
    
    // While the client is connected
    while (client.connected()) {
      // If there's data available to read
      if (client.available()) {
        char c = client.read(); // Read one character
        
        // If we receive a newline character, the HTTP request line is complete
        if (c == '\n') {
          // If the current line is blank, we've received two newlines
          // This means the HTTP header has ended, so we send a response
          if (currentLine.length() == 0) {
            
            // Send HTTP response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println(); // Empty line signals end of headers
            
            // ===== HTML WEB PAGE =====
            // Send the HTML page to the browser
            client.println("<!DOCTYPE html>");
            client.println("<html lang='en'>");
            client.println("<head>");
            client.println("  <meta charset='UTF-8'>");
            client.println("  <meta name='viewport' content='width=device-width, initial-scale=1.0'>");
            client.println("  <title>ESP32 LED Control</title>");
            client.println("  <style>");
            client.println("    body { font-family: Arial, sans-serif; text-align: center; margin: 50px; background-color: #f0f0f0; }");
            client.println("    h1 { color: #333; }");
            client.println("    .container { background-color: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); max-width: 400px; margin: 0 auto; }");
            client.println("    .status { font-size: 24px; margin: 20px 0; padding: 15px; border-radius: 5px; }");
            client.println("    .status.on { background-color: #4CAF50; color: white; }");
            client.println("    .status.off { background-color: #f44336; color: white; }");
            client.println("    button { font-size: 18px; padding: 15px 30px; margin: 10px; border: none; border-radius: 5px; cursor: pointer; transition: 0.3s; }");
            client.println("    .btn-on { background-color: #4CAF50; color: white; }");
            client.println("    .btn-on:hover { background-color: #45a049; }");
            client.println("    .btn-off { background-color: #f44336; color: white; }");
            client.println("    .btn-off:hover { background-color: #da190b; }");
            client.println("    .info { margin-top: 30px; color: #666; font-size: 14px; }");
            client.println("  </style>");
            client.println("</head>");
            client.println("<body>");
            client.println("  <div class='container'>");
            client.println("    <h1>🔌 ESP32 LED Control</h1>");
            
            // Display current LED status
            client.print("    <div class='status ");
            client.print(ledState == "ON" ? "on" : "off");
            client.print("'>LED is ");
            client.print(ledState);
            client.println("</div>");
            
            // Control buttons
            client.println("    <div>");
            client.println("      <a href='/ON'><button class='btn-on'>Turn ON</button></a>");
            client.println("      <a href='/OFF'><button class='btn-off'>Turn OFF</button></a>");
            client.println("    </div>");
            
            // Info section
            client.println("    <div class='info'>");
            client.println("      <p>GPIO 2 | ESP32 Dev Module</p>");
            client.print("      <p>IP: ");
            client.print(WiFi.localIP());
            client.println("</p>");
            client.println("    </div>");
            client.println("  </div>");
            client.println("</body>");
            client.println("</html>");
            
            // The HTTP response ends with a blank line
            client.println();
            
            // Break out of the while loop
            break;
            
          } else {
            // If we got a newline, clear the current line
            currentLine = "";
          }
          
        } else if (c != '\r') {
          // If we got any character other than carriage return, add it to current line
          currentLine += c;
        }
        
        // ===== CHECK FOR LED CONTROL COMMANDS =====
        // If the HTTP request contains "/ON", turn the LED on
        if (currentLine.endsWith("GET /ON")) {
          digitalWrite(ledPin, HIGH);
          ledState = "ON";
          Serial.println("LED turned ON");
        }
        
        // If the HTTP request contains "/OFF", turn the LED off
        if (currentLine.endsWith("GET /OFF")) {
          digitalWrite(ledPin, LOW);
          ledState = "OFF";
          Serial.println("LED turned OFF");
        }
      }
    }
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected\n");
  }
}
