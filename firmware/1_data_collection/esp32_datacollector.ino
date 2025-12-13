/*
 * ESP32 Side-Channel Data Collection - Phase 1
 * Realistic WiFi Activity Simulation for TinyML Training
 * 
 * WIRING:
 * ESP32 3.3V → 10kΩ → GPIO34 (ADC) → 10kΩ → GND
 * OLED: SDA=21, SCL=22, VCC=3.3V, GND=GND
 * 
 * DATA OUTPUT: High-speed CSV via Serial (115200 baud)
 * OLED: Minimal display to avoid interference with data collection
 */

 #include <WiFi.h>
 #include <HTTPClient.h>
 #include <WebSocketsClient.h>
 #include <Wire.h>
 #include <U8g2lib.h>
 #include "BluetoothSerial.h"
 
 // ============ CONFIGURATION ============
 #define WIFI_SSID "xxxxxx"
 #define WIFI_PASSWORD "xxxxxx"
 
 #define ADC_PIN 34
 #define SDA_PIN 21
 #define SCL_PIN 22
 
 #define SAMPLE_RATE 200          // 200 Hz sampling
 #define SAMPLES_PER_ACTIVITY 6000 // 30 seconds @ 200Hz
 #define REPETITIONS 5            // 5 samples per activity
 
 // ============ HARDWARE ============
 U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
 WebSocketsClient webSocket;
 BluetoothSerial SerialBT;
 HTTPClient http;
 WiFiClient client;
 
 // ============ ACTIVITIES ============
 enum Activity {
   IDLE = 0,
   SOCIAL_MEDIA = 1,
   VIDEO_STREAMING = 2,
   MESSAGING = 3,
   WEB_BROWSING = 4,
   GAMING = 5
 };
 
 const char* activity_names[] = {
   "IDLE", "SOCIAL", "VIDEO", "MESSAGE", "BROWSE", "GAMING"
 };
 
 // ============ STATE ============
 Activity current_activity = IDLE;
 int current_rep = 0;
 unsigned long sample_count = 0;
 unsigned long activity_start = 0;
 bool collection_active = false;
 
 // ============ SETUP ============
 void setup() {
   Serial.begin(115200);
   delay(2000);
   
   Serial.println("\n╔════════════════════════════════════════╗");
   Serial.println("║  RF Side-Channel Data Collector       ║");
   Serial.println("║  TinyML Training Dataset Generator     ║");
   Serial.println("╚════════════════════════════════════════╝\n");
   
   // Configure ADC
   analogReadResolution(12);
   analogSetAttenuation(ADC_11db);
   Serial.println("✓ ADC configured: GPIO34, 12-bit, 200Hz");
   
   // Initialize OLED (minimal display)
   Wire.begin(SDA_PIN, SCL_PIN);
   u8g2.begin();
   u8g2.setContrast(200);
   
   show_status("Initializing...", "");
   
   // Connect WiFi
   Serial.print("Connecting WiFi...");
   WiFi.mode(WIFI_STA);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   
   Serial.println("\n✓ WiFi connected");
   Serial.print("IP: ");
   Serial.println(WiFi.localIP());
   Serial.print("RSSI: ");
   Serial.println(WiFi.RSSI());
   
   show_status("WiFi Connected", WiFi.localIP().toString().c_str());
   delay(2000);
   
   // Print CSV header
   Serial.println("\n=== DATA COLLECTION START ===");
   Serial.println("timestamp_ms,voltage_mv,rssi_dbm,activity_id,activity_name");
   
   // Wait for user to start
   show_status("Ready to Collect", "Press any key...");
   Serial.println("\nPress ANY KEY to start data collection...");
   
   while (!Serial.available()) {
     delay(100);
   }
   Serial.read(); // Clear buffer
   
   collection_active = true;
   current_activity = IDLE;
   current_rep = 0;
   
   Serial.println("\n>>> Starting Collection <<<");
   start_new_activity();
 }
 
 // ============ MAIN LOOP ============
 void loop() {
   if (!collection_active) {
     show_status("Collection Done!", "Check Serial");
     delay(1000);
     return;
   }
   
   unsigned long now = millis();
   
   // Sample at 200Hz
   static unsigned long last_sample = 0;
   if (now - last_sample >= 5) {  // 5ms = 200Hz
     last_sample = now;
     
     // Read ADC
     int adc_raw = analogRead(ADC_PIN);
     float voltage_mv = (adc_raw / 4095.0) * 3300.0;
     
     // Read RSSI
     int rssi = WiFi.RSSI();
     
     // Output CSV
     Serial.print(now);
     Serial.print(",");
     Serial.print(voltage_mv, 2);
     Serial.print(",");
     Serial.print(rssi);
     Serial.print(",");
     Serial.print(current_activity);
     Serial.print(",");
     Serial.println(activity_names[current_activity]);
     
     sample_count++;
     
     // Update display every 100 samples (0.5 sec)
     if (sample_count % 100 == 0) {
       update_display();
     }
   }
   
   // Execute activity behavior
   execute_activity_behavior();
   
   // Check if activity complete
   if (sample_count >= SAMPLES_PER_ACTIVITY) {
     finish_activity();
   }
 }
 
 // ============ ACTIVITY BEHAVIORS ============
 void execute_activity_behavior() {
   static unsigned long last_action = 0;
   unsigned long now = millis();
   
   switch(current_activity) {
     case IDLE:
       // Minimal activity - just keepalive every 5 seconds
       if (now - last_action > 5000) {
         WiFi.RSSI(); // Passive check
         last_action = now;
       }
       break;
       
     case SOCIAL_MEDIA:
       // Bursty: scroll → load images → pause → repeat
       if (now - last_action > random(1000, 3000)) {
         http_get_burst(random(50000, 200000)); // Image load
         delay(random(100, 300));
         http_get_burst(random(5000, 20000));   // Feed refresh
         last_action = now;
       }
       break;
       
     case VIDEO_STREAMING:
       // Continuous high-bandwidth chunks
       if (now - last_action > 40) {  // 25fps
         http_get_burst(1400);  // MTU-sized
         last_action = now;
       }
       break;
       
     case MESSAGING:
       // Small frequent packets with occasional voice
       if (now - last_action > random(2000, 8000)) {
         http_get_burst(random(100, 500)); // Text message
         
         // Occasional voice note (20% chance)
         if (random(0, 5) == 0) {
           delay(100);
           http_get_burst(random(50000, 150000));
         }
         last_action = now;
       }
       break;
       
     case WEB_BROWSING:
       // Page load burst
       if (now - last_action > random(3000, 8000)) {
         http_get_burst(random(10000, 50000));   // HTML
         delay(random(200, 500));
         http_get_burst(random(50000, 200000));  // Assets
         last_action = now;
       }
       break;
       
     case GAMING:
       // High frequency small packets + CPU load
       if (now - last_action > 20) {  // 50Hz tick
         udp_burst(random(50, 150));
         
         // CPU-intensive calculation (game physics simulation)
         volatile float dummy = 0;
         for(int i = 0; i < 5000; i++) {
           dummy += sqrt(i) * sin(i * 0.01);
         }
         
         // Occasional voice chat (10% chance)
         if (random(0, 10) == 0) {
           http_get_burst(random(20000, 50000));
         }
         
         last_action = now;
       }
       break;
   }
 }
 
 // ============ NETWORK FUNCTIONS ============
 void http_get_burst(int size_bytes) {
   if (WiFi.status() != WL_CONNECTED) return;
   
   // Use httpbin.org for realistic traffic
   String url = "http://httpbin.org/bytes/" + String(size_bytes);
   
   http.begin(client, url);
   http.setTimeout(2000);
   http.GET();
   http.end();
 }
 
 void udp_burst(int size_bytes) {
   // Send UDP packets (gaming simulation)
   WiFiUDP udp;
   udp.begin(12345);
   
   uint8_t buffer[1400];
   int sent = 0;
   
   while (sent < size_bytes) {
     int chunk = min(1400, size_bytes - sent);
     udp.beginPacket("8.8.8.8", 53);
     udp.write(buffer, chunk);
     udp.endPacket();
     sent += chunk;
   }
   
   udp.stop();
 }
 
 // ============ COLLECTION CONTROL ============
 void start_new_activity() {
   sample_count = 0;
   activity_start = millis();
   
   Serial.print("\n╔══════════════════════════════════════╗\n");
   Serial.print("║  Activity: ");
   Serial.print(activity_names[current_activity]);
   Serial.print(" | Rep: ");
   Serial.print(current_rep + 1);
   Serial.print("/");
   Serial.print(REPETITIONS);
   Serial.println(" ║");
   Serial.println("╚══════════════════════════════════════╝");
   
   show_status(activity_names[current_activity], 
               String("Rep " + String(current_rep + 1) + "/" + String(REPETITIONS)).c_str());
 }
 
 void finish_activity() {
   Serial.print("\n>>> Completed: ");
   Serial.print(activity_names[current_activity]);
   Serial.print(" Rep ");
   Serial.print(current_rep + 1);
   Serial.print(" (");
   Serial.print(sample_count);
   Serial.println(" samples)");
   
   delay(2000); // Pause between activities
   
   current_rep++;
   
   // Check if all reps done for this activity
   if (current_rep >= REPETITIONS) {
     current_rep = 0;
     current_activity = (Activity)((int)current_activity + 1);
     
     // Check if all activities done
     if (current_activity > GAMING) {
       collection_complete();
       return;
     }
   }
   
   start_new_activity();
 }
 
 void collection_complete() {
   collection_active = false;
   
   Serial.println("\n╔══════════════════════════════════════╗");
   Serial.println("║      DATA COLLECTION COMPLETE        ║");
   Serial.println("╚══════════════════════════════════════╝");
   Serial.println("\nTotal samples collected: " + String(sample_count));
   Serial.println("Activities: 6");
   Serial.println("Reps per activity: " + String(REPETITIONS));
   Serial.println("Expected total: " + String(6 * REPETITIONS * SAMPLES_PER_ACTIVITY));
   
   show_status("COMPLETE!", "Check Serial");
   
   // Celebrate
   for(int i = 0; i < 3; i++) {
     u8g2.clearBuffer();
     u8g2.setFont(u8g2_font_logisoso16_tr);
     u8g2.drawStr(30, 32, "DONE!");
     u8g2.sendBuffer();
     delay(300);
     u8g2.clearBuffer();
     u8g2.sendBuffer();
     delay(300);
   }
 }
 
 // ============ DISPLAY FUNCTIONS ============
 void show_status(const char* line1, const char* line2) {
   u8g2.clearBuffer();
   u8g2.setFont(u8g2_font_7x13B_tr);
   u8g2.drawStr(0, 16, line1);
   u8g2.setFont(u8g2_font_6x10_tr);
   u8g2.drawStr(0, 35, line2);
   u8g2.sendBuffer();
 }
 
 void update_display() {
   u8g2.clearBuffer();
   
   // Activity name
   u8g2.setFont(u8g2_font_7x13B_tr);
   u8g2.drawStr(0, 12, activity_names[current_activity]);
   
   // Progress bar
   int progress = (sample_count * 100) / SAMPLES_PER_ACTIVITY;
   u8g2.drawFrame(0, 18, 128, 10);
   u8g2.drawBox(2, 20, (progress * 124) / 100, 6);
   
   // Stats
   u8g2.setFont(u8g2_font_5x7_tr);
   u8g2.setCursor(0, 40);
   u8g2.print("Samples: ");
   u8g2.print(sample_count);
   u8g2.print("/");
   u8g2.print(SAMPLES_PER_ACTIVITY);
   
   u8g2.setCursor(0, 50);
   u8g2.print("Rep: ");
   u8g2.print(current_rep + 1);
   u8g2.print("/");
   u8g2.print(REPETITIONS);
   
   u8g2.setCursor(0, 60);
   u8g2.print("Activity: ");
   u8g2.print((int)current_activity + 1);
   u8g2.print("/6");
   
   u8g2.sendBuffer();
 }