
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW  50
#define COL  20
#define LSIZ 1000 

char ArrayOfString[ROW][COL]; 
// hold strings after separeted by comma

struct NFA 
// Structure to hold NFA and string with parentheses and commas
{
  char  sym[LSIZ];
  char  states[LSIZ];
  char  start[LSIZ];
  char  final[LSIZ];
  char  trans[LSIZ];
  char  string[LSIZ];
};

struct StackNode // Structure for stack nodes for configuration
{
	const char *state;
  char teststring[LSIZ];
	struct StackNode* next;  // Pointer to the next node
};
struct StackNode *top = NULL;

struct trans // structure to hold all possible transitions. Divide by start, path, destination
{
  char trans_path[ROW][COL];
  char trans_des[ROW][COL];
  char trans_start[ROW][COL];
};

struct two_d // structure to hold NFA and strings without parentheses and commas
{
  char sym_2[ROW][COL];
  char states_2[ROW][COL];
  char start_2[ROW][COL];
  char final_2[ROW][COL];
  char trans_2[ROW][COL];
  char strings_2[ROW][COL];
};

void push(char state[], char NFAstring[]) // function to push the configuration onto the stack
{
	struct StackNode* newNode;
	newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
	newNode->state=state;
  strcpy(newNode->teststring,NFAstring); // copy a string

	if (top == NULL)
	{
		newNode->next = NULL;
		top = newNode;
	}
	else
	{
		newNode->next = top;
		top = newNode;
	}
}

struct StackNode pop(void) // function to pop the configuration out of the stack
{ 
  struct StackNode pop;
	struct StackNode* ptr;
	ptr = top; //ptr will be use to delete a node
	if (top == NULL)
		printf("\n STACK UNDERFLOW\n");  //Stack is empty
	else
	{		
    pop.state=top->state;
    strcpy(pop.teststring,top->teststring);
		top = top->next;
		free(ptr);  // allows to deallocate the memory blocks 
	}
  return pop;
} 

size_t vSeparateSringByComma (char* string) // function to take out the commas from the file
{
  const char *delims = ",\n";
  char *s = string;
  size_t n = 0, len;

  for (s = strtok (s, delims); s && n < ROW; s = strtok (NULL, delims))
  {
    if ((len = strlen (s)) < COL)
      strcpy (ArrayOfString[n++], s);
    else
      fprintf (stderr, "error: '%s' exceeds COL - 1 chars.\n", s); // formats the output
  }
  return n;
}

char* remove_par(char par[]) // function to remove the parentheses
{
  int i=0,j=0;
  char* no_par=malloc(LSIZ);;
  while(par[i] != '\0')
  {
    if(par[i] != '(' &&  par[i] != ')'&&par[i] != ' ')  //Excluding brackets.
      no_par[j++] = par[i];
    i++;
  }
  no_par[j] = '\0';
  return no_par;
}

void deleteList(struct StackNode** head) // function to delete stack
{
    struct StackNode* prev = *head;
 
    while (*head)
    {
        *head = (*head)->next;
        free(prev);
        prev = *head;
    }
}

void algo(struct StackNode conf,struct two_d two_d, int p, int s, struct trans transition) // function to check the configuration given the strings
{ 
 int i; //index
  //loop for the number of strings to test in the file
  for (size_t k=0;k<p;k++)
  { 
    printf("string %s\n",two_d.strings_2[k]);
    //push initial state and string from the file
    push(two_d.start_2[0],two_d.strings_2[k]); 
    //result flag
    int accept=0;
    // loop while the stack is not empty and result flag is 0
    while(top!=NULL && accept==0)
    { 
      //pop configuration and store the configuration to conf
      conf=pop();
      //check for match between current configuration state and final state
      int result = memcmp(conf.state, two_d.final_2[0],2);
      //if configuration string is empty and configuration state is final
      if((strcmp(conf.teststring, "") == 0) && (conf.teststring[0] == '\0') && result==0)
      {
        printf("Accepted.\n");
        accept=1;
      }
      // if configuration stirng is empty but not at final state
      else if((strcmp(conf.teststring, "") == 0) && (conf.teststring[0] == '\0') )
        continue;
      else 
      {
        int i=0, j=0; //index
        char push_string[ROW][COL]; // index 0 of conf.teststring
        push_string[0][0]=conf.teststring[0];
         
        int idxToDel = 0; //index
        memmove(&conf.teststring[idxToDel], &conf.teststring[idxToDel + 1], strlen(conf.teststring) - idxToDel);
       //get rid of the index 0 of conf.teststring
       char next_state[ROW][COL]; //hold next state
      // loop transition relation element to find the next state, if there is, push into the stack
        
        for(i=0;i<s/3;i++)
        {
          if((push_string[0][0]==transition.trans_path[i][0])&&memcmp(conf.state,transition.trans_start[i],1)==0)
          { 
            memcpy(&next_state[i],&transition.trans_des[i],3);
            push(next_state[i],conf.teststring);
          }
        } 
      }
    }
    //if the result flag remains 0, rejected.
    if(accept==0)
      printf("Rejected.\n"); 
    deleteList(&top);
  }
}

