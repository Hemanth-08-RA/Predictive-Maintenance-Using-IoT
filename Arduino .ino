#include <WiFi.h>
#include "DHT.h"

// Hardware Pin Configuration
#define DHTPIN 21           // Digital pin connected to the DHT11 sensor
#define DHTTYPE DHT11       // DHT 11 temperature & humidity sensor

const int VIB_PIN = 23;              // Digital pin connected to the vibration sensor
const bool VIB_ACTIVE_STATE_LOW = true; // Set true if sensor pulls LOW when vibration is detected

// WiFi and Cloud API Credentials (REPLACE WITH YOUR OWN VALUES)
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String apiKey        = "YOUR_THINGSPEAK_API_KEY";
const char* server   = "api.thingspeak.com";

// Timing Intervals (Non-blocking using millis)
const unsigned long UPLOAD_INTERVAL_MS = 15000UL; // Upload data to ThingSpeak every 15 seconds
const unsigned long PLOT_INTERVAL_MS = 300UL;     // Print data to Serial Monitor every 300 ms
const uint32_t VIB_SAMPLE_WINDOW_MS = 120;        // Vibration sampling window duration (ms)
const uint16_t VIB_SAMPLE_DELAY_MS = 2;          // Delay between vibration samples (ms)

// Object Initializations
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

// Tracking variables for non-blocking execution timers
unsigned long lastUpload = 0;
unsigned long lastPlot = 0;
int lastVib1023 = 0;

/**
 * Handles the initial WiFi connection and reconnection attempts.
 * Includes a 20-second timeout to prevent the code from hanging indefinitely.
 */
void connectWiFiOnce() {
  if (WiFi.status() == WL_CONNECTED) return;
  
  Serial.print("Connecting to WiFi '"); 
  Serial.print(ssid); 
  Serial.println("' ...");
  
  WiFi.disconnect(true);
  delay(200);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  // Loop until connected or 20 seconds have passed
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000UL) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connect failed");
  }
}

/**
 * Samples a digital vibration sensor repeatedly over a defined window frame.
 * Converts the active frequency ratio into an analog-like range (0 to 1023).
 */
int sampleVibrationAs1023(uint32_t windowMs = VIB_SAMPLE_WINDOW_MS, uint16_t intervalMs = VIB_SAMPLE_DELAY_MS) {
  uint32_t start = millis();
  uint32_t samples = 0;
  uint32_t activeCount = 0;

  // Gather samples within the specified time window
  while (millis() - start < windowMs) {
    int v = digitalRead(VIB_PIN);
    bool active = VIB_ACTIVE_STATE_LOW ? (v == LOW) : (v == HIGH);
    if (active) activeCount++;
    samples++;
    delay(intervalMs);
  }
  
  if (samples == 0) return 0;
  
  // Calculate active ratio and scale it to a 0-1023 range
  float fraction = (float)activeCount / (float)samples;
  int val = (int)round(fraction * 1023.0f);
  
  // Constrain the output to valid bounds
  if (val < 0) val = 0;
  if (val > 1023) val = 1023;
  return val;
}

void setup() {
  Serial.begin(115200);
  delay(50);

  // Initialize sensors
  dht.begin();
  if (VIB_ACTIVE_STATE_LOW) {
    pinMode(VIB_PIN, INPUT_PULLUP); // Use internal pullup resistor for active-LOW configurations
  } else {
    pinMode(VIB_PIN, INPUT);
  }

  // Initial connection to network
  connectWiFiOnce();
  lastUpload = millis();
  lastPlot = millis();
}

void loop() {
  // Ensure network persistence
  if (WiFi.status() != WL_CONNECTED) connectWiFiOnce();

  unsigned long now = millis();

  // --- Task 1: Local Serial Monitoring and Local Plotting ---
  if (now - lastPlot >= PLOT_INTERVAL_MS) {
    lastPlot = now;

    float temp = dht.readTemperature();
    bool tempOk = !isnan(temp);
    if (!tempOk) temp = 0.0f; // Fallback value if sensor read fails

    // Sample the vibration intensity
    lastVib1023 = sampleVibrationAs1023();

    // Human-readable output
    Serial.print("Temp: ");
    Serial.print(tempOk ? String(temp, 2) : String(0.00, 2));
    Serial.print(" °C  | Vib(0..1023): ");
    Serial.println(lastVib1023);

    // Serial Plotter compatible format
    Serial.print(tempOk ? String(temp, 2) : String(0.00, 2));
    Serial.print(" ");
    Serial.println(String(lastVib1023));
  }

  // --- Task 2: Remote Data Upload to ThingSpeak ---
  if (now - lastUpload >= UPLOAD_INTERVAL_MS) {
    lastUpload = now;

    float tempU = dht.readTemperature();
    if (isnan(tempU)) tempU = 0.0f;

    Serial.println();
    Serial.println("Uploading to ThingSpeak...");
    
    // Construct HTTP GET payload string
    String url = "/update?api_key=" + apiKey + "&field1=" + String(tempU, 2) + "&field2=" + String(lastVib1023);
    Serial.println("GET " + url);

    if (WiFi.status() == WL_CONNECTED) {
      // Connect to server on HTTP port 80
      if (client.connect(server, 80)) {
        // Send manual raw HTTP headers
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");

        // Handle server response with a 4-second hard timeout
        unsigned long tmo = millis() + 4000UL;
        while (millis() < tmo && (client.connected() || client.available())) {
          if (client.available()) {
            String line = client.readStringUntil('\n');
            Serial.println(line); // Output server acknowledgment
          }
        }
        client.stop(); // Free up client connection socket
      } else {
        Serial.println("client.connect failed");
      }
    } else {
      Serial.println("WiFi not connected");
    }
  }

  delay(10); // Short stabilization delay per loop cycle
}
