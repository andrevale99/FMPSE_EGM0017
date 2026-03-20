import matplotlib.pyplot as plt
import numpy as np
from numpy import pi
from scipy import signal
import os
import pandas as pd

# ==========================
# DADOS COLETADOS
# ==========================
file1 = "MotorDC/csv/rpm_data_20260320_123617.csv"
file2 = "MotorDC/csv/rpm_data_20260320_123711.csv"

data = pd.read_csv(file2)

Samples = len(data['rpm'])

timeMax = Samples*100e-6
timeVect = np.linspace(0, timeMax, Samples)

# ================================
# Modelagem por Espaco de Estados
# ================================

Ra = 2.5 #Ohm
La = 5.5e-3 #H
J = 0.00025
B = 1.9e-3
Ke = 0.31
Kt = Ke
Kte = Kt

A = np.array([
    [-Ra/La, -Kte/La, 0],
    [Kte/J, -B/J, 0],
    [0,1,0]
])

B = np.array([[1/La],[0],[0]])

# x = [ia, wa, theta]
C = np.eye(3)
C = C[1,:]

D = 0

# ===========================
# Resposta da planta
# ===========================
u = np.zeros_like(timeVect)
u[timeVect >= 0] = 16.7

sys = signal.StateSpace(A, B, C, D)
t,y,_ = signal.lsim(sys, U=u, T=timeVect)

plt.plot(timeVect, data)
plt.plot(timeVect,y, label='aprox', ls='--')
# plt.plot(t, u)
plt.grid()
plt.legend()
plt.show()