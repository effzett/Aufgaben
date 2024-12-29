CC = gcc
OBJ = pirate.o
CFLAGS= -Wall
LATEX = latexmk
LATEXFLAGS = -pdf -silent -interaction=nonstopmode
TEXFILE = aufgabe03
LATEXTMP = *.aux *.log *.listing *.fls *.fdb_latexmk *.synctex.gz


all: aufgabe03.pdf pirate 

pirate: $(OBJ)
	$(CC) $(CFLAGS) -o pirate pirate.o

pirate.o: pirate.c
	$(CC) $(CFLAGS) -c pirate.c

aufgabe03.pdf: aufgabe03.tex pirate.c Aufgabe03.py Aufgabe03Simple.py Aufgabe03Enhanced.py Makefile
	$(LATEX) $(LATEXFLAGS) $(TEXFILE).tex  && open $(TEXFILE).pdf

clean:
	rm -f $(OBJ) $(LATEXTMP)