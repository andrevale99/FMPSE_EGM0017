import matplotlib.pyplot as plt
import numpy as np
from numpy import pi
from scipy import signal

timeMax = 3
Samples = 1000 * timeMax
timeVect = np.linspace(0,timeMax,Samples)

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
C = C[1,:]

D = 0

u = np.zeros_like(timeVect)
u[timeVect > 0.5] = 5
u[timeVect > 1.0] = 0

sys = signal.StateSpace(A, B, C, D)
t, y = signal.step(sys, T=timeVect)
# t,y,_ = signal.lsim(sys, U=u, T=timeVect)

plt.plot(t,y)
plt.grid()
plt.show()