/*
 * ESP32 Side-Channel Data Collection - OPTIMIZED VERSION
 * Fits in default partition (1.3MB)
 * Removed: WebSockets, BluetoothSerial (they're huge libraries)
 * 
 * WIRING:
 * ESP32 3.3V → 10kΩ → GPIO34 (ADC) → 10kΩ → GND
 * OLED: SDA=21, SCL=22, VCC=3.3V, GND=GND
 */

 #include <WiFi.h>
 #include <HTTPClient.h>
 #include <Wire.h>
 #include <U8g2lib.h>
 
 // ============ CONFIGURATION ============
 #define WIFI_SSID "xxxxx"
 #define WIFI_PASSWORD "xxxxxx"
 
 #define ADC_PIN 34
 #define SDA_PIN 21
 #define SCL_PIN 22
 
 #define SAMPLE_RATE 200          // 200 Hz sampling
 #define SAMPLES_PER_ACTIVITY 6000 // 30 seconds @ 200Hz
 #define REPETITIONS 5            // 5 samples per activity
 
 // ============ HARDWARE ============
 U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
 HTTPClient http;
 WiFiClient client;
 WiFiUDP udp;
 
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
 unsigned long last_action = 0;
 
 // ============ SETUP ============
 void setup() {
   Serial.begin(115200);
  Serial.setTimeout(10);
   delay(2000);
   
   Serial.println("\n╔════════════════════════════════════════╗");
   Serial.println("║  RF Side-Channel Data Collector       ║");
   Serial.println("║  TinyML Training Dataset (Optimized)   ║");
   Serial.println("╚════════════════════════════════════════╝\n");
   
   // Configure ADC
   analogReadResolution(12);
   analogSetAttenuation(ADC_11db);
   Serial.println("✓ ADC: GPIO34, 12-bit, 200Hz");
   
   // Initialize OLED
   Wire.begin(SDA_PIN, SCL_PIN);
   u8g2.begin();
   u8g2.setContrast(200);
   show_status("Initializing...", "");
   
   // Connect WiFi
   Serial.print("Connecting WiFi...");
   WiFi.mode(WIFI_STA);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   
   int attempts = 0;
   while (WiFi.status() != WL_CONNECTED && attempts < 40) {
     delay(500);
     Serial.print(".");
     attempts++;
   }
   
   if(WiFi.status() == WL_CONNECTED) {
     Serial.println("\n✓ WiFi connected");
     Serial.print("IP: ");
     Serial.println(WiFi.localIP());
     Serial.print("RSSI: ");
     Serial.println(WiFi.RSSI());
     show_status("WiFi Connected", WiFi.localIP().toString().c_str());
   } else {
     Serial.println("\n⚠ WiFi failed - continuing anyway");
     show_status("WiFi Failed", "Check credentials");
   }
   
   delay(2000);
   
   // Print CSV header
   Serial.println("\n=== DATA COLLECTION START ===");
   Serial.println("timestamp_ms,voltage_mv,rssi_dbm,activity_id,activity_name");
   
   // Wait for user
   show_status("Ready to Start", "Press any key...");
   Serial.println("\nPress ANY KEY to start collection...");
   
   while (!Serial.available()) {
     delay(100);
   }
   while(Serial.available()) Serial.read(); // Clear buffer
   
   collection_active = true;
   current_activity = IDLE;
   current_rep = 0;
   
   Serial.println("\n>>> Starting Collection <<<");
   start_new_activity();
 }
 
 // ============ MAIN LOOP ============
 void loop() {
  check_control_commands();
  
   if (!collection_active) {
     show_status("Collection Done!", "Check Serial");
     delay(1000);
     return;
   }
   
   unsigned long now = millis();
   
   // Sample at 200Hz
   static unsigned long last_sample = 0;
   if (now - last_sample >= 5) {
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
     
     // Update display every 100 samples
     if (sample_count % 100 == 0) {
       update_display();
     }
   }
   
   // Execute activity behavior
   execute_activity();
   
   // Check if done
   if (sample_count >= SAMPLES_PER_ACTIVITY) {
     finish_activity();
   }
 }
 
 // ============ ACTIVITY BEHAVIORS ============
 void execute_activity() {
   unsigned long now = millis();
   
   switch(current_activity) {
     case IDLE:
       if (now - last_action > 5000) {
         WiFi.RSSI();
         last_action = now;
       }
       break;
       
     case SOCIAL_MEDIA:
       // Bursty: image loads
       if (now - last_action > random(1000, 3000)) {
         http_get(random(50000, 200000));
         delay(random(100, 300));
         http_get(random(5000, 20000));
         last_action = now;
       }
       break;
       
     case VIDEO_STREAMING:
       // Continuous chunks
       if (now - last_action > 40) {
         http_get(1400);
         last_action = now;
       }
       break;
       
     case MESSAGING:
       // Small frequent
       if (now - last_action > random(2000, 8000)) {
         http_get(random(100, 500));
         
         // Occasional voice (20%)
         if (random(0, 5) == 0) {
           delay(100);
           http_get(random(50000, 150000));
         }
         last_action = now;
       }
       break;
       
     case WEB_BROWSING:
       // Page load burst
       if (now - last_action > random(3000, 8000)) {
         http_get(random(10000, 50000));
         delay(random(200, 500));
         http_get(random(50000, 200000));
         last_action = now;
       }
       break;
       
     case GAMING:
       // High freq + CPU load
       if (now - last_action > 20) {
         // UDP burst
         udp_send(random(50, 150));
         
         // CPU load (game physics)
         volatile float dummy = 0;
         for(int i = 0; i < 5000; i++) {
           dummy += sqrt(i) * sin(i * 0.01);
         }
         
         // Voice (10%)
         if (random(0, 10) == 0) {
           http_get(random(20000, 50000));
         }
         
         last_action = now;
       }
       break;
   }
 }
 
 // ============ NETWORK FUNCTIONS ============
 void http_get(int size_bytes) {
   if (WiFi.status() != WL_CONNECTED) return;
   
   String url = "http://httpbin.org/bytes/" + String(size_bytes);
   
   http.begin(client, url);
   http.setTimeout(2000);
   int httpCode = http.GET();
   http.end();
 }
 
 void udp_send(int size_bytes) {
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
   last_action = millis();
   
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
   
   delay(2000);
   
   current_rep++;
   
   if (current_rep >= REPETITIONS) {
     current_rep = 0;
     current_activity = (Activity)((int)current_activity + 1);
     
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
   Serial.print("\nTotal samples: ");
   Serial.println(sample_count);
   Serial.println("Activities: 6");
   Serial.print("Reps per activity: ");
   Serial.println(REPETITIONS);
   Serial.print("Expected total: ");
   Serial.println(6 * REPETITIONS * SAMPLES_PER_ACTIVITY);
   
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
 
// ============ HOST CONTROL ============
void check_control_commands() {
  if (!Serial.available()) return;
  
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  
  if (cmd.equalsIgnoreCase("STOP")) {
    collection_active = false;
    Serial.println("\n>>> STOP received - halting collection <<<");
    show_status("STOPPED", "Host request");
  }
}

 // ============ DISPLAY ============
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
   
   // Activity
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