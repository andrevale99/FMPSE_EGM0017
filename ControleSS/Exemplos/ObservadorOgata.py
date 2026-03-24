import matplotlib.pyplot as plt
from scipy import signal
import numpy as np
import pandas as pd
import control as ct

from numpy import pi

# Exemplo 10.7 do livro de Ogata

timeMax = 4
Samples = 1000*timeMax

timeVect = np.linspace(0, timeMax, Samples)

# Valor de entrada escolhida arbitrariamente
u = np.zeros_like(timeVect)
u[timeVect >= timeMax/4] = 1 #16.7 (mais ou menos o ganho real)

A = np.array([
    [0, 1],
    [20.6, 0]
])

B = np.array([[0],[1]])
C = np.array([[1,0]])
D = 0 

pAloc = [-1.8+2.4j, -1.8-2.4j]
pObs = [-8, -8]

K = ct.acker(A,B, pAloc).reshape((1,2))
print(f'\nK = {K}')

Ke = ct.acker(A.T,C.T, pObs).reshape((2,1))
print(f'\nKe = {Ke}\n')

A00 = A - B@K
A01 = B@K
A10 = np.zeros((2,2))
A11 = A - Ke@C

print(A00,'\n')
print(A01,'\n')
print(A10,'\n')
print(A11,'\n')

Ahat = np.block([
    [A00,A01],
    [A10,A11]
])

B = np.array([[0],[1],[0],[0]])
C = np.array([[0,0,1,0]])

t,y,_ = signal.lsim((Ahat,B,C,D), U=u, T=timeVect, X0 = np.array([1,0,0.5,0]))

plt.plot(t,y)
plt.grid()
plt.show()