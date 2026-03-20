import matplotlib.pyplot as plt
from scipy import signal
import numpy as np
import pandas as pd

from numpy import pi

# ========================
# FUNCOES
# ========================= 

def plotTheta(timeVect,u,y):
    plt.plot(timeVect, u, label='u(t)', ls='--',color='black')
    plt.plot(timeVect,y, label='theta(t)')
    plt.xlabel("s")
    plt.ylabel("rad")
    plt.grid()
    plt.legend()
    plt.show()


# ==========================
# DADOS COLETADOS
# ==========================
file1 = "MotorDC/csv/rpm_data_20260320_123617.csv"
file2 = "MotorDC/csv/rpm_data_20260320_123711.csv"

data = pd.read_csv(file2)

# Samples = len(data['rpm'])
# timeMax = Samples*100e-6

timeMax = 1
Samples = 1000*timeMax

timeVect = np.linspace(0, timeMax, Samples)

# ================================
# Entrada da planta
# ================================

# Valor de entrada escolhida arbitrariamente
u = np.zeros_like(timeVect)
u[timeVect >= 0] = 16.7 #16.7 (mais ou menos o ganho real)
u[timeVect >= timeMax/2] = 0 #16.7 (mais ou menos o ganho real)
# u[timeVect >= timeMax-1] = 1
# u = np.cos(timeVect*2*pi*2)
# u = 1*timeVect
# u[timeVect>timeMax/2] = timeMax/2

# ================================
# Polos desejados
# =================================

polos = np.array([10+1j, 10-1j, -10])

# ================================
# Modelagem por Espaco de Estados
# ================================

Ra = 2.6 #Ohm
La = 0.006245 #H
J = 0.0002
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
# ia -> Corrente de armadura
# wm -> Velocidade (rad/s) do rotor
# theta -> Posicao (rad) do rotor
C = np.eye(3)
C = C[2,:]

D = 0

# ===========================
# Resposta da planta
# ===========================

sys = signal.StateSpace(A, B, C, D)
t,y,_ = signal.lsim(sys, U=u, T=timeVect)

plotTheta(timeVect,u,y)

# ===============================
#  Realimentacao de estados
# ===============================

# u = -kx
# dx/dt = (A-BK)x + Bu

