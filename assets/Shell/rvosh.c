#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define RVOSH_RL_BUFFSIZE 1024//allowing for us to have a buffersize of 1024, used for the input reading. Should never be exceeded.
#define RVOSH_TOK_BUFFSIZE 8//allowing 4 char* arguments per action that can be piped together, which we will literally never exceed as we're only expected to do 1 set.
#define RVOSH_TOK_DELIMITER " "
/*Creating a Shell for myself. We make it fun or we don't make it at all. */
//reading in our input, returning a string... I could've bypassed this with getline
char *rvosh_read_line(void)
{
   char *line = NULL;
   size_t bufsize = 0; // have getline allocate a buffer for us

   if (getline(&line, &bufsize, stdin) == -1){
      if (feof(stdin)) {
         exit(EXIT_SUCCESS);  // We recieved an EOF
      } 
      else  {
         perror("readline");
         exit(EXIT_FAILURE);
      }
   }
   return line;
}

char **rvosh_split_line(char line[]){
   int buffsize = RVOSH_TOK_BUFFSIZE;
   int buffsize2 = RVOSH_RL_BUFFSIZE;
   char delimiters[3] = " \n";
   //allocating the memory for the 8 pointers to character arrays.
   
   char **arguments = malloc(buffsize * sizeof(char*));
   for(int i = 0; i < buffsize; i++)
      arguments[i] = malloc(buffsize2 * sizeof(char));
   //generic argument variable.
   char *argument = malloc(buffsize2 * sizeof(char));
   int position = 0;
   if(!arguments) {
      fprintf(stderr, "rvosh: allocation error\n");
      exit(EXIT_FAILURE);
   }
   //using delimiting arguments of space, null value
   argument = strtok(line, delimiters);
   while(argument != NULL){
      //placing our argument from the string into our string array
      arguments[position] = argument;
      position++;
      //allows the loop to run
      argument = strtok(NULL, delimiters);
      if(position >= buffsize){
         buffsize += RVOSH_TOK_BUFFSIZE;
         arguments = realloc(arguments, buffsize);
         if(!arguments) {
            fprintf(stderr, "rvosh: allocation error\n");
            exit(EXIT_FAILURE);
         } 
      }
   }
   //final character in an array should be null
   arguments[position] = NULL;
   return arguments;
}
//following the guide provided up until this point, execute seems like better choice for piping and excluding the built-in commands he provides.
int rvosh_execute(char **arguments){
   pid_t child_pid;
   int status;
   child_pid = fork();
   //the parent process now has value for child_pid while the child does not.
   if(child_pid == 0){
      if(execvp(arguments[0], arguments) == -1){
         perror("no argument provided... help pls");
      }
      exit(EXIT_FAILURE);
   } 
   else {
      if(child_pid < 0){
         printf("fork failed... garbage machine");
         exit(EXIT_FAILURE);
      } 
      else{
         do {
            waitpid(child_pid, &status, WUNTRACED);
         }while(!WIFEXITED(status) && !WIFSIGNALED(status));
      }
   }
   return 1;
}
//function of whether a command has a pipe in it.
int rvosh_findPipedArguments(char** arguments){
   int position = 0;
   while(arguments[position] != NULL)
      position++;
   for(int i = 0; i < position; i++){
      if(!strcmp(arguments[i], "|"))
         return 1;
   }
   return 0;
}
// Function where the piped system commands is executed 
void rvosh_executePipedArguments(char** arguments)
{ 
   // 0 is read end, 1 is write end 
   int pipeEnds[2];
   int buffsize = RVOSH_TOK_BUFFSIZE;
   int buffsize2 = RVOSH_RL_BUFFSIZE;
   int pipeIndex = 0;
   int position = 0;
   //declaring separate arrays for read end and write end arguments
   char **tempArgumentsFirst = malloc(buffsize * sizeof(char*));
   for(int i = 0; i < buffsize; i++)
      tempArgumentsFirst[i] = malloc(buffsize2 * sizeof(char));
   
   char **tempArgumentsSecond = malloc(buffsize * sizeof(char*));
   for(int i = 0; i < buffsize; i++)
      tempArgumentsSecond[i] = malloc(buffsize2 * sizeof(char));
   
   pid_t p1, p2;
   //finding length and position within the array of the pipe.
   while(arguments[position] != NULL)
      position++;
   for(int i = 0; i < position; i++){
      if(!strcmp(arguments[i], "|"))
         pipeIndex = i;
   }
   //making our argument arrays for the pipe.
   for(int i = 0; i < pipeIndex; i++){
      tempArgumentsFirst[i] = arguments[i];
   }
   tempArgumentsFirst[pipeIndex] = NULL;
   //execvp(tempArgumentsFirst[0], tempArgumentsFirst);
   //second argument array
   for(int i = pipeIndex + 1; i < position; i++){
      tempArgumentsSecond[i - (pipeIndex + 1)] = arguments[i];
   }
   tempArgumentsSecond[position - (pipeIndex + 1)] = NULL;
   //execvp(tempArgumentsSecond[0], tempArgumentsSecond);
   if(pipe(pipeEnds) < 0){
      printf("pipe no worko");
      exit(0);
   }
   //initializing child process
   p1 = fork();
   if(p1 == 0){
      //child work, opening write end.
      close(pipeEnds[0]);
      dup2(pipeEnds[1], STDOUT_FILENO);
      //close unused half of pipe
      close(pipeEnds[1]);
      //execute first command
      if(execvp(tempArgumentsFirst[0], tempArgumentsFirst) < 0){
         printf("first arguments didn't work");
         exit(0);
      }
   }
   else{
      p2 = fork();
      if(p2 == 0){
         //opening up read end
         close(pipeEnds[1]);
         dup2(pipeEnds[0], STDIN_FILENO);
         //close unused half of pipe
         close(pipeEnds[0]);
         if(execvp(tempArgumentsSecond[0], tempArgumentsSecond)){
            printf("second arguments didn't work");
            exit(0);
         }
      } 
      else {
         //parent waiting on two children
         close(pipeEnds[1]);
         wait(NULL);
         wait(NULL);
      }
   }
} 
//the loop for oour shell
void rvosh_loop(void){
   char *line; //string
   char **args; //array of strings
   int status; // status int
   int isPipe = 0; //pipe time
   size_t length;
   do {
      //making it appear like the shell
      printf("RVOSH>>");
      //reading input
      line = rvosh_read_line();
      length = strlen(line);
   
      //splitting input into arguments
      args = rvosh_split_line(line);
      isPipe = rvosh_findPipedArguments(args);
      if(isPipe > 0){
         rvosh_executePipedArguments(args);
      }
      else{
         status = rvosh_execute(args);
      }
      free(line);
      free(args);
   }while(status);
}
int main(int argc, char **argv){
   //our main just calls my shell loop, no other calls needed.
   printf("Why use linux shell when we can use RVO Shell, the Snow Crab of Shells.\n");
   rvosh_loop();

   return 0;
}

