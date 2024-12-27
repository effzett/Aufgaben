CC = gcc
OBJ = myfilter.o
CFLAGS = -Wall
TEXFILE = aufgabe01
LATEX = latexmk
LATEXFLAGS = -pdf -silent -interaction=nonstopmode
LATEXTMP = *.log *.listing *.fls *.fdb_latexmk *.synctex.gz *.aux

all: aufgabe01.pdf myfilter

aufgabe01.pdf: aufgabe01.tex myfilter.c Aufgabe01PAP.png Makefile
	$(LATEX) $(LATEXFLAGS) $(TEXFILE).tex  && open $(TEXFILE).pdf

myfilter: $(OBJ)
	$(CC) $(CFLAGS) -o myfilter myfilter.o

myfilter.o: myfilter.c
	$(CC) $(CFLAGS) -c myfilter.c

clean: 
	rm -f $(OBJ) $(LATEXTMP)