/**
 * Utility code for shakespeare.c: for tree, linked list and debugging
 * Author: Frank Zimmermann
 * Date: 15.10.2020
 * modified: 29.10.24
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BINTREE
#include "bintree.h"
#endif

#define ERROR_MEMORYALLOCATION fprintf(stderr, "Konnte keinen Speicher allokieren!\n")

extern char ss;          /* buffer for text ss */
extern int ssLength;     /* default pattern length */
extern TNODE *head;      /* pointer to root of binary tree */
extern char search[];    /* the current ss that determines the next char */
extern int patternCount; /* count the different patterns */

int psHeight = 0;
int psWidth = 0;
int psXconst = 15;
int psYconst = 15;
int maxY = 0;

POINT position = {0, 0}; /* position in tree */

/* Inserts the ss into the data structure if not present, otherwise increment counter */
TNODE *insert(TNODE *pn, char *ss) {
  int cond;
  LNODE *pl = NULL;
  char sPattern[MAXPAT + 1]; /* sPattern will hold search pattern */
  char c = ss[ssLength - 1]; /* get char before string end */
  strcpy(sPattern, ss);
  sPattern[ssLength - 1] = '\0'; /* cut sPattern from string */

  if (pn == NULL) {
    /* new entry */
    pn = malloc(sizeof(TNODE));           /* allocating space for new tnode */
    if(pn==NULL){
      ERROR_MEMORYALLOCATION;
      exit(EXIT_FAILURE);
    }
    strcpy(pn->sPattern, sPattern);       /* copy search pattern into tnode */
    pn->pl = insert_lnode(pl, c);         /* put lnode into tnode */
    pn->pleft = pn->pright = NULL;        /* leave tnode */
    pn->maxList = 1;                      /* first entry in list */
    patternCount++;                       /* number of different pattern */
  } else if ((cond = strcmp(pn->sPattern, sPattern)) == 0) {
    /* ss is already in tree */
    insert_lnode(pn->pl, c); /* insert into list or increment counter */
    pn->maxList++;           /* increment entry counter */
  } else if (cond > 0) {
    /* add to the left */
    pn->pleft = insert(pn->pleft, ss); /* recursive */
  } else {
    /* add to the right */
    pn->pright = insert(pn->pright, ss); /* recursive */
  }
  return pn;
}

/* appends a lnode if non existing, otherwise increment counter */
LNODE *insert_lnode(LNODE *pl, char c) {
  if (pl == NULL) {
    /* new entry */
    pl = malloc(sizeof(LNODE)); /* allocating space for new lnode */
    if(pl==NULL){
      ERROR_MEMORYALLOCATION;
      exit(EXIT_FAILURE);
    }
    pl->pnext = NULL;                    /* end of list */
    pl->nextChar = c;                    /* copy char into nextChar */
    pl->counter = 1;                     /* init counter */
  } else if (pl->nextChar == c) {
    /* c is already in structure */
    pl->counter++; /* increment counter */
  } else {
    pl->pnext = insert_lnode(pl->pnext, c); /* try next, recursive */
  }
  return pl;
}

char random_text(TNODE *pn) {
  /* generates the next char from s */
  TNODE *pnode;
  int index;
  char c = 'z';
  int i;

  pnode = lookup(pn);              /* searching the tnode that contains s */
  index = get_index(pnode);        /* random index between 1...maxList */
  c = get_next_char(pnode, index); /* the generated char */
  for (i = 0; i < ssLength - 2; i++) {
    /* correcting the new search pattern */
    search[i] = search[i + 1];
  }
  search[ssLength - 2] = c;
  search[ssLength - 1] = '\0';
  return c;
}

char get_next_char(TNODE *pn, int index) {
  LNODE *pl;

  pl = pn->pl;
  while ((index -= pl->counter) > 0) {
    if (pl->pnext != NULL) {
      pl = pl->pnext;
    } else {
      break;
    }
  }
  return pl->nextChar;
}

TNODE *lookup(TNODE *pn) {
  /* lookup an node entry in head */
  int cond;
  TNODE *retval;

  if (pn == NULL) {
    retval = NULL;
  }
  if ((cond = strcmp(pn->sPattern, search)) == 0) {
    retval = pn;
  } else if (cond > 0) {
    retval = lookup(pn->pleft);
  } else {
    retval = lookup(pn->pright);
  }
  return retval;
}

