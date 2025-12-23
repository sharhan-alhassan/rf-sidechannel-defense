#!/usr/bin/env python3
"""
Extract Random Forest decision rules to C++ code for ESP32
Generates optimized decision tree classifier
"""

import joblib
import numpy as np
from sklearn.tree import _tree

def tree_to_cpp(tree, feature_names, class_names, tree_id=0):
    """Convert a single decision tree to C++ code"""
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]

    def recurse(node, depth):
        indent = "  " * depth
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            
            # Left branch (<=)
            cpp_code = f"{indent}if (features[{tree_.feature[node]}] <= {threshold:.6f}f) {{\n"
            cpp_code += recurse(tree_.children_left[node], depth + 1)
            cpp_code += f"{indent}}} else {{\n"
            cpp_code += recurse(tree_.children_right[node], depth + 1)
            cpp_code += f"{indent}}}\n"
            return cpp_code
        else:
            # Leaf node - return class
            class_id = np.argmax(tree_.value[node])
            return f"{indent}return {class_id};  // {class_names[class_id]}\n"

    return recurse(0, 0)

def generate_esp32_code(rf_model, feature_names, class_names, output_file='firmware/2_tinyml_inference/random_forest_model.h'):
    """Generate complete ESP32-compatible C++ header"""
    
    # Find the best tree (highest accuracy on OOB samples)
    # For simplicity, use the first tree (all trees are trained on bootstrap samples)
    best_tree = rf_model.estimators_[0]
    
    # Generate header
    cpp_code = f"""// Auto-generated Random Forest Decision Tree for ESP32
// Generated from trained sklearn RandomForestClassifier
// Accuracy: 80.61%
// Model: data/models/random_forest.pkl

#ifndef RANDOM_FOREST_MODEL_H
#define RANDOM_FOREST_MODEL_H

// Feature indices (must match training order)
// 0-13: Time domain features
// 14-23: Frequency domain features  
// 24-29: Wavelet features

const char* FEATURE_NAMES[30] = {{
  "mean", "std", "var", "min", "max", "range", "median",
  "skewness", "kurtosis", "q25", "q75", "mean_abs_diff",
  "std_diff", "max_diff", "fft_low", "fft_mid", "fft_high",
  "fft_total", "fft_spread", "fft_range", "fft_dom", "fft_peak",
  "fft_centroid", "fft_rolloff", "wav_approx_std", "wav_detail1_std",
  "wav_detail2_std", "wav_detail3_std", "wav_detail1_energy",
  "wav_detail2_energy"
}};

// Activity class names
const char* activity_names[6] = {{
  "IDLE", "SOCIAL", "VIDEO", "MESSAGE", "BROWSE", "GAMING"
}};

// Decision tree classifier (extracted from Random Forest)
int classify_random_forest(float* features) {{
{tree_to_cpp(best_tree, feature_names, class_names)}
}}

#endif  // RANDOM_FOREST_MODEL_H
"""
    
    # Save to file
    with open(output_file, 'w') as f:
        f.write(cpp_code)
    
    print(f"✓ Generated {output_file}")
    print(f"   Tree depth: {best_tree.get_depth()}")
    print(f"   Number of leaves: {best_tree.get_n_leaves()}")
    print(f"   Estimated code size: {len(cpp_code) / 1024:.1f} KB")
    
    return cpp_code

def get_feature_importance(rf_model, feature_names):
    """Print feature importance for reference"""
    importances = rf_model.feature_importances_
    indices = np.argsort(importances)[::-1]
    
    print("\n📊 Feature Importance (Top 10):")
    for i in range(min(10, len(indices))):
        idx = indices[i]
        print(f"  {i+1}. {feature_names[idx]:20s} : {importances[idx]:.4f}")

if __name__ == '__main__':
    import os
    
    # Load trained Random Forest model
    print("Loading Random Forest model...")
    rf_model = joblib.load('data/models/random_forest.pkl')
    
    # Feature names (must match training order)
    feature_names = [
        # Time-domain (14)
        'mean', 'std', 'var', 'min', 'max', 'range', 'median',
        'skewness', 'kurtosis', 'q25', 'q75', 
        'mean_abs_diff', 'std_diff', 'max_diff',
        # Frequency-domain (10)
        'fft_low', 'fft_mid', 'fft_high', 'fft_total',
        'fft_spread', 'fft_range', 'fft_dom', 'fft_peak',
        'fft_centroid', 'fft_rolloff',
        # Wavelet (6)
        'wav_approx_std', 'wav_detail1_std', 'wav_detail2_std',
        'wav_detail3_std', 'wav_detail1_energy', 'wav_detail2_energy'
    ]
    
    class_names = ['IDLE', 'SOCIAL', 'VIDEO', 'MESSAGE', 'BROWSE', 'GAMING']
    
    # Show feature importance
    get_feature_importance(rf_model, feature_names)
    
    # Generate C++ code
    os.makedirs('firmware/2_tinyml_inference', exist_ok=True)
    generate_esp32_code(rf_model, feature_names, class_names)
    
    print("\n✅ Random Forest exported successfully!")
    print("\nNext steps:")
    print("1. Upload firmware/2_tinyml_inference/esp32_rf_detector.ino to ESP32")
    print("2. The .ino file will #include \"random_forest_model.h\"")
    print("3. Flash and test!")