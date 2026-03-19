import matplotlib.pyplot as plt
import numpy as np
from numpy import pi
from scipy import signal


Samples = 1000
timeVect = np.linspace(0,2,Samples)

Ra = 2.5 #Ohm
La = 0.05 #H
J = 0.02 
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

D = 0

sys = signal.StateSpace(A, B, C[1,:], D)
t, y = signal.step(sys, T=timeVect)

plt.plot(t,y)
plt.grid()
plt.show()