void tree_print(TNODE *pn) {
  /* traverse tree from left to right */
  if (pn != NULL) {
    tree_print(pn->pleft);
    printf("%s [%d]-> ", pn->sPattern, pn->maxList);
    list_print(pn);
    tree_print(pn->pright);
  }
}

void print_ps(TNODE *p) {
  FILE *fout;
  if ((fout = fopen("shakespeare.ps", "w")) != NULL) {
    print_header_ps(fout);
    fill_tree_ps(fout, p, NULL);
    print_tree_ps(fout, p, NULL);
    print_footer_ps(fout);
    fclose(fout);
  }
}

void list_print(TNODE *pn) {
  LNODE *pl;

  for (pl = pn->pl; pl != NULL; pl = pl->pnext) {
    /* move through the linked list...*/
    printf("(%c,%02d)", pl->nextChar, pl->counter);
  }
  printf("\n");
}

int get_index(TNODE *pn) {
  int rand_index;
  /* Intervall bestimmen */
  int a=1;
  int e=pn->maxList;
  double range = e-a+1.0;
  rand_index = a + (int)(range * rand() / (RAND_MAX + 1.0));
  /* these corrections should never happen */
  if (rand_index < 1) {
    rand_index = 1;
  }
  if (rand_index > pn->maxList) {
    rand_index = pn->maxList;
  }
  return rand_index;
}

POINT ps_coord(int x, int y) {
  POINT pos;
  /* pos.x = x*(842-2*psXconst) / patternCount +psXconst; */
  pos.x = x * (750 - 2 * psXconst) / patternCount + psXconst;
  /*  pos.y = (590-psYconst) - (y*(590-psYconst) / maxY) +psYconst; */
  pos.y = (590 - psYconst) - (y * (590 - psYconst) / maxY) + psYconst;
  return pos;
}

void print_header_ps(FILE *fp) {
  fprintf(fp, "%%!PS\r\n");
  fprintf(fp, "/inch { 72 mul } def\r\n");
  fprintf(fp, "590 0  translate\r\n");
  fprintf(fp, "90 rotate\r\n");
}
void print_footer_ps(FILE *fp) {
  fprintf(fp, "\r\n");
}

void fill_node_ps(FILE *fout, TNODE *pn, TNODE *ppn) {
  pn->pos.x = position.x++;
  pn->pos.y = position.y;
}

void print_node_ps(FILE *fout, TNODE *pn, TNODE *ppn) {
  char str[MAXPAT + 1];
  char *ptr;
  if (ppn != NULL) {
    fprintf(fout, "%d %d moveto\r\n", ps_coord(ppn->pos.x, ppn->pos.y).x, ps_coord(ppn->pos.x, ppn->pos.y).y);
    fprintf(fout, "%d %d lineto\r\n", ps_coord(pn->pos.x, pn->pos.y).x, ps_coord(pn->pos.x, pn->pos.y).y);
    fprintf(fout, "stroke\r\n");
  }
  fprintf(fout, "%d %d moveto\r\n", ps_coord(pn->pos.x - psWidth, pn->pos.y - psHeight).x,
          ps_coord(pn->pos.x - psWidth, pn->pos.y - psHeight).y);
  fprintf(fout, "gsave\r\n");
  fprintf(fout, "/Palatino-Roman\r\n");
  fprintf(fout, "0.1 inch\r\n");
  fprintf(fout, "selectfont\r\n");
  strcpy(str, pn->sPattern);
  while ((ptr = strchr(str, '(')) != NULL) {
    /* ( and ) have to be escaped for PS */
    *ptr = '<';
  }
  while ((ptr = strchr(str, ')')) != NULL) {
    *ptr = '>';
  }

  fprintf(fout, "([%s]) show\r\n", str);
  fprintf(fout, "grestore\r\n");
}

void fill_tree_ps(FILE *fout, TNODE *pn, TNODE *ppn) {
  position.y++;
  if (pn != NULL) {
    fill_tree_ps(fout, pn->pleft, pn);
    fill_node_ps(fout, pn, ppn);
    fill_tree_ps(fout, pn->pright, pn);
  }
  if (maxY < position.y) {
    maxY = position.y;
  }
  position.y--;
}

void print_tree_ps(FILE *fout, TNODE *pn, TNODE *ppn) {
  position.y++;
  if (pn != NULL) {
    print_tree_ps(fout, pn->pleft, pn);
    print_node_ps(fout, pn, ppn);
    print_tree_ps(fout, pn->pright, pn);
  }
  position.y--;
}
