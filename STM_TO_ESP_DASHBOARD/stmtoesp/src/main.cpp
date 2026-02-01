#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// REPLACE WITH YOUR WIFI CREDENTIALS
const char *ssid = "Simmmm:)";
const char *password = "sim10111001";

AsyncWebServer server(80);
String currentValue = "Waiting...";
String touchState = "0";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>STM32 Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; margin: 0; background-color: #121212; color: #e0e0e0; }
    h1 { color: #03dac6; margin-top: 50px; font-weight: 300; letter-spacing: 2px; }
    .card { background-color: #1e1e1e; max-width: 400px; margin: 30px auto; padding: 40px; border-radius: 15px; box-shadow: 0 10px 20px rgba(0,0,0,0.5); border: 1px solid #333; }
    .label { font-size: 1.2rem; color: #bbbbbb; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 20px; }
    .value { font-size: 6rem; font-weight: bold; color: #bb86fc; font-variant-numeric: tabular-nums; line-height: 1; }
    .status { margin-top: 30px; font-size: 0.9rem; color: #666; }
    .touch-indicator { margin-top: 20px; padding: 10px; border-radius: 5px; font-weight: bold; background-color: #333; color: #777; transition: all 0.3s ease; }
    .touch-indicator.active { background-color: #03dac6; color: #000; box-shadow: 0 0 15px rgba(3, 218, 198, 0.5); }
  </style>
</head>
<body>
  <h1>STM32 MONITOR</h1>
  
  <div class="card">
    <div class="label">Current Value</div>
    <div class="value" id="val">--</div>
    <div class="status">Live Data from DWIN</div>
  </div>

  <div class="card">
    <div class="label">Touch Sensor</div>
    <div class="touch-indicator" id="touch">RELEASED</div>
  </div>

  <div class="card">
    <div class="label">Debug Info</div>
    <div class="status" id="debug">Waiting for data...</div>
  </div>

<script>
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4) {
      if (this.status == 200) {
        document.getElementById("debug").innerHTML = "Raw: " + this.responseText;
        var parts = this.responseText.split(",");
        if (parts.length >= 2) {
          document.getElementById("val").innerHTML = parts[0];
          var touchEl = document.getElementById("touch");
          if (parts[1].trim() == "1") {
            touchEl.innerHTML = "TOUCHED";
            touchEl.classList.add("active");
          } else {
            touchEl.innerHTML = "RELEASED";
            touchEl.classList.remove("active");
          }
        }
      } else {
        document.getElementById("debug").innerHTML = "Error: " + this.status;
      }
    }
  };
  xhttp.open("GET", "/value", true);
  xhttp.send();
}, 500); // Refresh every 500ms
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(115200);

  // Initialize Serial2 for communication with STM32
  // RX = GPIO16 (Connect to STM32 PA9/TX)
  // TX = GPIO17 (Connect to STM32 PA10/RX, optional)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  int retry_count = 0;
  while (WiFi.status() != WL_CONNECTED &&
         retry_count < 20) { // Wait max 20 seconds
    delay(1000);
    Serial.print(".");
    retry_count++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected! IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(
        "WiFi Connection Failed! Proceeding with Serial Monitor Test only...");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/value", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", currentValue + "," + touchState);
  });

  server.begin();
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim(); // Remove whitespace/newlines is important
    if (data.length() > 0) {
      // Handle new simple text format from STM32
      if (data.indexOf("TOUCHED") != -1) {
        touchState = "1";
        currentValue = "0"; // Default value
      } else if (data.indexOf("RELEASED") != -1) {
        touchState = "0";
        currentValue = "0"; // Default value
      } else {
        // Fallback just in case
        int commaIndex = data.indexOf(',');
        if (commaIndex != -1) {
          currentValue = data.substring(0, commaIndex);
          touchState = data.substring(commaIndex + 1);
        }
      }

      // Also print to Serial Monitor for debugging
      Serial.print("RX: ");
      Serial.println(data);
    }
  }
}
