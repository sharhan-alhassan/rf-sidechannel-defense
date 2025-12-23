# RF Side-Channel Defense via On-Device Machine Learning
## Self-Introspection-Based Side-Channel Attack Detection for Resource-Constrained IoT Devices


## 📊 Key Results at a Glance
MetricValueSignificanceOverall Accuracy80.6%Across 6 WiFi activity classesHigh-Risk Detection100% F1-scoreVideo streaming & gaming (privacy-critical)Inference Latency18msReal-time detection capabilityModel Size8KBFits in ESP32 flash (4MB available)RAM Usage25KBMinimal footprint (520KB total available)Hardware Cost$0.102× 10kΩ resistors (voltage divider)Dataset Size180,000 samples30K samples per activity class


## 🎯 Project Motivation
The Problem: Side-Channel Attacks on IoT Devices
Recent research (Ni et al., USENIX Security 2023) demonstrates that:

- RF energy harvesting can classify smartphone apps with 98% accuracy
- Wireless charging pads leak user interactions (typing, scrolling, tapping)
- USB power banks reveal video streaming, gaming, and browsing patterns
- Multi-port chargers enable both passive eavesdropping and active voice injection

## The Defense Gap
Existing countermeasures proposed in literature have critical limitations:

- Noise injection: 20-40% performance overhead
- Random delays: Degrades user experience
- External monitoring: Requires additional infrastructure cost

## Our Solution: Self-Introspection Defense
Core Idea: Enable IoT devices to monitor their own power consumption and detect ongoing attacks using on-device machine learning, then trigger adaptive countermeasures only when needed.

### Key Advantages:

✅ Zero additional hardware - just firmware update
✅ Low overhead - mitigation only activates when attack detected
✅ Privacy-preserving - no cloud dependency, all processing on-device
✅ Retrofittable - OTA updates to existing device fleets


## Architecture
```sh
┌─────────────────────────────────────────────────────┐
│           PHASE 1: DATA COLLECTION                  │
│  ESP32 → Realistic WiFi Activities → ADC Sampling   │
│         → Serial CSV → M4 MacBook                   │
└─────────────────────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────────┐
│           PHASE 2: ML TRAINING (M4 Mac)             │
│  Raw CSV → Feature Engineering → Train 4 Models:    │
│  • Random Forest                                    │
│  • SVM (RBF kernel)                                 │
│  • Decision Tree                                    │
│  • 1D-CNN (TensorFlow)                              │
│         → Export to TensorFlow Lite                 │
└─────────────────────────────────────────────────────┘
                      ↓
┌─────────────────────────────────────────────────────┐
│        PHASE 3: EDGE DEPLOYMENT (ESP32)             │
│  TFLite Model → ESP32 Flash → Real-time Inference   │
│         → OLED Display → Detection Alerts           │
└─────────────────────────────────────────────────────┘
```

## Schematic Diagram
```sh
┌────────────────────────────────────────────┐
│           ESP32 DEVKIT V1                  │
├────────────────────────────────────────────┤
│                                            │
│  3.3V ──┬── 10kΩ ──┬── GPIO34 (ADC)       │
│         │          │                       │<-- Voltage divider
│         │          └── 10kΩ ── GND        │     (power side-channel)
│         │                                  │
│         └─> WiFi/BT chip draws power here │
│                                            │
│  GPIO21 (SDA) ───────> OLED SDA            │
│  GPIO22 (SCL) ───────> OLED SCL            │
│  3.3V ───────────────> OLED VCC            │
│  GND ────────────────> OLED GND            │
│                                            │
│  Optional Enhancement:                     │
│  GPIO35 (ADC2) ──> RSSI monitoring         │
│  GPIO25 (DAC)  ──> Mitigation output       │
└────────────────────────────────────────────┘
```

## Prerequisites Checklist (Hardware)
```sh
 ESP32 DevKit V1
 1.3" OLED SH1106 (128x64)
 2× 10kΩ resistors (or any matched pair 1kΩ-100kΩ)
 Jumper wires
 USB-C cable
 Laptop (with decent compute resources)
```

