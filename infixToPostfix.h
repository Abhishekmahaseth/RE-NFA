#include <stdio.h>
#include <string.h>

int getPrecedence(char operator) {
  if(operator == '|')
    return 0;
  else if(operator == '.')
    return 1;
  else if(operator == '*')
    return 2;
  else
    return -1;
}

char* getPostfixExp(char* exp) {
  char* expression = malloc(sizeof(char));

  for (int i = 0; i < strlen(exp); i++) {
    if(i == 0)
    {
      strncat(expression, &exp[i], 1);
    } else {
      expression = (char *)realloc(expression, sizeof(char)*strlen(expression)+1);
      strncat(expression, &exp[i], 1);
    }

    if( exp[i] == '(' || exp[i] == '|' ) {
        continue;
    }

    if (i < strlen(exp) - 1) {
        if( exp[i+1] == '*' || exp[i+1] == '|' || exp[i+1] == ')' ) {
            continue;
        }

        char tok = '.';
        expression = (char *)realloc(expression, sizeof(char)*strlen(expression)+1);
        strncat(expression, &tok, 1);
    }
  }
  printf("After adding . : %s\n", expression);

  /*  now expression has '.' in place of all concatination  */
  char stack[200];
  char queue[200];

  //push the operators and openParenthesis into the stack
  //enque the symbols

  //if the current operator has lower precedence, pop and enque that/those symbol from the stack

  int s, q;
  s = q = 0;
  for(int i = 0; i < strlen(expression); i++) {
    char tok = expression[i];

    //tok: '.', '*', '|'
    if (tok == '.' || tok == '*' || tok == '|') {
      // while precedence of OP on top of stack is higher enque
      while( getPrecedence(tok) < getPrecedence(stack[s-1]) ) {
        queue[q++] = stack[--s];
      }
      stack[s++] = tok;
    }
    //tok: (
    else if (tok == '(') {
      stack[s++] = tok;
    }
    //tok: ')'
    else if (tok == ')') {
      while( stack[s-1] != '(' ) {
        queue[q++] = stack[--s];
      } s--; //to remove the '(' as well
    }
    else {
      //tok: symbol
      queue[q++] = tok;
    }


    if(i == strlen(expression) - 1){
      while(s != 0) {
        queue[q++] = stack[--s];
      }
    }
  }

  char* postfix = (char *)malloc(sizeof(char) * --q);
  strcpy(postfix, queue);
  return postfix;
}
