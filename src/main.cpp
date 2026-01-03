#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const int SOIL_PIN = 34;

const int DRY_VALUE = 2500;
const int WET_VALUE = 1100;

const char* WIFI_SSID = "Tomsovsky";
const char* WIFI_PASSWORD = "604246127";

WebServer server(80);
unsigned long lastMeasurement = 0;
const unsigned long MEASUREMENT_INTERVAL = 5000;

int currentRaw = 0;
int currentPercent = 0;
String currentStatus = "INIT";

const char HTML_TEMPLATE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Soil Monitor</title>
<style>
body{font-family:Arial,sans-serif;max-width:600px;margin:40px auto;padding:20px;background:#f5f5f5}
.card{background:white;padding:30px;border-radius:8px;box-shadow:0 2px 4px rgba(0,0,0,0.1)}
h1{color:#333;margin-top:0}
.metric{margin:20px 0;padding:15px;background:#f9f9f9;border-radius:4px}
.label{font-size:14px;color:#666;margin-bottom:5px}
.value{font-size:32px;font-weight:bold;color:#2196F3}
.status{padding:10px 20px;border-radius:4px;display:inline-block;font-weight:bold;margin-top:10px}
.status.dry{background:#ff9800;color:white}
.status.ok{background:#4caf50;color:white}
.status.wet{background:#2196f3;color:white}
.footer{margin-top:20px;font-size:12px;color:#999;text-align:center}
</style>
<script>
setInterval(function(){location.reload()},10000);
</script>
</head>
<body>
<div class="card">
<h1>Soil Moisture Monitor</h1>
<div class="metric">
<div class="label">Moisture Level</div>
<div class="value">{{PERCENT}}%</div>
</div>
<div class="metric">
<div class="label">Raw ADC Value</div>
<div class="value">{{RAW}}</div>
</div>
<div class="metric">
<div class="label">Status</div>
<div class="status {{STATUS_CLASS}}">{{STATUS}}</div>
</div>
<div class="footer">Auto-refresh every 10 seconds</div>
</div>
</body>
</html>
)rawliteral";

int readSoilMoisture() {
  int raw = analogRead(SOIL_PIN);
  Serial.print("[DEBUG] ADC read: ");
  Serial.println(raw);
  return raw;
}

String getStatus(int percent) {
  if (percent < 30) return "DRY";
  if (percent < 70) return "OK";
  return "WET";
}

String getStatusClass(String status) {
  if (status == "DRY") return "dry";
  if (status == "OK") return "ok";
  return "wet";
}

void handleRoot() {
  String html = String(HTML_TEMPLATE);
  html.replace("{{PERCENT}}", String(currentPercent));
  html.replace("{{RAW}}", String(currentRaw));
  html.replace("{{STATUS}}", currentStatus);
  html.replace("{{STATUS_CLASS}}", getStatusClass(currentStatus));
  server.send(200, "text/html", html);
}

void handleAPI() {
  String json = "{";
  json += "\"raw\":" + String(currentRaw) + ",";
  json += "\"percent\":" + String(currentPercent) + ",";
  json += "\"status\":\"" + currentStatus + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  adcAttachPin(SOIL_PIN);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  Serial.println();
  Serial.println("ESP32 Soil Moisture Monitor");
  Serial.println("============================");
  
  Serial.println("\nTesting sensor...");
  delay(100);
  int testRead = analogRead(SOIL_PIN);
  Serial.print("Sensor test reading: ");
  Serial.println(testRead);
  
  if (testRead == 0 || testRead >= 4095) {
    Serial.println("WARNING: Sensor may not be connected properly!");
    Serial.println("Expected range: 1000-3000");
  }
  
  Serial.print("\nConnecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
  }
  
  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  
  server.begin();
  Serial.println("HTTP server started on port 80");
  
  currentRaw = readSoilMoisture();
  currentPercent = map(currentRaw, DRY_VALUE, WET_VALUE, 0, 100);
  currentPercent = constrain(currentPercent, 0, 100);
  currentStatus = getStatus(currentPercent);
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - lastMeasurement >= MEASUREMENT_INTERVAL) {
    lastMeasurement = currentMillis;
    
    currentRaw = readSoilMoisture();
    currentPercent = map(currentRaw, DRY_VALUE, WET_VALUE, 0, 100);
    currentPercent = constrain(currentPercent, 0, 100);
    currentStatus = getStatus(currentPercent);
    
    Serial.print("Raw: ");
    Serial.print(currentRaw);
    Serial.print(" | Percent: ");
    Serial.print(currentPercent);
    Serial.print("% | Status: ");
    Serial.println(currentStatus);
  }
}