## Prerequisites Checklist (Arduino IDE)
```sh
On Arduino IDE:

Install ESP32 board support:

File → Preferences → Additional Board Manager URLs:
Add: https://dl.espressif.com/dl/package_esp32_index.json
Tools → Board → Boards Manager → Search "ESP32" → Install


Install Libraries (Tools → Manage Libraries):

WebSockets by Markus Sattler
U8g2 by olikraus
EloquentTinyML by Eloquent Arduino (for TinyML inference)
```
## Repo Structure

```md
rf-sidechannel-tinyml/
├── firmware/
│   ├── 1_data_collection/
│   │   └── esp32_datacollector.ino      # Phase 1: Realistic activities
│   ├── 2_tinyml_inference/
│   │   ├── esp32_tinyml_detector.ino    # Phase 3: TFLite inference
│   │   ├── model.h                      # Converted TFLite model
│   │   └── model_data.cc                # Model weights
│   └── lib/
│       └── tensorflow_lite_esp32/        # TFLM library
├── ml_training/
│   ├── 1_data_collection.py             # Collect from ESP32
│   ├── 2_feature_engineering.py         # Extract features
│   ├── 3_train_models.py                # Train 4 models
│   ├── 4_convert_to_tflite.py           # TF → TFLite
│   └── 5_generate_arduino_header.py     # TFLite → .h file
├── data/
│   ├── raw/                             # CSV logs
│   ├── processed/                       # Feature matrices
│   └── models/
│       ├── random_forest.pkl
│       ├── svm.pkl
│       ├── decision_tree.pkl
│       ├── cnn_model.h5
│       └── model.tflite                 # Final TFLite model
├── evaluation/
│   ├── model_comparison.png
│   ├── confusion_matrices/
│   └── metrics.json
├── demo/
│   ├── hardware_photos/
│   ├── demo_video.mp4
│   └── tinyml_inference.gif
├── paper/
│   └── sidechannel_defense_tinyml.pdf
└── README.md
```

## Wiring diagram
```sh
[ COMPUTER ]
      |
      | USB Cable
      v
[ USB BREAKOUT ]
   VBUS (5V) ───────► INA219 VIN+
   GND       ───────► ESP32 GND    + INA219 GND + OLED GND

[ INA219 ]
   VIN+  ◄──── USB 5V
   VIN−  ─────► ESP32 5V pin
   VCC   ─────► ESP32 3.3V
   GND   ─────► ESP32 GND
   SDA   ─────► ESP32 GPIO 21
   SCL   ─────► ESP32 GPIO 22

[ OLED SH1106 ]
   VCC   ─────► ESP32 3.3V
   GND   ─────► ESP32 GND
   SDA   ─────► ESP32 GPIO 21
   SCL   ─────► ESP32 GPIO 22
```

## Data Collection
```sh
# 1. Upload data collection firmware
cd firmware/1_data_collection
# Open esp32_datacollector_optimized_with_interrupt.ino in Arduino IDE
# Upload to ESP32

# 2. Run Python data collector
python3 ml_training/ml_training_pipeline.py --mode collect

# 3. Verify data collection
ls -lh data/raw/
# Should see 6 CSV files, ~3-5MB each
```

## Model Training
```sh
# 1. Run the model trainer
python3 ml_training/ml_training_pipeline.py --mode train

# 2. Train & compare models
# Output: 
# - data/models/*.pkl (scikit-learn models)
# - data/models/cnn_model.h5 (Keras model)
# - evaluation/model_comparison.png
# - evaluation/confusion_matrix.png

```

## Model Conversion
```sh
# 3. Convert best model (Random Forest (RF) to C code)
python3 ml/training/extract_rf_to_cpp.py

# Output: This will generate firmware/2_tinyml_inference/random_forest_model.h with the decision tree in C++ format.
**Expected output:**
Loading Random Forest model...

📊 Feature Importance (Top 10):
  1. q75                  : 0.1245
  2. median               : 0.1132
  3. fft_high             : 0.0987
  4. mean                 : 0.0876
  5. fft_mid              : 0.0754
  ...

✓ Generated firmware/2_tinyml_inference/random_forest_model.h
   Tree depth: 17
   Number of leaves: 142
   Estimated code size: 8.3 KB

✅ Random Forest exported successfully!

# 4.  Extract Random Forest to C++ and upload to ESP32
1. **Open Arduino IDE**
2. **File → Open** → `firmware/2_tinyml_inference/esp32_rf_detector.ino`
3. **Verify** `random_forest_model.h` is in the same folder firmware/2_tinyml_inference
4. **Select Board:** ESP32 Dev Module
5. **Select Port:** Your ESP32 port
6. **Upload**
```

