CC = gcc
OBJ = shakespeare.o bintree.o
CFLAGS= -Wall
LATEX = latexmk
LATEXFLAGS = -pdf -silent -interaction=nonstopmode
TEXFILE = aufgabe02
LATEXTMP = *.aux *.log *.listing *.fls *.fdb_latexmk *.synctex.gz


all: aufgabe02.pdf shakespeare

shakespeare: $(OBJ)
	$(CC) $(CFLAGS) -o shakespeare shakespeare.o bintree.o

shakespeare.o: shakespeare.c
	$(CC) $(CFLAGS) -c shakespeare.c

bintree.o: bintree.c bintree.h
	$(CC) $(CFLAGS) -c bintree.c

aufgabe02.pdf: aufgabe02.tex shakespeare.c bintree.c bintree.h shakespeareDatenstruktur.png \
	Aufgabe02Flowchart.png TippWahrscheinlichkeiten.png shakespeareBaum2.png Makefile
	$(LATEX) $(LATEXFLAGS) $(TEXFILE).tex  && open $(TEXFILE).pdf

clean:
	rm -f $(OBJ) $(LATEXTMP)