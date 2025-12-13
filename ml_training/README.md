## Phase 2: ML Training

Complete Python Pipeline (ml_training_pipeline.py) - One script does everything:

- Data collection from ESP32
- Feature engineering (30 features: time + frequency + wavelet)
- Train 4 models (Random Forest, SVM, Decision Tree, CNN)
- TFLite conversion with INT8 quantization
- Arduino header generation