## Test Run
```sh
**Open Serial Monitor (115200 baud):**
╔════════════════════════════════════════╗
║  Random Forest Side-Channel Detector  ║
║  Accuracy: 80.6% | VIDEO/GAMING: 100% ║
╚════════════════════════════════════════╝

✓ ADC configured: GPIO34, 12-bit, 200Hz
✓ OLED initialized: 1.3" SH1106
✓ WiFi connected
IP: 192.168.1.123

🌲 Random Forest Model Loaded
   Algorithm: Decision Tree (from RF ensemble)
   Features: 30 (time + freq + wavelet)
   Classes: 6 activities

=== Real-time Inference Started ===

#1 | IDLE | Latency: 18.23 ms | Avg: 18.23 ms
#2 | IDLE | Latency: 17.89 ms | Avg: 18.06 ms
#3 | SOCIAL | Latency: 18.45 ms | Avg: 18.19 ms
⚠️  MITIGATION: SOCIAL side-channel detected!
#4 | SOCIAL | Latency: 18.12 ms | Avg: 18.17 ms
   [Features] mean=1425.3 std=18.7 range=52.1 energy_mid=892.4

---

## 📊 What You'll See on OLED

### When IDLE:
┌──────────────────────────┐
│ LIVE DETECTION           │
├──────────────────────────┤
│ IDLE                     │
│                          │
│ Latency: 18.2 ms        │
│ Count: 42                │
│ Dist: ████░░░░░░        │
└──────────────────────────┘

### When VIDEO Detected (ALERT):
┏━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃ LIVE DETECTION    ALERT  ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ VIDEO                    ┃ ← Flashing border
┃                          ┃
┃ Latency: 18.5 ms        ┃
┃ Count: 57                ┃
┃ Dist: ░░██░░░░          ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

## Code Style

- C/C++ (Arduino): Follow Google C++ Style Guide
- Python: Follow PEP 8, use Black formatter
- Comments: Explain why, not what (code should be self-documenting)


## 📜 License
This project is licensed under the MIT License - see LICENSE file for details.
TL;DR: You can use, modify, and distribute this code freely, even commercially. Just include the original license.

## 🙏 Acknowledgments

- Professor Tao Ni (KAUST) - Research inspiration from AppListener paper
- TensorFlow Lite Micro Team - Enabling ML on microcontrollers
- Espressif Systems - ESP32 platform and documentation
- Adafruit - INA219 library and hardware support
- Eloquent Arduino - TinyML tutorials and TFLite integration

## 📧 Contact

Author: Sharhan Alhassan

Email: sharhanalhassan@gmail.com

LinkedIn: https://www.linkedin.com/in/sharhan-alhassan1/

GitHub: https://github.com/sharhan-alhassan

Research Interests: CPS Security, MLSys Security, Cloud Security, Embedded AI, IoT and Side-Channel Analysis


## 📖 Citation
If you use this work in your research, please cite:

```sh
bibtex@misc{sidechannel-defense-2024,
  author = {Your Name},
  title = {RF Side-Channel Defense via On-Device Machine Learning},
  year = {2024},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/yourusername/rf-sidechannel-defense}}
}
```

## 🔗 Useful Links

- Technical Report: [ Coming soon...]
- Demo Video: [ Coming soon...]
- Dataset (Google Drive): [Coming soon - 180K samples, ~1GB]
- Pre-trained Models: `data/models/`
- Arduino Library: checks docs


## ⚠️ Disclaimer
This project is for research and educational purposes only. The authors do not condone or encourage:

Unauthorized monitoring of network traffic
Violation of privacy laws or terms of service
Deployment without user consent

Always obtain proper authorization before deploying security monitoring systems.



## Questions
- Which part is the defense or mitigatoin output
- disconnect wifi
- add noise if predicated_class != IDLE

```sh
if (predicted_class != IDLE) {
    // Mitigation: Add noise to power consumption
    digitalWrite(MITIGATION_PIN, HIGH);
    delay(random(10, 50));
    digitalWrite(MITIGATION_PIN, LOW);
}
```