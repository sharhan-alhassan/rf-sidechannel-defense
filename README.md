# rf-sidechannel-defense

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

## Questions
- Which part is the defense or mitigatoin output

