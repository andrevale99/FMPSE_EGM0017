import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

os.chdir("MotorDC/csv")

file1 = "rpm_data_20260320_123617.csv"
file2 = "rpm_data_20260320_123711.csv"

data = pd.read_csv(file2)
print(np.max(data))

Samples = len(data['rpm'])

# # 100us
timeMax = Samples*100e-6
print(timeMax)
timeVect = np.linspace(0, timeMax, Samples)

plt.plot(timeVect, data)
plt.grid()
plt.show()