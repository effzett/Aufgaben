# Musterlösung zu den 2 optionalen Aufgaben für FI/Duales Studium
# Es geht um Simulation von Nash-Gleichgewichten
# Author: Frank Zimmermann
# Datum: 2.12.2022
import time
import numpy as np
import matplotlib.pyplot as plt

# Szenario I
# Geschwindigkeitstest
print("Berechnung des Preises und des Maximalgewinnes im Szenario I")
# 1.Version mit Schleife (C)
max1 = 0
p01 = 0
step = 0.001
st = time.time()
for p1 in np.arange(1.5, 5.0, step):
    if (5.0 - p1) * (p1 - 1.5) > max1:
        (p01, max1) = (p1, (5.0 - p1) * (p1 - 1.5))
print("Preis={0:5.4f}, Gesamtgewinn={1:5.4f}".format(p01, max1))
print("Schleife nach C-Art: {0:5.2f} msec".format((time.time() - st) * 1000.))

# 2. Version ohne Schleife (pythonic)
st = time.time()
xarr = np.arange(1.5, 5.0, step)
nxarr = (5.0 - xarr) * (xarr - 1.5)  # Gewinnfunktion
GewinnMax = np.amax(nxarr)  # Max finden
PreisMax = (np.where(nxarr == GewinnMax)[0] * step + 1.5)[0]  # Preis aus Index
print("Preis={0:5.4f}, Gesamtgewinn={1:5.4f}".format(PreisMax, GewinnMax))
print("Pythonic ohne Schleife: {0:5.2f} msec".format((time.time() - st) * 1000.))

print()

eps = step / 100.0
max2 = 0
p0x2 = 0
p0y2 = 0
for p2y in np.arange(1.5, 5.0, step):
    for p2x in np.arange(1.5, 5.0, step):
        if (5.0 - 2 * p2x + p2y) * (p2x - 1.5) > max2:
            (p0x2, p0y2, max2) = (p2x, p2y, (5.0 - 2 * p2x + p2y) * (p2x - 1.5))
    if abs(p0x2 - p0y2) < eps:
        break
    else:
        max2 = 0

print("Errechnete Werte für Szenario I / Szenario II\n")
print("------------------------------------------------\n")

print(
    "Gesamtgewinn:{0:5.4f} / {1:5.4f}\nPreis:{2:5.4f} / {3:5.4f}\n" \
    "Einzelgewinn:{4:5.4f} / {5:5.4f}\nKäufe:{6:5.4f} / {7:5.4f}\n".
    format(max1, max2,
           p01, p0x2,
           (p01 - 1.5), (p0x2 - 1.5),
           (5.0 - p01), (5.0 - p0x2)))

print("Analytisch berechnet:\n"
    "Gesamtgewinn:{0:5.4f} / {1:5.4f}\nPreis:{2:5.4f} / {3:5.4f}\n" \
    "Einzelgewinn:{4:5.4f} / {5:5.4f}\nKäufe:{6:5.4f} / {7:5.4f}\n".
      format((13. / 4. - 1.5) * (5. - 13. / 4.), (8.0 / 3.0 - 1.5) * (5.0 - 8.0 / 3.0),
             13. / 4., 8. / 3.,
             (13. / 4. - 1.5), (8. / 3. - 1.5),
             (5. - 13. / 4.), (5. - 8. / 3.)))

levels = 20
plt.subplot(121)
x1, y1 = np.mgrid[0:8:100j, 0:8:100j]
z1 = (5 * x1 - 15 / 2 - 2 * x1 * x1 + 3 * x1 + x1 * x1 - x1 * 3 / 2)
contourset1 = plt.contourf(x1, y1, z1, levels, cmap='viridis')
contourlines1 = plt.contour(x1, y1, z1, levels, colors=('k',))
plt.clabel(contourlines1, inline=1)
plt.hlines(p01, 0, 8, color="red")
plt.vlines(p01, 0, 8, color="red")
plt.plot([13 / 4, 13 / 4], [0, 8], color="green", ls="dotted")
plt.plot([0, 8], [0, 8], color="lightgrey")
plt.title("Szenario I: Gewinnfunktion für Firma 1")
plt.xlabel("Preis $p_1$")
plt.ylabel("Preis $p_2$")

plt.subplot(122)
x2, y2 = np.mgrid[0:8:100j, 0:8:100j]
z2 = (5 * x2 - 15 / 2 - 2 * x2 * x2 + 3 * x2 + x2 * y2 - y2 * 3 / 2)
contourset2 = plt.contourf(x2, y2, z2, levels, cmap='viridis')
contourlines2 = plt.contour(x2, y2, z2, levels, colors=('k',))
plt.clabel(contourlines2, inline=1)
plt.vlines(p0y2, 0, 8, color="red")
plt.hlines(p0x2, 0, 8, color="red")
plt.plot([2, 4], [0, 8], color="green", ls="dotted")
plt.plot([0, 8], [0, 8], color="lightgrey")
plt.title("Szenario II: Gewinnfunktion für Firma 1")
plt.xlabel("Preis $p_1$")
plt.ylabel("Preis $p_2$")
textstr = "Die gestrichelte grüne Linie zeigt den Maximalwert für $p_1$ in Abhängigkeit von "\
          "$p_2$: $p_{1,max}(p_2)$. " \
          "Aus Sicht von Firma 2 muss die Grafik genauso aussehen (Symmetrie).\n" \
          "Daher muss das Ergebnis an der grauen Linie (Symmetrieachse) gespiegelt "\
          "werden können. " \
          "Der Schnittpunkt graue und grüne Linie ist dann genau das Nash--GG."
plt.gcf().text(0.02, +0.01, textstr, fontsize=7)
plt.show()
