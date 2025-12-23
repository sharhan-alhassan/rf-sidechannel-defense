import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('data/raw/dataset.csv')
print(df.describe())
print(df['activity_name'].value_counts())

df.groupby('activity_name')['voltage_mv'].plot(legend=True)
plt.savefig('data_preview.png')
print('✓ Saved data_preview.png')
