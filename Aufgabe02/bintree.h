#ifndef BINTREE
#define BINTREE

/* This is the number for length of 'search string' + 'next char' + '\0' */
#define MAXPAT 52

/* A structure used for PS output */
struct point {
  int x;
  int y;
};
typedef struct point POINT;

/* List that contains for a given sPattern the nextChar and its frequency */
struct lnode {
  struct lnode *pnext;        /* points to next lnode entry */
  char nextChar;              /* contains next char for pattern*/
  int counter;                /* counts occurences of char for defined pattern*/
};
typedef struct lnode LNODE;

/* Node in a binary tree that contains a pattern and a pointer to a List (see above) */
struct tnode {
  struct tnode *pleft;
  struct tnode *pright;
  char sPattern[MAXPAT - 1];
  LNODE* pl;
  int maxList;                /* counts the entries in the list */
  POINT pos;
};
typedef struct tnode TNODE;

/**
 * function prototypes
 */
TNODE* insert(TNODE*, char*);
LNODE* insert_lnode(LNODE*, char);
char   random_text(TNODE*);
char   get_next_char(TNODE*, int);
TNODE* lookup(TNODE*);
void   tree_print(TNODE*);
void   print_ps(TNODE*);

void   list_print(TNODE*);
int    get_index(TNODE*);
POINT  ps_coord(int, int);
void   print_header_ps(FILE*);
void   print_footer_ps(FILE*);

void   fill_node_ps(FILE*, TNODE*, TNODE*);
void   print_node_ps(FILE*, TNODE*, TNODE*);
void   fill_tree_ps(FILE*, TNODE*, TNODE*);
void   print_tree_ps(FILE*, TNODE*, TNODE*);

#endif