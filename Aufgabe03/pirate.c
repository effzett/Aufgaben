/**
 * Program simulates the pirate problem (long double version)
 * Author: Frank Zimmermann
 * Date:   28.10.2020
 *
 * Genauigkeit: 
 * delta t = 1 * 10^-10
 * eps     = 1 * 10^-10
 * Ergebnis: 10 Digits
 * Dauer ca. 7000 s ; ca 2 Stunden
 */   
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(int argc, char** argv) {
	clock_t prgstart, prgende;
	long double dt  = powl(10,-10);
	long double eps = powl(10,-10);
	long double n = 3. / 4.;
	long double dx   = dt;
	long double xh   = 1;
	long double t = 0, yh = 0, x = 0, y = 0, k = 0, phi = 0, dy = 0;
   	prgstart=clock();
	printf("k=%11.0Lf     xh-x=%15.13Lf   yh-y=%15.13Lf\n", k,(xh -x), (yh -y));
	while (( (long double)(yh - y) > eps || (long double)(xh - x) > eps ) && k < powl(10,12)){
		k = k + 1;
		t = t + dt;
		yh = k * n * dt;
		x = x + dx;
		y = y + dy;
		/* to avoid division by zero */
		if (x != 1) {
			phi = atanl((yh - y) / (1 - x));
		} else {
			phi = M_PI / 2;
		}
		dx = dt * cosl(phi);
		dy = dt * sinl(phi);
		if (fmodl(k, powl(10,8)) == 0) {
			printf("k=%11.0Lf     xh-x=%15.13Lf   yh-y=%15.13Lf\n", k,(xh -x), (yh -y));
		}
	}
	printf("%15.13Lf/%15.13Lf   k0=%Lf    k*dt*n=%15.13Lf     n/(1-n^2)=%15.13Lf\n",
	       dt, eps, k, k * dt * n, n / (1 - n * n));
	printf("xh-x=%15.13Lf   yh-y=%15.13Lf\n", (xh -x), (yh-y));
   	prgende=clock();
   	printf("Die Programmlaufzeit betrug %.3f Sekunden\n",
      (float)(prgende-prgstart) / CLOCKS_PER_SEC);
	return 0;
}