# pirate problem
# Frank Zimmermann
# 2.11.2020
import time
import math
import matplotlib.pyplot as plt
import numpy as np

# Setzen der Anfangsparameter
dt = 0.0000001
eps = 0.0000001
n = 3 / 4

# Für graphische Ausgabe
plt.ion()
fig, ax = plt.subplots()
xp, yp = [], []
xph, yph = [], []
sc1 = ax.scatter(xph, yph, marker='^')
sc2 = ax.scatter(xp, yp, c="red")
plt.xlim(0, 1.2)
plt.ylim(0, round(n / (1 - n ** 2) + 0.5))
plt.title('Bouguer-Kurve')
fig.canvas.manager.set_window_title('SVLFG Aufgabe 03')
plt.text(0.15, 0.9 * round(n / (1 - n ** 2) + 0.5), r'$n=$' + '{:3.2f}   '.format(n) +
         r'$\Delta t=$' + '{:9.8f}   '.format(dt) + r'$\epsilon=$' + '{:9.8f}'.format(eps))
plt.ylabel('Entfernung')
plt.xlabel('Entfernung')
# plt.grid(True)
plt.draw()
#################################

start = time.time()
t = 0.
xh = 1.
yh = 0.
x = 0.
y = 0.
xx = 0.
yy = 0.
k = 0.
phi = 0.
dx = dt
dxx = dt
dy = 0.
dyy = 0.
while (math.fabs(yy - yh) > eps or math.fabs(xx - xh) > eps) and k < 10000000000:
    k = k + 1
    t = t + dt
    yh = k * n * dt
    x = x + dx
    y = y + dy
    xx = xx + dxx
    yy = yy + dyy
    # Variante 1
    # Trigonometrie
    if x != 1:
        phi = math.atan((yh - y) / (1 - x))
    else:
        phi = math.pi / 2
    dx = dt * math.cos(phi)
    dy = dt * math.sin(phi)
    # Variante 2
    # Pythagoras
    dxx = dt * (1 - xx) / math.sqrt((yh - yy) ** 2 + (1 - xx) ** 2)
    dyy = dt * (yh - yy) / math.sqrt((yh - yy) ** 2 + (1 - xx) ** 2)
    if math.fmod(k, 100000) == 0:
        # print("phi=", '{:20.18f}'.format(phi), "     xh-x  =",
        #       '{:20.18f}'.format(math.fabs(x - xh)),
        #       "    yh-y =", '{:20.18f}'.format(math.fabs(y - yh)))
        # print("Hyp=",  '{:20.18f}'.format(math.sqrt((yh-yy)**2+(1-xx)**2)) ,
        #       "     xh-xx =", '{:20.18f}'.format(math.fabs(xx - xh)),
        #       "   yh-yy =", '{:20.18f}'.format(math.fabs(yy - yh)))
        # Graphische Ausgabe
        xp.append(xx)
        yp.append(yy)
        xph.append(xh)
        yph.append(yh)
        sc1.set_offsets(np.c_[xph, yph])
        sc2.set_offsets(np.c_[xp, yp])
        fig.canvas.draw_idle()
        plt.pause(0.001)
distance = k * dt * n
plt.plot([0, 1.0], [distance, distance], 'b')
plt.text(1.01, distance, '{:9.8f}'.format(distance))

#plt.draw()
fig.canvas.draw_idle()
#######################
print(dt, "/", eps, ":", "k0=", k, distance, n / (1 - n ** 2))
print("xh-x=", math.fabs(x - xh), "   yh-y=", math.fabs(y - yh), "  eps=", eps)
ende = time.time()
print('{:5.3f}s'.format(ende - start))
plt.waitforbuttonpress()
