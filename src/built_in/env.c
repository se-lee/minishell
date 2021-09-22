#include "../../include/minishell.h"

/*

*/


int main () 
{
   printf("PATH : %s\n", getenv("PATH"));
   printf("USER : %s\n", getenv("USER"));
   printf("HOME : %s\n", getenv("HOME"));
   printf("ROOT : %s\n", getenv("ROOT"));
   printf("AAA : %s\n", getenv("AAA"));

   return(0);
}

