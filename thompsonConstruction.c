#include <stdio.h>
#include <stdlib.h>

struct node {
  int isFinal;
  char symbol;  //symbol used to
  /*  transition is a struct of symbol:node_t  */
  struct node* symbol_transition;

  /*  epsilonTransitions is an array of node_t  */
  struct node* epsilon_transition1;
  struct node* epsilon_transition2;

};

struct List {
  struct node** sf_pair;
};

struct node* createNode(int final) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->isFinal = final;
  newNode->symbol_transition = NULL;
  newNode->epsilon_transition1 = NULL;
  newNode->epsilon_transition2 = NULL;

  return newNode;

}

void addSymbolTransition(struct node* from, struct node* to, char symbol) {
  if( from->symbol_transition == NULL ) {
    from->symbol_transition = to;
    to->symbol = symbol;
  }
}

void addEpsilonTransition(struct node* from, struct node* to) {
  if( from->epsilon_transition1 == NULL ) {
    from->epsilon_transition1 = to;
  }
  else if ( from->epsilon_transition2 == NULL ) {
    from->epsilon_transition2 = to;
  }
  else {
    perror("Cannot add more than two Epsilon Transitions!");
  }
}

struct List* createFromSymbol(char symbol) {
  struct List* start_end_pair = malloc(sizeof(struct List));
  start_end_pair->sf_pair = malloc(2 * sizeof(struct node*));

  struct node* start = createNode(0);
  struct node* end = createNode(1);

  addSymbolTransition(start, end, symbol);

  start_end_pair->sf_pair[0] = start;
  start_end_pair->sf_pair[1] = end;

  return start_end_pair;
}

struct List* createFromEpsilon() {
  struct List* start_end_pair = malloc(sizeof(struct List));
  start_end_pair->sf_pair = malloc(2 * sizeof(struct node*));

  struct node* start = createNode(0);
  struct node* end = createNode(1);

  addEpsilonTransition(start, end);

  start_end_pair->sf_pair[0] = start;
  start_end_pair->sf_pair[1] = end;

  return start_end_pair;
}

struct List* concat(struct List* nfa1, struct List* nfa2) {
  addEpsilonTransition(nfa1->sf_pair[1], nfa2->sf_pair[0]);
  nfa1->sf_pair[1]->isFinal = 0;
  // printf("%d", nfa1[1]->node->isFinal);

  struct List* start_end_pair = malloc(sizeof(struct List));
  start_end_pair->sf_pair = malloc(2 * sizeof(struct node*));

  start_end_pair->sf_pair[0] = nfa1->sf_pair[0];
  start_end_pair->sf_pair[1] = nfa2->sf_pair[1];

  return start_end_pair;
}

//union is a keryword in C
struct List* unison(struct List* nfa1, struct List* nfa2) {
  struct node* start = createNode(0);
  addEpsilonTransition( start, nfa1->sf_pair[0] );
  addEpsilonTransition( start, nfa2->sf_pair[0] );

  struct node* end = createNode(1);
  addEpsilonTransition( nfa1->sf_pair[1], end );
  addEpsilonTransition( nfa2->sf_pair[1], end );
  nfa1->sf_pair[1]->isFinal = 0;
  nfa2->sf_pair[1]->isFinal = 0;


  struct List* start_end_pair = malloc(sizeof(struct List));
  start_end_pair->sf_pair = malloc(2 * sizeof(struct node*));

  start_end_pair->sf_pair[0] = start;
  start_end_pair->sf_pair[1] = end;

  return start_end_pair;
}

struct List* closure(struct List* nfa) {
  struct node* start = createNode(0);
  struct node* end = createNode(1);

  addEpsilonTransition( start, nfa->sf_pair[0] );
  addEpsilonTransition( start, end );

  addEpsilonTransition( nfa->sf_pair[1], end );
  addEpsilonTransition( nfa->sf_pair[1], nfa->sf_pair[0] );
  nfa->sf_pair[1]->isFinal = 0;

  struct List* start_end_pair = malloc(sizeof(struct List));
  start_end_pair->sf_pair = malloc(2 * sizeof(struct node*));

  start_end_pair->sf_pair[0] = start;
  start_end_pair->sf_pair[1] = end;

  return start_end_pair;
}


int main()
{
  struct List* a = createFromSymbol('a');
  struct List* b = createFromSymbol('b');
  struct List* c = createFromSymbol('c');
  printf("%c\n", a->sf_pair[0]->symbol_transition->symbol);
  printf("%c\n", b->sf_pair[0]->symbol_transition->symbol);

  struct List* ab = concat(a, b);
  printf("%c\n", ab->sf_pair[0]->symbol_transition->epsilon_transition1->symbol_transition->symbol);

  struct List* aORb = unison(a, b);
  printf("%c\n", aORb->sf_pair[0]->epsilon_transition1->symbol_transition->symbol);

  struct List* c_closure = closure(c);
  printf("%c\n", c_closure->sf_pair[0]->epsilon_transition1->symbol_transition->symbol);

  return 0;
}
