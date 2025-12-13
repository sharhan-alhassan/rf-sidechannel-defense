#!/usr/bin/env python3
"""
Complete ML Training Pipeline for RF Side-Channel Detection
M4 MacBook - From CSV to TFLite Model

Usage:
    python ml_training_pipeline.py --mode collect   # Collect from ESP32
    python ml_training_pipeline.py --mode train     # Train all models
    python ml_training_pipeline.py --mode convert   # Convert to TFLite
    python ml_training_pipeline.py --mode all       # Full pipeline
"""

import serial
import serial.tools.list_ports
import argparse
import numpy as np
import pandas as pd
from datetime import datetime
import os
import json
import time

# ML libraries
from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
from sklearn.preprocessing import StandardScaler
import joblib

# Deep learning
import tensorflow as tf
from tensorflow import keras

# Signal processing
from scipy import signal, stats
import pywt

# Visualization
import matplotlib.pyplot as plt
import seaborn as sns

# ============ CONFIGURATION ============
SAMPLE_RATE = 200  # Hz
WINDOW_SIZE = 100  # 0.5 seconds @ 200Hz
OVERLAP = 0.5      # 50% overlap

ACTIVITIES = {
    0: 'IDLE',
    1: 'SOCIAL',
    2: 'VIDEO',
    3: 'MESSAGE',
    4: 'BROWSE',
    5: 'GAMING'
}

# ============ DATA COLLECTION ============
class DataCollector:
    def __init__(self, port=None, baudrate=115200):
        if port is None:
            port = self.find_esp32_port()
        
        print(f"📡 Connecting to {port}...")
        self.ser = serial.Serial(port, baudrate, timeout=1)
        
        # Allow ESP32 to reboot after DTR/RTS toggle, clear junk, and trigger start gate
        time.sleep(2)
        self.ser.reset_input_buffer()
        self.ser.write(b'\n')
        self.ser.flush()
        
        print("✓ Connected to ESP32")
        
    def find_esp32_port(self):
        """Auto-detect ESP32 on macOS"""
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if 'usb' in port.device.lower() or 'serial' in port.device.lower():
                return port.device
        
        # Manual selection
        print("\nAvailable ports:")
        for i, port in enumerate(ports):
            print(f"  {i+1}. {port.device}: {port.description}")
        
        choice = int(input("\nSelect port number: ")) - 1
        return list(ports)[choice].device
    
    def collect_dataset(self, output_file='data/raw/dataset.csv'):
        """Collect data from ESP32 via serial"""
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        
        print("\n╔════════════════════════════════════════╗")
        print("║      Data Collection from ESP32        ║")
        print("╚════════════════════════════════════════╝")
        print(f"\nSaving to: {output_file}")
        print("Press Ctrl+C to stop\n")
        
        with open(output_file, 'w') as f:
            # Wait for CSV header from ESP32
            while True:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                if line.startswith('timestamp_ms'):
                    f.write(line + '\n')
                    print(f"✓ CSV Header: {line}")
                    break
            
            # Collect data
            sample_count = 0
            try:
                while True:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    
                    if line and ',' in line and not line.startswith('='):
                        parts = line.split(',')
                        if len(parts) == 5:
                            f.write(line + '\n')
                            sample_count += 1
                            
                            if sample_count % 1000 == 0:
                                print(f"Samples: {sample_count:6d} | {parts[4]}")
            
            except KeyboardInterrupt:
                print("\n\n↪ Sending STOP to ESP32...")
                try:
                    self.ser.write(b'STOP\n')
                    self.ser.flush()
                    time.sleep(0.1)
                except Exception as e:
                    print(f"⚠ Failed to send STOP: {e}")
                
                print("✓ Collection stopped")
                print(f"Total samples: {sample_count}")
            
            finally:
                try:
                    self.ser.close()
                except Exception:
                    pass
            
            return output_file

