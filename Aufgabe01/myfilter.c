/**
 * myfilter.c
 * Author: Frank Zimmermann 
 * Date: 8/2020 
 * 
 * Example solution for Aufgabe01
 * (see K&R book)
 * (it uses several constructs just for demonstration...)
 * (...with slightly too much comments)
 *
 * Counts chars, words and lines.
 * Program can be used as
 * a filter or with one file as input argument.
 * The output is controlled by option arguments -c,-w,-l
 * The option -i <inputfile> is optional, <inputfile> is sufficient
 * Every C modul should contain such a (or similar) description header!
 */

/**
 * Includes
 */
#include <stdio.h>  /* standard io from c */
#include <getopt.h> /* for processing the program arguments */
#include <string.h> /* for string copy */
#include <stdlib.h> /* constants like EXIT_SUCCESS,... */
#include <unistd.h> /* access, ... */

/**
 * Macros
 */
#define FALSE 0     /* just for better readability (alt: <stdbool.h> or FALSE (0!=0)) */
#define TRUE !(FALSE)
#define ERROR_FILEREAD(message) fprintf(stderr,"%s is not a valid input file name!\n",message)
#define ERROR_FILEOPEN(message) fprintf(stderr,"An error occured while opening %s!\n",message)
#define ERROR_USAGE(message)    fprintf(stderr,"Usage: %s [-c|-w|-l] [-i] inputfile\n",message)
#define ERROR_NAMELESS(message) fprintf(stderr,"...Nameless and lonly...%s\n",message)

/**
 * Data structure and type definition for a multi value return (for demonstration)
 */
typedef struct {
  long chars; /* counted chars */
  long words; /* counted words */
  long lines; /* counted lines */
} TRIPLET;

/**
 * Forward declarations for C syntax
 */
TRIPLET countCharsWordsLines(FILE *in); /* do the counting work */
void   usage(void);                     /* simple usage message */
FILE*  openInputFile(char* str);        /* opens an input file or stdin  */
int    checkFilename(char* str);        /* check if filename is readable */

/**
 * Global variables
 * (not necessary, we could use argv[0] directly, just to have a global variable)
 */
char progname[255];                     /* the progname for the usage() function */


/**
 * every C program needs a main procedure
 * @param  argc argument count
 * @param  argv array of strings
 * @return      0=Success
 */
