/**
 * shakespeare.c
 * Author: Frank Zimmermann
 * 9/2020
 *
 * Generates a random text from an input text
 * uses tree and linked list as data structures
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> /*C-99*/
#include <time.h>
#include <limits.h>

#ifndef BINTREE
#include "bintree.h"
#endif

#define ERROR_FILEOPEN(message) fprintf(stderr, "Konnte Datei %s nicht öffnen!\n", (message));
#define ERROR_PARAMETER() fprintf(stderr, "Wrong command line ...\n");
#define ERROR_SSLENGTH(message1, message2) fprintf(stderr,\
        "Die Suchstringlänge von %d überschreitet die maximale Länge und wurde auf %d gesetzt\n",\
        (message1), (message2));
#define ERROR_USAGE(message) fprintf(stderr, \
        "Usage: %s [-h|-s<PatternLength:1-50>|-l<OutputLength>|-f<FileName>|-d<Debug:0-7>]\n",\
        (message));
#define DEBUG_PATTERNCOUNT(message) printf("There were %d different patterns\n", (message));

/* some global variables ************************************/
char ss[MAXPAT];                    /* buffer for text ss=searchstring+c+'\0' */
int ssLength = 2;                   /* default ss length =searchstring+c */
TNODE *head;                        /* pointer to root of binary tree */
char search[MAXPAT] = "";           /* the current ss that determines the next char */
int patternCount = 0;               /* DEBUG: count the different patterns (no of TNODEs) */

int main(int argc, char *argv[]) {
  int debug = 0;                    /* default: no debugging */
  char opt;                         /* holds the current option char */
  char c;                           /* char read in */
  int countFirstChar;               /* needed for fill the first pattern */
  int mainReturn = EXIT_SUCCESS;    /* indicates errors in command line */
  bool isEnough;                    /* flag for completeness of first pattern */
  int i;
  unsigned long maxOut = 0;         /* max number of output characters */
  unsigned long totalCharCount = 0; /* counter for input chars */

  srand((unsigned)time(NULL));      /* initialize random function */

  head = NULL;                      /* initialiaze root tree pointer */

  /* Parsing the commandline */
  while ((opt = getopt(argc, argv, "s:f:l:d:h")) != -1) {
    switch (opt) {
    case 'h': /* Usage */
      ERROR_USAGE(argv[0]);
      exit(EXIT_FAILURE);
      break;
    case 's':                       /* length of searchstring */
      ssLength = (int)atoi(optarg) + 1; /* ss = searchstring + c */
      break;
    case 'l':                       /* number of max char to generate */
      maxOut = (int)atoi(optarg);   /* besser: überprüfen! */
      break;
    case 'd':                       /* 0=no Debug, 1=print ss, 2=print tree nodes + list values */
      debug = (int)atoi(optarg);    /* 4=print some levels of graphic tree in Postscript */
      if (debug > 7 || debug < 0) {
        debug = 0;
      }
      break;
    case 'f':
      if (!freopen(optarg, "r", stdin)) {
        /* to use as a filter as well as with filename */
        ERROR_FILEOPEN(optarg);
        ERROR_USAGE(argv[0]);
        exit(EXIT_FAILURE);
      }
      break;
    }
  }

  /**  Here we _would_ test for further arguments... */
  /** ...but we are lazy */

  if (mainReturn == EXIT_FAILURE) {
    /* program call was faulty */
    /* for future checks */
    ERROR_PARAMETER();
    exit(EXIT_FAILURE); /* exit program with error code */
  }

  if (ssLength > MAXPAT - 1) {
    /* to avoid buffer overflow */
    ERROR_SSLENGTH(ssLength - 1, MAXPAT - 2)
    ssLength = MAXPAT - 1;
  }

  /* Reading in the text char by char */
  /* ********************************************************************** */

  countFirstChar = 0;
  isEnough = false;
  ss[ssLength] = '\0'; /* string termination */
  while ((!feof(stdin)) && ((c = fgetc(stdin)) != EOF)) {
    if (isEnough == true) {/* the current char completes ss (searchstr + c) */
      ss[ssLength - 1] = c;
      head = insert(head, ss); /* save the ss in the tree */
      if (debug & 1) {
        printf("%s\n", ss);
      }; /* print ss */
      for (i = 0; i < ssLength; i++) { /* shift to left ... */
        ss[i] = ss[i + 1];
      }
    } /* ...and get the next char...*/
    else { /* filling up the ss...until searchString is full */
      ss[countFirstChar++] = c;
      isEnough = (countFirstChar == ssLength - 1) ? true : false;
    }
    if (totalCharCount++ == (ULONG_MAX - 1)) {
      /* watch if chars are longer than ULONG_MAX */
      break;
    }
  }
  fclose(stdin);

  /* DEBUG */
  if (debug & 2) {
    DEBUG_PATTERNCOUNT(patternCount);
    tree_print(head); /* print the tree with linked list */
  }

  /* print out the generated text char by char */
  /* when not otherwise set start at beginning */
  if (strcmp(search, "") == 0) {
    strcpy(search, head->sPattern); 
  }

  /* DEBUG */
  if (debug & 4) {
    print_ps(head);
  }

  /* if maxOut not set use the input text length */
  maxOut = (maxOut == 0) ? totalCharCount : maxOut;

  printf("<%s>", search); /* ...beginning with starting pattern */
  /* maxOut = (maxOut >= strlen(search)) ? (maxOut -= strlen(search)) : 0;  unsequenced ! */
  if(maxOut >= strlen(search)){
    maxOut -= strlen(search);
  }
  else{
    maxOut = 0;
  }
  while (maxOut-- > 0) {
    printf("%c", random_text(head)); /* print out next generated char */
  }

  printf("\n");
  return mainReturn;
}