// function to check the configuration without given strings
void algo2(struct StackNode conf,struct two_d two_d, int s, struct trans transition)
{
  int k=0, i; //index
  //loop until the user input exit
  do
  {
    printf("\nInput string or type exit to quit: ");
    scanf("%s",two_d.strings_2[k]);

    if (strcmp(two_d.strings_2[k], "exit") == 0)
      break;
    //push initial state and string to stack
    push(two_d.start_2[0],two_d.strings_2[k]); 
    
    int accept=0; // result flag set to 0

    // loop while the stack is not empty and result flag is 0
    while(top!=NULL && accept==0)
    {
      conf=pop();
      
      int result = memcmp(conf.state, two_d.final_2[0],2);
      if((strcmp(conf.teststring, "") == 0) && (conf.teststring[0] == '\0') && result==0)
      {
        printf("\nAccepted\n");
        accept=1;
      }
      else if((strcmp(conf.teststring, "") == 0) && (conf.teststring[0] == '\0') )
        continue;
      else 
      {
        int i=0, j=0;
        char push_string[ROW][COL];
        
        push_string[0][0]=conf.teststring[0];
        int idxToDel = 0; 
        memmove(&conf.teststring[idxToDel], &conf.teststring[idxToDel + 1], strlen(conf.teststring) - idxToDel);
        
        char next_state[ROW][COL];
        for(i=0;i<s/3;i++)
        {
          if((push_string[0][0]==transition.trans_path[i][0])&&memcmp(conf.state,transition.trans_start[i],1)==0)
          {
            memcpy(next_state[i],transition.trans_des[i],3);
            push(next_state[i],conf.teststring);
          }
        } 
      }
    }
   
    if(accept==0)
      printf("\nRejected.\n");
    deleteList(&top);
    i=0;
    ++k;
  }while(1);
  printf("\nBye bye.\n");
}
  

int main()
{  
  FILE *fptr;     // file pointer
  char fname[20]; // file name
  int i = 0;      // 1st index
  int j = 0;      // 2nd index
  
  char empty[] = " ()\n";  // content of a line in file which doesn't have strings to test
  char line[20],line2[20],line3[20];  //read parentheses
  char *sym, *states,*start,*final,*trans,*strings;  // variables to hold return values from remove_par function
  struct NFA nfa;           // Structure name to hold NFA and string from file
  struct StackNode conf;    // structure name to hold configuration
  struct trans transition;  // structure name to hold transition
  struct two_d two_d;       // structure name to hold file contents after parsing them
  
  
  printf("Please input the file name: "); 
  scanf("%s",fname);  
  fptr=fopen(fname,"r");    // open the file fname
  fgets(line, LSIZ, fptr);  // read elements in the file
  fgets(line2, LSIZ, fptr); 
  fgets(nfa.sym, LSIZ, fptr); 
  fgets(nfa.states, LSIZ, fptr); 
  fgets(nfa.start, LSIZ, fptr);
  fgets(nfa.final, LSIZ, fptr); 
  fgets(nfa.trans, LSIZ, fptr);
  fgets(line3, LSIZ, fptr);
  fgets(nfa.string, LSIZ,fptr);
  nfa.string[strcspn(nfa.string, "\n")] = 0; //from the NFA string, the function returns the index of the character found 
  
  sym=remove_par(nfa.sym); //remove parentheses and commas and put the strings into a new string array
  size_t n = vSeparateSringByComma (sym);
  memcpy(&two_d.sym_2,&ArrayOfString,sizeof(two_d.sym_2));
  
  states=remove_par(nfa.states);
  size_t m = vSeparateSringByComma (states);
  memcpy(&two_d.states_2,&ArrayOfString,sizeof(two_d.states_2));

  start=remove_par(nfa.start);
  size_t l = vSeparateSringByComma (start);
  memcpy(&two_d.start_2,&ArrayOfString,sizeof(two_d.start_2));
 
  final=remove_par(nfa.final);
  size_t o = vSeparateSringByComma (final);
  memcpy(&two_d.final_2,&ArrayOfString,sizeof(two_d.final_2));
 
  trans=remove_par(nfa.trans);
  size_t s= vSeparateSringByComma (trans);
  memcpy(&two_d.trans_2,&ArrayOfString,sizeof(two_d.trans_2));
  
  for(i=0;i<s;i+=3) // copy and store the transition relation
  {
    memcpy(transition.trans_start[j],two_d.trans_2[i],2);
    memcpy(transition.trans_path[j],two_d.trans_2[i+1],2);
    memcpy(transition.trans_des[j],two_d.trans_2[i+2],2);
    j++;
  }
  
  if (strncmp(nfa.string,empty, 3) != 0) // if strings are compared and to be tested are given, run algo
  {
    strings=remove_par(nfa.string);
    size_t p= vSeparateSringByComma (strings);
    memcpy(&two_d.strings_2,&ArrayOfString,sizeof(two_d.strings_2));
    algo(conf,two_d, p, s,transition);
  }
  else      // else run algo2 to get user input strings, close the file pointer and return
    algo2(conf,two_d, s,transition);
  fclose(fptr);
  return 0;
}
  