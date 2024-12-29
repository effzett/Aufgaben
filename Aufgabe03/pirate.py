# pirate problem with extended precision
# Frank Zimmermann
# 2.11.2020
import time
from mpmath import *

start = time.time()
mp.dps = 20
print(mp)
dt  = mp.mpf('0.00000010000000')
eps = mp.mpf('0.00000010000000')
print(nstr(eps, 20))
n = mp.mpf('0.75')
t = mp.mpf('0')
xh = mp.mpf('1')
yh = mp.mpf('0')
x = mp.mpf('0')
y = mp.mpf('0')
k = mp.mpf('0')
alpha = mp.mpf('0')
dx = mp.mpf(dt)
dy = mp.mpf('0')
while (mp.fabs(mp.fsub(y, yh)) > eps) or (mp.fabs(mp.fsub(x, xh)) > eps) and k < 1000000000:
    k = mp.fadd(k, 1)
    t = mp.fadd(t, dt)
    yh = mp.fmul(k, mp.fmul(n, dt))
    x = mp.fadd(x, dx)
    y = mp.fadd(y, dy)
    if x != 1:
        alpha = mp.atan(mp.fsub(yh, y) / mp.fsub(1, x))
    else:
        alpha = pi/2
    dx = mp.fmul(dt, mp.cos(alpha))
    dy = mp.fmul(dt, mp.sin(alpha))
    if mp.fmod(k, 100000) == 0:
        print("xh-x=", mp.fabs(mp.fsub(x, xh)), "   yh-y=", mp.fabs(mp.fsub(y, yh)))
    if mp.fmod(k, 1000000) == 0:
        print("k=", k, "k*dt*n=", nstr(k * dt * n, 20), "x,y=", nstr(x, 20), nstr(y, 20))
print(nstr(dt, 20), "/", nstr(eps, 20), ":", "k0=", k, nstr(k * dt * n, 20), nstr(n / (1 - n ** 2), 20))
print("xh-x=", nstr(mp.fabs(mp.fsub(x, xh)), 20), "   yh-y=",
      nstr(mp.fabs(mp.fsub(y, yh)), 20), "  eps=", nstr(eps, 20))
stop = time.time()
print('{:5.3f}s'.format(stop - start))