# ============ FEATURE ENGINEERING ============
class FeatureExtractor:
    @staticmethod
    def extract_time_domain(window):
        """Extract 14 time-domain features"""
        return [
            np.mean(window),
            np.std(window),
            np.var(window),
            np.min(window),
            np.max(window),
            np.ptp(window),  # peak-to-peak
            np.median(window),
            stats.skew(window),
            stats.kurtosis(window),
            np.percentile(window, 25),
            np.percentile(window, 75),
            np.mean(np.abs(np.diff(window))),
            np.std(np.diff(window)),
            np.max(np.abs(np.diff(window)))
        ]
    
    @staticmethod
    def extract_frequency_domain(window):
        """Extract 10 frequency-domain features"""
        fft = np.fft.fft(window)
        fft_mag = np.abs(fft[:len(window)//2])
        
        # Spectral features
        spectral_centroid = np.sum(np.arange(len(fft_mag)) * fft_mag) / np.sum(fft_mag)
        spectral_rolloff = np.where(np.cumsum(fft_mag) >= 0.85 * np.sum(fft_mag))[0][0]
        spectral_entropy = -np.sum((fft_mag / np.sum(fft_mag)) * np.log2(fft_mag / np.sum(fft_mag) + 1e-10))
        
        return [
            np.argmax(fft_mag),           # Dominant frequency
            np.max(fft_mag),              # Peak magnitude
            np.mean(fft_mag),             # Average magnitude
            np.std(fft_mag),              # Frequency variance
            np.sum(fft_mag[:10]),         # Low-freq energy
            np.sum(fft_mag[10:50]),       # Mid-freq energy
            np.sum(fft_mag[50:]),         # High-freq energy
            spectral_centroid,
            spectral_rolloff,
            spectral_entropy
        ]
    
    @staticmethod
    def extract_wavelet_features(window):
        """Extract 6 wavelet features"""
        coeffs = pywt.wavedec(window, 'db4', level=3)
        
        return [
            np.std(coeffs[0]),
            np.std(coeffs[1]),
            np.std(coeffs[2]),
            np.std(coeffs[3]),
            np.sum(np.abs(coeffs[1])),
            np.sum(np.abs(coeffs[2]))
        ]
    
    def extract_all_features(self, window):
        """Extract all 30 features"""
        features = []
        features.extend(self.extract_time_domain(window))
        features.extend(self.extract_frequency_domain(window))
        features.extend(self.extract_wavelet_features(window))
        return features
    
    def process_dataset(self, csv_file):
        """Convert raw CSV to feature matrix"""
        print(f"\n📊 Processing {csv_file}...")
        
        df = pd.read_csv(csv_file)
        print(f"Raw samples: {len(df)}")
        
        X, y = [], []
        step = int(WINDOW_SIZE * (1 - OVERLAP))
        
        for i in range(0, len(df) - WINDOW_SIZE, step):
            window_data = df.iloc[i:i+WINDOW_SIZE]
            voltage = window_data['voltage_mv'].values
            activity = window_data['activity_id'].iloc[0]
            
            # Extract features
            features = self.extract_all_features(voltage)
            
            X.append(features)
            y.append(activity)
            
            if len(X) % 500 == 0:
                print(f"Processed {len(X)} windows...")
        
        X = np.array(X)
        y = np.array(y)
        
        print(f"\n✓ Feature extraction complete")
        print(f"Feature matrix: {X.shape}")
        print(f"Labels: {y.shape}")
        print(f"Features per sample: {X.shape[1]}")
        
        # Save processed data
        os.makedirs('data/processed', exist_ok=True)
        np.save('data/processed/X.npy', X)
        np.save('data/processed/y.npy', y)
        
        return X, y

# ============ MODEL TRAINING ============
class ModelTrainer:
    def __init__(self, X_train, X_test, y_train, y_test):
        self.X_train = X_train
        self.X_test = X_test
        self.y_train = y_train
        self.y_test = y_test
        self.models = {}
        self.results = {}
    
    def train_random_forest(self):
        """Train Random Forest"""
        print("\n🌲 Training Random Forest...")
        
        rf = RandomForestClassifier(
            n_estimators=100,
            max_depth=15,
            min_samples_split=5,
            class_weight='balanced',
            random_state=42,
            n_jobs=-1
        )
        
        rf.fit(self.X_train, self.y_train)
        y_pred = rf.predict(self.X_test)
        accuracy = accuracy_score(self.y_test, y_pred)
        
        self.models['random_forest'] = rf
        self.results['random_forest'] = {
            'accuracy': accuracy,
            'predictions': y_pred
        }
        
        print(f"✓ Random Forest Accuracy: {accuracy*100:.2f}%")
        
        # Feature importance
        importances = rf.feature_importances_
        print(f"Top 5 features: {np.argsort(importances)[-5:][::-1]}")
        
        return rf, accuracy
    
    def train_svm(self):
        """Train SVM"""
        print("\n⚡ Training SVM...")
        
        # Scale features for SVM
        scaler = StandardScaler()
        X_train_scaled = scaler.fit_transform(self.X_train)
        X_test_scaled = scaler.transform(self.X_test)
        
        svm = SVC(
            kernel='rbf',
            C=10.0,
            gamma='scale',
            class_weight='balanced',
            random_state=42
        )
        
        svm.fit(X_train_scaled, self.y_train)
        y_pred = svm.predict(X_test_scaled)
        accuracy = accuracy_score(self.y_test, y_pred)
        
        self.models['svm'] = {'model': svm, 'scaler': scaler}
        self.results['svm'] = {
            'accuracy': accuracy,
            'predictions': y_pred
        }
        
        print(f"✓ SVM Accuracy: {accuracy*100:.2f}%")
        
        return svm, accuracy
    
    def train_decision_tree(self):
        """Train Decision Tree"""
        print("\n🌳 Training Decision Tree...")
        
        dt = DecisionTreeClassifier(
            max_depth=12,
            min_samples_split=10,
            class_weight='balanced',
            random_state=42
        )
        
        dt.fit(self.X_train, self.y_train)
        y_pred = dt.predict(self.X_test)
        accuracy = accuracy_score(self.y_test, y_pred)
        
        self.models['decision_tree'] = dt
        self.results['decision_tree'] = {
            'accuracy': accuracy,
            'predictions': y_pred
        }
        
        print(f"✓ Decision Tree Accuracy: {accuracy*100:.2f}%")
        
        return dt, accuracy
    
    def train_cnn(self):
        """Train 1D-CNN"""
        print("\n🧠 Training 1D-CNN...")
        
        # Reshape for CNN
        X_train_cnn = self.X_train.reshape((-1, 30, 1))
        X_test_cnn = self.X_test.reshape((-1, 30, 1))
        
        # Build model
        model = keras.Sequential([
            keras.layers.Conv1D(16, 3, activation='relu', input_shape=(30, 1)),
            keras.layers.MaxPooling1D(2),
            keras.layers.Conv1D(32, 3, activation='relu'),
            keras.layers.GlobalAveragePooling1D(),
            keras.layers.Dense(64, activation='relu'),
            keras.layers.Dropout(0.3),
            keras.layers.Dense(6, activation='softmax')
        ])
        
        model.compile(
            optimizer='adam',
            loss='sparse_categorical_crossentropy',
            metrics=['accuracy']
        )
        
        print("Model architecture:")
        model.summary()
        
        # Train
        history = model.fit(
            X_train_cnn, self.y_train,
            validation_split=0.2,
            epochs=50,
            batch_size=32,
            verbose=1
        )
        
        # Evaluate
        y_pred_prob = model.predict(X_test_cnn)
        y_pred = np.argmax(y_pred_prob, axis=1)
        accuracy = accuracy_score(self.y_test, y_pred)
        
        self.models['cnn'] = model
        self.results['cnn'] = {
            'accuracy': accuracy,
            'predictions': y_pred,
            'history': history.history
        }
        
        print(f"✓ CNN Accuracy: {accuracy*100:.2f}%")
        
        return model, accuracy
    
    def train_all(self):
        """Train all 4 models"""
        self.train_random_forest()
        self.train_svm()
        self.train_decision_tree()
        self.train_cnn()
        
        return self.models, self.results
    
    def save_models(self):
        """Save all trained models"""
        os.makedirs('data/models', exist_ok=True)
        
        # Sklearn models
        joblib.dump(self.models['random_forest'], 'data/models/random_forest.pkl')
        joblib.dump(self.models['svm'], 'data/models/svm.pkl')
        joblib.dump(self.models['decision_tree'], 'data/models/decision_tree.pkl')
        
        # Keras model
        self.models['cnn'].save('data/models/cnn_model.h5')
        
        print("\n✓ All models saved to data/models/")
    
    def generate_report(self):
        """Generate evaluation report"""
        print("\n" + "="*60)
        print("MODEL COMPARISON REPORT")
        print("="*60)
        
        for model_name, result in self.results.items():
            print(f"\n{model_name.upper()}:")
            print(f"  Accuracy: {result['accuracy']*100:.2f}%")
            print("\nClassification Report:")
            print(classification_report(
                self.y_test,
                result['predictions'],
                target_names=list(ACTIVITIES.values())
            ))
        
        # Plot comparison
        self.plot_comparison()
    
    def plot_comparison(self):
        """Plot model comparison"""
        os.makedirs('evaluation', exist_ok=True)
        
        # Accuracy comparison
        fig, axes = plt.subplots(2, 2, figsize=(14, 10))
        
        for idx, (model_name, result) in enumerate(self.results.items()):
            ax = axes[idx // 2, idx % 2]
            
            cm = confusion_matrix(self.y_test, result['predictions'])
            sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', ax=ax,
                       xticklabels=list(ACTIVITIES.values()),
                       yticklabels=list(ACTIVITIES.values()))
            ax.set_title(f'{model_name.upper()}\nAccuracy: {result["accuracy"]*100:.1f}%')
            ax.set_ylabel('True')
            ax.set_xlabel('Predicted')
        
        plt.tight_layout()
        plt.savefig('evaluation/model_comparison.png', dpi=300)
        print("\n✓ Saved evaluation/model_comparison.png")
        
        plt.close()

# ============ TFLITE CONVERSION ============
class TFLiteConverter:
    def __init__(self, keras_model):
        self.keras_model = keras_model
    
    def convert_with_quantization(self, X_train):
        """Convert to TFLite with INT8 quantization"""
        print("\n⚙️  Converting to TensorFlow Lite...")
        
        converter = tf.lite.TFLiteConverter.from_keras_model(self.keras_model)
        
        # INT8 quantization
        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        converter.target_spec.supported_types = [tf.int8]
        
        # Representative dataset for quantization
        def representative_dataset():
            for i in range(min(100, len(X_train))):
                sample = X_train[i:i+1].reshape((1, 30, 1)).astype(np.float32)
                yield [sample]
        
        converter.representative_dataset = representative_dataset
        converter.inference_input_type = tf.int8
        converter.inference_output_type = tf.int8
        
        tflite_model = converter.convert()
        
        # Save
        with open('data/models/model.tflite', 'wb') as f:
            f.write(tflite_model)
        
        size_kb = len(tflite_model) / 1024
        print(f"✓ TFLite model saved: {size_kb:.1f} KB")
        
        return tflite_model
    
    def generate_arduino_header(self, tflite_model):
        """Generate Arduino .h file"""
        print("\n📝 Generating Arduino header...")
        
        # Convert to C array
        hex_array = ','.join([f'0x{byte:02x}' for byte in tflite_model])
        
        # Wrap in chunks of 12 bytes per line
        wrapped = []
        for i in range(0, len(hex_array.split(',')), 12):
            chunk = ','.join(hex_array.split(',')[i:i+12])
            wrapped.append(f"  {chunk}")
        
        header_content = f"""// Auto-generated TensorFlow Lite model for ESP32
// Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
// Model size: {len(tflite_model)} bytes ({len(tflite_model)/1024:.1f} KB)

#ifndef MODEL_H
#define MODEL_H

const unsigned char model_data[] = {{
{chr(10).join(wrapped)}
}};

const int model_data_len = {len(tflite_model)};

// Activity labels
const char* activity_names[] = {{
  "IDLE", "SOCIAL", "VIDEO", "MESSAGE", "BROWSE", "GAMING"
}};

#endif  // MODEL_H
"""
        
        # Save
        with open('firmware/2_tinyml_inference/model.h', 'w') as f:
            f.write(header_content)
        
        print("✓ Generated firmware/2_tinyml_inference/model.h")
        print(f"   Model size: {len(tflite_model)/1024:.1f} KB")

# ============ MAIN PIPELINE ============
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--mode', choices=['collect', 'train', 'convert', 'all'], 
                       default='all', help='Pipeline mode')
    args = parser.parse_args()
    
    if args.mode in ['collect', 'all']:
        # Phase 1: Data Collection
        collector = DataCollector()
        dataset_file = collector.collect_dataset()
    else:
        dataset_file = 'data/raw/dataset.csv'
    
    if args.mode in ['train', 'convert', 'all']:
        # Phase 2: Feature Engineering
        extractor = FeatureExtractor()
        X, y = extractor.process_dataset(dataset_file)
        
        # Split data
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=42, stratify=y
        )
        
        print(f"\nDataset split:")
        print(f"  Train: {len(X_train)} samples")
        print(f"  Test:  {len(X_test)} samples")
        
        # Phase 3: Model Training
        trainer = ModelTrainer(X_train, X_test, y_train, y_test)
        models, results = trainer.train_all()
        trainer.save_models()
        trainer.generate_report()
    
    if args.mode in ['convert', 'all']:
        # Phase 4: TFLite Conversion
        cnn_model = keras.models.load_model('data/models/cnn_model.h5')
        X_train = np.load('data/processed/X.npy')
        
        converter = TFLiteConverter(cnn_model)
        tflite_model = converter.convert_with_quantization(X_train)
        converter.generate_arduino_header(tflite_model)
        
        print("\n" + "="*60)
        print("PIPELINE COMPLETE!")
        print("="*60)
        print("\nNext steps:")
        print("1. Upload firmware/2_tinyml_inference/esp32_tinyml_detector.ino")
        print("2. Model is in firmware/2_tinyml_inference/model.h")
        print("3. Flash to ESP32 and test!")

if __name__ == '__main__':
    main()