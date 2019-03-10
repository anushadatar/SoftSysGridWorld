
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
// #include <regexpr.h>
#include <pwd.h>
#include "GridWorldShell3.h"

int toExit = 0;

typedef struct SimpleCommand{
  int numberOfAvailableArguments;
  int numberOfArguments;
  char ** arguments;
  // void SimpleCommand();
  // void insertArgument(char *argument);
}simpleCommand;


typedef struct Command{
  int numberOfAvailableSimpleCommands;
  int numberOfSimpleCommands;
  char ** commands;
  struct SimpleCommand ** simpleCommands;
  char * outputFile;
  char * inputFile;
  char * errFile;
  int background;

}command;


static struct Command *currentCommand;
static struct SimpleCommand *currentSimpleCommand;

// currentSimpleCommand->numberOfAvailableArguments = NUMOFARGUMENTS;
// currentSimpleCommand->numberOfArguments = 0;
// currentSimpleCommand->arguments = (char **) malloc(currentSimpleCommand->numberOfArguments * sizeof(char));;

void SimpleCommandInit(){
  currentSimpleCommand = (simpleCommand*)malloc(sizeof(simpleCommand));
  currentSimpleCommand->numberOfAvailableArguments=NUMOFARGUMENTS; // Will change when implmenting realloc()
  currentSimpleCommand->numberOfArguments = 0;
  currentSimpleCommand->arguments = malloc(sizeof(char)*PARAM_BUFF_SIZE * NUMOFARGUMENTS);
  int i;
  for (i = 0; i < NUMOFARGUMENTS; i++)
  {
    currentSimpleCommand->arguments[i] = malloc(sizeof(char) * PARAM_BUFF_SIZE);
  }
}

void insertArgument(char* argument){
  if (currentSimpleCommand->numberOfAvailableArguments == currentSimpleCommand->numberOfArguments) {
    currentSimpleCommand->numberOfAvailableArguments *= SCALEFACTOR;
    currentSimpleCommand->arguments = realloc(currentSimpleCommand->arguments, NUMOFARGUMENTS*SCALEFACTOR * sizeof(simpleCommand));
  }

  int pos = currentSimpleCommand->numberOfArguments;
  currentSimpleCommand->arguments[pos] = argument;
  currentSimpleCommand->numberOfArguments = pos+1;
}


void CommandInit(){
  currentCommand = (command*)malloc(sizeof(command));
  currentCommand->numberOfAvailableSimpleCommands = NUMOFCOMMANDS;
  currentCommand->numberOfSimpleCommands = 0;
  currentCommand->simpleCommands = malloc(sizeof(simpleCommand)* NUMOFCOMMANDS);
  int i;
  for (i = 0; i < NUMOFCOMMANDS; i++)
  {
    currentCommand->simpleCommands[i] = malloc(sizeof(simpleCommand));
  }

}

void prompt(){
  //TODO: make
}

void execute(){

  int tmpin = dup(0);
  int tmpout = dup(1);

  int fdin;
  if (currentCommand->inputFile) {
    fdin = open(currentCommand->inputFile, O_RDONLY);
  }
  else {
    fdin = dup(tmpin);
  }

  int ret;
  int fdout;
  for (int i = 0; i < currentCommand->numberOfSimpleCommands; i++) {
    printf("%s\n", "here");
    dup2(fdin, 0);
    close(fdin);

    if (i == currentCommand->numberOfSimpleCommands - 1) {
      if (currentCommand->outputFile) {
        fdout = open(currentCommand->outputFile, O_WRONLY);
      }
      else {
        fdout = dup(tmpout);
      }
    }
    else {
      int fdpipe[2];
      pipe(fdpipe);
      fdin = fdpipe[0];
      fdout = fdpipe[1];
    }

    dup2(fdout, 1);
    close(fdout);

    ret = fork();
    if (!ret) {
      execvp(currentSimpleCommand->arguments[0], &currentSimpleCommand->arguments[0]);
      perror(currentSimpleCommand->arguments[0]);
      exit(1);
    }
  }

  dup2(tmpin, 0);
  dup2(tmpout, 1);
  close(tmpin);
  close(tmpout);
}

void print(){
  //TODO: make
}

void clear(){
<<<<<<< HEAD
=======

  for (int i = 0; i < currentCommand->numberOfSimpleCommands; i++) {
    for (int j = 0; j < currentCommand->commands[i]->numberOfArguments; j++) {
      if (currentCommand->commands[i]->arguments[j]) {
        free(currentCommand->commands[i]->arguments[j]);
      }
    }
    if (currentCommand->commands[i]->arguments) {
        free(currentCommand->commands[i]->arguments);
    }
    if (currentCommand->commands[i]) {
        free(currentCommand->commands[i]);
    }
  }

  for (int i = 0; i < currentSimpleCommand->numberOfArguments; i++) {
    if (currentCommand->arguments[i]) {
        free(currentCommand->arguments[i]);
    }
  }
  if (currentSimpleCommand->arguments) {
        free(currentCommand->arguments);
  }

  if (currentCommand->inputFile) {
    free(currentCommand->inputFile);
  }
  
  if (currentCommand->outputFile) {
    free(currentCommand->outputFile);
  }

  if (currentCommand->errFile) {
    free(currentCommand->errFile);
  }

  free(currentCommand);
  free(currentSimpleCommand);
>>>>>>> a0ff93701cb64d8c4da69708e75ba0bfa22f26ec
}

void insertSimpleCommand( struct SimpleCommand * simpleCommand ){
  if (currentCommand->numberOfAvailableSimpleCommands == currentCommand->numberOfSimpleCommands) {
    currentCommand->numberOfAvailableSimpleCommands *= SCALEFACTOR;
    currentCommand->simpleCommands = realloc(currentCommand->simpleCommands, NUMOFCOMMANDS * SCALEFACTOR * sizeof(simpleCommand));
  }
  int posOfCommands = currentCommand->numberOfSimpleCommands;
  currentCommand->simpleCommands[posOfCommands] = simpleCommand;
  currentCommand->numberOfSimpleCommands = currentCommand->numberOfSimpleCommands + 1;
}

void shell_loop(){
  while(1){
   SimpleCommandInit();
   insertArgument("man");
   insertArgument("ls");
   CommandInit();
   insertSimpleCommand(currentSimpleCommand);
   printf("%s\n", currentSimpleCommand->arguments[0]);
   printf("%s\n", currentCommand->simpleCommands[0]->arguments[0]);
   execute();
   toExit = 1;  //Tempory Force Break until error handling
    if(toExit){
      break;
    }
  }
}

int main(int argc, char **argv)]{
  shell_loop();
  return 0;
}
