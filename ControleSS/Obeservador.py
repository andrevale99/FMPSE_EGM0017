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
file1 = "csv/rpm_data_20260320_123617.csv"
file2 = "csv/rpm_data_20260320_123711.csv"

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
# Parametros de Projeto
# =================================

'''
Requisitos de projeto:

1. +10% mais rápido;
2. 5% de overshoot;
3. Erro nulo em regime permanente;
4. Simulação;
5. Discretização (passo de integração, método numérico 
6. Valor máximo de tensão aplicado no motor: 2,5 V
'''

Mp = 5 / 100
ts2 = 0.6
zeta = np.sqrt((np.log(Mp)**2) / (np.pi**2 + np.log(Mp)**2))
wm = 4 / (ts2*zeta)

print()
print(f'Mp = {Mp}')
print(f'ts (2%) = {ts2}')
print(f'Zeta = {zeta}')
print(f'wm (rad/s) = {wm}')
print()

real = zeta*wm
imag = wm*np.sqrt(1-zeta**2)

polosAlocados = np.array([-real+imag*1j, -real-imag*1j, -real*4, -real*4])

print()
print(f'polos = {polosAlocados}')
print()

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

# x = [ia, wa, theta]
# ia -> Corrente de armadura
# wm -> Velocidade (rad/s) do rotor
# theta -> Posicao (rad) do rotor
A = np.array([
    [-Ra/La, -Kte/La, 0],
    [Kte/J, -B/J, 0],
    [0,1,0]
])

B = np.array([[1/La],[0],[0]])

C = np.array([[0,0,1]])

D = 0

print()
print(f'Autovalores de A:\n{np.linalg.eigvals(A)}')
print()

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

# =========================================
# Sistema com erro como variavel de estado
# =========================================

A00 = A - B@K