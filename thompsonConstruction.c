#include <stdio.h>
#include <stdlib.h>
#include "infixToPostfix.h"

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
  else {
    perror("Cannot add more than one Symbol Transitions!");
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

  printf("IN CONCAT: %c\n",nfa1->sf_pair[0]->epsilon_transition1->symbol_transition->epsilon_transition1->epsilon_transition1->symbol_transition->symbol);

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

  printf("IN CLOSURE: %c\n",nfa->sf_pair[0]->symbol_transition->symbol);
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
  char* postfixExp = getPostfixExp("a|b*c");
  // printf(  "Captured strlen: %lu\n", strlen( getPostfixExp("(a|b)*c") )  );
  printf("Captured: %s\n", postfixExp);


  //read the postfixExp
  //if its a symbol, createWithSymbol and push graph into stack
  //if its '*' pop the top item in stack and push the return from closure(top item in stack)
  //if its '|' pop the top two item in stack the push the return from unison(top two item in stack)

  int s = 0;
  struct List* stack[100];
  for(int i = 0; i < strlen(postfixExp); i++) {
    printf("postfixExp[%d]: %c", i, postfixExp[i]);
    if(postfixExp[i] == '*') {
      struct List* nfa = stack[--s];
      stack[s++] = closure(nfa);
    }
    else if(postfixExp[i] == '.') {
      struct List* nfa1 = stack[--s];
      struct List* nfa2 = stack[--s];
      stack[s++] = concat(nfa2, nfa1);
    }
    else if(postfixExp[i] == '|') {
      struct List* nfa1 = stack[--s];
      struct List* nfa2 = stack[--s];
      stack[s++] = unison(nfa2, nfa1);
    }
    else {
      stack[s++] = createFromSymbol( postfixExp[i] );
    }
  }

  // printf("s: %d\n", --s);
  printf("IN ENDDD: %c\n",stack[0]->sf_pair[0]->epsilon_transition2->epsilon_transition1->symbol_transition->symbol);

  return 0;
}
