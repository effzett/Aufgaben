#include <stdio.h>
#include <time.h>     /* time() */
#include <stdlib.h>   /* rand() */

/**
 * Programm berechnet Events zwischen 1 und 9 und zählt Häufigkeiten
 * Zuerst ohne Setzen von Seed
 * Dann mit Setzen von Seed
 * 
 *  -------+ <----Seed
 *         |
 *  x = (a*x+c) mod 2^31
 *
 *  Man beachte, dass ohne Seed immer dieselben Verteilungen herauskommen!
 */
#define EVENTS 1000000
int main(int argc, char** argv) {
	int a = 1;
	int e = 9;
	int r;
	int counterOhne[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int counterMit[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int range = e - a + 1;
	double rr;
	for(int i=0;i<10;i++) {counterOhne[i]=counterMit[i]=0;};
		for (int i = 0; i < EVENTS; i++) {
		rr = rand() / (RAND_MAX + 1.0);				
		r = a +(int) range * rr;
		counterOhne[r]++;
	}
	for (int i = 1; i < 10; i++) {
		printf("Ohne Seed: %2d %18d\n", i,counterOhne[i]);
	}
	for(int i=0;i<10;i++) {counterOhne[i]=counterMit[i]=0;};
	srand((unsigned)time(NULL));
	for (int i = 0; i < EVENTS; i++) {
		rr = rand() / (RAND_MAX + 1.0);
		r = a +(int) range * rr;
		counterOhne[r]++;
	}
	for (int i = 1; i < 10; i++) {
		printf("Mit Seed : %2d %18d\n", i,counterOhne[i]);
	}

	return 0;
}
