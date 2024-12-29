# pirate problem
# Frank Zimmermann
# 2.11.2020
import time
import math
import matplotlib.pyplot as plt
import numpy as np
import random

random.seed()
mu = math.pi/2  # Hauptbewegungsrichtung nach Norden
kappa = 20     # 0...ziemlich gross: Verteilungsbreite der Winkel
def get_xhyh(n,dt):
    phi_rand = random.vonmisesvariate(mu , kappa)
    return [(n*dt)* x for x in [math.cos(phi_rand), math.sin(phi_rand)]]

# Setzen der Anfangsparameter
dt = 0.0000001
eps = 0.0000001
n = 3/4

# Für graphische Ausgabe
plt.ion()
fig, ax = plt.subplots()
xp, yp = [], []
xph, yph = [], []
sc1 = ax.scatter(xph, yph, marker='^')
sc2 = ax.scatter(xp, yp, c="red")
plt.xlim(0, 1.2)
plt.ylim(0, round(n / (1 - n ** 2) + 0.5))
plt.title('Bouguer-Kurve (enhanced)')
fig.canvas.manager.set_window_title('SVLFG Aufgabe 03 (enhanced)')
#fig.canvas.set_window_title('SVLFG Aufgabe 03 (enhanced)')
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
k = 0.
phi = 0.
dx = dt
dy = 0.
dxh = 0.
dyh = 0.
while (math.fabs(y - yh) > eps or math.fabs(x - xh) > eps) and k < 10000000000:
    k = k + 1
    t = t + dt
    x = x + dx
    y = y + dy
    xh = xh + dxh
    yh = yh + dyh
    if math.fabs(xh - x) != 0:
        phi = math.atan2((yh - y) , (xh - x))   # da es in alle Richtungen gehen kann: atan2
    else:
        phi = math.pi / 2
    dx = dt * math.cos(phi)
    dy = dt * math.sin(phi)
    if math.fmod(k, 100000) == 0:
        dxh, dyh = get_xhyh(n,dt)   # damit die Richtung auch sichtbar wird....
        # print("phi=", '{:20.18f}'.format(phi), "     xh-x  =",
        #       '{:20.18f}'.format(math.fabs(x - xh)),
        #       "    yh-y =", '{:20.18f}'.format(math.fabs(y - yh)))
        # print("Hyp=",  '{:20.18f}'.format(math.sqrt((yh-yy)**2+(1-xx)**2)) ,
        #       "     xh-xx =", '{:20.18f}'.format(math.fabs(xx - xh)),
        #       "   yh-yy =", '{:20.18f}'.format(math.fabs(yy - yh)))
        # Graphische Ausgabe
        xp.append(x)
        yp.append(y)
        xph.append(xh)
        yph.append(yh)
        sc1.set_offsets(np.c_[xph, yph])
        sc2.set_offsets(np.c_[xp, yp])
        fig.canvas.draw_idle()
        plt.pause(0.001)
distance = yh
plt.plot([0, xh], [distance, distance], 'b')
plt.text(xh, distance, '{:9.8f}'.format(distance))

#plt.draw()
fig.canvas.draw_idle()
#######################
print(dt, "/", eps, ":", "k0=", k, distance, n / (1 - n ** 2))
print("xh-x=", math.fabs(x - xh), "   yh-y=", math.fabs(y - yh), "  eps=", eps)
ende = time.time()
print('{:5.3f}s'.format(ende - start))
plt.waitforbuttonpress()