int main(int argc, char** argv) {
  int mainReturn = EXIT_SUCCESS;    /* a return for the main function */
  TRIPLET ret3;                     /* a return for the worker function */
  int option;                       /* contains one option letter */
  char filename[255] = "";          /* takes the filname */
  int filenameValid = TRUE;
  FILE* in;                         /* the file descriptor */

  enum {OFF, ON};                   /* to demonstrate an unamed enum OFF=0, ON=1 */

  struct {                          /* to demonstrate a bit field for the options */
    unsigned int c: 1;
    unsigned int w: 1;
    unsigned int l: 1;
    unsigned int i: 1;
  } optflags = {OFF, OFF, OFF, OFF}; /* you HAVE to initialize! */

  in = stdin;                        /* for the case: openInputFile() is never used  */
  strncpy(progname, argv[0], 255);   /* progname/path from argument list (unix/win) to global */

  /**
   * reading the options and arguments and setting flags
   */
  while ((option = getopt(argc, argv, "cwli:")) != EOF) {
    switch (option) {
    case 'c' :                       /* print counted char */
      optflags.c = ON;               /* saving in a bit field */
      break;
    case 'w' :                       /* print counted words */
      optflags.w = ON;               /* saving in a bit field */
      break;
    case 'l' :                       /* print counted lines */
      optflags.l = ON;               /* saving in a bit field */
      break;
    case 'i' :                       /* get input filename from argument list */
      optflags.i = ON;               /* saving in a bit field */
      strncpy(filename, optarg, 255);
      filenameValid = checkFilename(filename);/* just check file */
      break;
    default :
      usage();                       /* in case of wrong call of program */
      exit(EXIT_FAILURE);
    }
  }

  /**
   * looking for a further parameter without option
   */
  if (optind < argc) {                       /* more arguments than options */
    strncpy(filename, argv[optind], 255);
    filenameValid = checkFilename(filename); /* check file. */
  }

  /**
   * test for no options at all or wrong filename
   */
  if (filenameValid == FALSE || (optflags.c == OFF && optflags.w == OFF && optflags.l == OFF)) {
    usage();
    exit(EXIT_FAILURE);                      /* no arguments or wrong filename */
  }

  /**
   * when we are here: filenameValid is TRUE AND there are valid flags
   * openInputFile gives a new pointer to FILE (when filename has content)
   * or returns stdin (when filename is empty)
   * openInputFile returns NULL in case of error
   */
  mainReturn = ((in = openInputFile(filename)) == NULL) ? EXIT_FAILURE : EXIT_SUCCESS;

  if (mainReturn == EXIT_SUCCESS) {          /* if no errors in opening */
    ret3 =  countCharsWordsLines(in);        /* this does all the work */
    fclose(in);                              /* close file, for stdin not harmful */

    /**
     * see what we have to write to stdout:
     */
    if (optflags.c == ON) {
      printf("Zeichen = %ld\n", ret3.chars);
    }
    if (optflags.w == ON) {
      printf("Wörter = %ld\n", ret3.words);
    }
    if (optflags.l == ON) {
      printf("Zeilen = %ld\n", ret3.lines);
    }
    fflush(stdout); /* better be sure */
  }
  else {
    ERROR_FILEOPEN(filename);
  }
  return mainReturn;
}

/**
 * counts chars,words and lines
 * @param  in pointer to FILE
 * @return    TRIPLET
 */
TRIPLET countCharsWordsLines(FILE *in) {
  TRIPLET retval = {0, 0, 0};
  long nl,                 /* counter for lines */
       nw,                 /* counter for words */
       nc;                 /* counter for chars */
  int  state,              /* flag for position */
       c;                  /* a char */
  /* needful definitions (for demonstration) */
  enum {OUT, IN};

  state = OUT;             /* initialize position flag */
  nc = nw = nl = 0;        /* initialize values */
  while ((c = getc(in)) && !feof(in)) {
    ++nc;                       /* increment character count */
    if (c == '\n') {
      ++nl;                /* increment line count */
    }
    if (c == ' ' || c == '\n' || c == '\t') {
      state = OUT;         /* leaving a word */
    }
    else if (state == OUT) {
      state = IN;          /* entering a word */
      ++nw;                /* ...and count */
    }
  }
  fclose(in);

  retval.chars = nc;       /* fill struct fields ... */
  retval.words = nw;
  retval.lines = nl;
  return retval;           /* return the struct */
}

/**
 * just print out a usage() message on stderr
 */
void usage() {
  if (*progname != '\0') {
    ERROR_USAGE(progname);
  }
  else {
    ERROR_NAMELESS("Greeting to grandma...!");    /* what is this ??! */
  }
}

/**
 * Tries to open a file with a string
 * @param  str filename
 * @return     pointer to FILE or NULL
 */
FILE* openInputFile(char* str) {
  FILE* in = stdin;
  if (*str != '\0') {               /* str is empty */
    if ((in = fopen(str, "r")) == NULL) {
      ERROR_FILEREAD(str);          /* could not open for read */
    }
  }
  return in;
}

/**
 * just checks the readability and print on stderr in case of error
 * @param  str string holds filename
 * @return     boolean value for success
 */
int checkFilename(char* str) {
  int retVal;
  if ( access( str, R_OK ) == EXIT_SUCCESS) { /* security issue...! */
      retVal = TRUE;
  }
  else {
    retVal = FALSE;
    ERROR_FILEREAD(str);
  }
  return retVal;
}