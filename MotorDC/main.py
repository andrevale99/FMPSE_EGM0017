import matplotlib.pyplot as plt
from scipy import signal
import numpy as np
import pandas as pd
import control as ct

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

timeMax = 5
Samples = 1000*timeMax

timeVect = np.linspace(0, timeMax, Samples)

# ================================
# Entrada da planta
# ================================

# Valor de entrada escolhida arbitrariamente
u = np.zeros_like(timeVect)
u[timeVect >= timeMax/2] = 180 #16.7 (mais ou menos o ganho real)

# u = np.cos(timeVect*2*pi*2)
# u = 1*timeVect
# u[timeVect>timeMax/2] = timeMax/2

# ================================
# Polos desejados
# =================================

polos = np.array([-5+30j, -5-30j, -20, -20])

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
C = np.array([[0,0,1]])

D = 0

# ==========================
#  Funcao de transferencia do sistema
# ==========================

num, den = signal.ss2tf(A, B, C, D, input=0)
print()
print("Funcao de transferencia do sistema:")
print(f'num = {num}')
print(f'den = {den}')
print()

# ===========================
# Resposta da planta
# ===========================

sys = signal.StateSpace(A, B, C, D)
t,y,_ = signal.lsim(sys, U=u, T=timeVect)

# plotTheta(timeVect,u,y)

# ===============================
#  Realimentacao de estados
# ===============================

# Matriz de Controlabilidade
U = np.concatenate((B, A @ B, np.linalg.matrix_power(A,2)@B), axis=1)
rank_U = np.linalg.matrix_rank(U)
print()
print("\nMatriz de Controlabilidade U:\n", U)
print("Posto da Matriz de Controlabilidade:", rank_U)
print()

# u = -kx
# dx/dt = (A-BK)x + Bu
print()
print(f'Polos = {polos}')

# Expandindo as matrizes para incluir o integrador
Ahat = np.concatenate((A, np.zeros((A.shape[0],1))), axis=1)
temp = np.concatenate((-C, np.array([[0]])), axis=1)
Ahat = np.concatenate((Ahat, temp), axis=0)
Bhat = np.concatenate((B, np.array([[0]])), axis=0)

del temp

K = ct.acker(Ahat, Bhat, polos)
KI = K[len(K)-1]
K = np.copy(K[0:len(K)-1]).reshape(1,A.shape[0])
print()
print("Ganho de realimentação de estado K:\n", K)
print("Ganho de realimentação de estado KI:\n", KI)
print()

# Aplicando no sistema realimentado
A00 = A - (B * K)
A01 = B * -KI
A10 = -C
A11 = np.array([[0]])
    
Af = np.concatenate((A00, A01), axis=1)
temp = np.concatenate((A10, A11), axis=1)
Af = np.concatenate((Af, temp), axis=0)

print()
print(f'autovalores de Af = {np.linalg.eigvals(Af)}')
print()

del temp

Bf = np.array([[0],[0],[0],[1]])
Cf = np.concatenate((C, np.array([[0]])), axis=1)
Df = np.array([[0]])

# ==========================
#  Funcao de transferencia do sistema
# ==========================

num, den = signal.ss2tf(Af, Bf, Cf, Df, input=0)
print()
print("Funcao de transferencia do sistema realimentado e com integrador:")
print(f'num = {num}')
print(f'den = {den}')
print()


# ==========================
# Resposta coma realimentação e eintegrador
# ==========================

sys = signal.StateSpace(Af, Bf, Cf, Df)
t,y,_ = signal.lsim(sys, U=u, T=timeVect)

plotTheta(timeVect,u,y)