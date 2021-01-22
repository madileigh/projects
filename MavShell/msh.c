/*
  
  Name: Madison Adams

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10    // Mav shell only supports ten arguments

// Function:        int main()
// Parameters:      None
// Return value(s): An int representing whether it exited/finished successfully or unsucessfully
// Description:     A function that is automatically ran by the compiler and calls other functions, 
//                  or has code within it, for the entire program
int main()
{
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );

  int index1 = 0; // for showpids array
  int index2 = 0; // for history array
  int showpids[15]; // showpids array
  char history[15][MAX_COMMAND_SIZE]; // history array
  memset(history, '\0', sizeof(history)); // fill history array up with null terminators
  int i;

  while( 1 )
  {
    // Since the history and showpids command only shows up to
    // the last 15 items (commands or PIDs), restart the indexes
    // for both arrays
    if(index1 == 14)
      index1 = 0;
    if(index2 == 14)
      index2 = 0;

    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    // Set the cmd_str to be the requested command so it can 
    // be correctly parsed into the token array
    if(strstr(cmd_str, "!"))
    {
      int history_index;
      sscanf(cmd_str, "!%d", &history_index);
      // check if command exists in history; if not, loop until
      // the user enters a command that does exist in history
      // or another command altogether that's not in history
      if(strcmp(history[history_index], "\0") != 0)
      {
        strcpy(cmd_str, history[history_index]);
      }
      while(strcmp(history[history_index], "\0") == 0 && strstr(cmd_str, "!"))
      {
        printf("Command not in history.\n\n");
        printf ("msh> ");
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
        if(strstr(cmd_str, "!"))
          sscanf(cmd_str, "!%d", &history_index);
      }
    }

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];
    memset(token, '\0', sizeof(token));

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr;                                         
                                                           
    char *working_str  = strdup( cmd_str );                

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // add the entered command to history array
    if(token[0] != NULL)
    {
      int token_index;
      for( token_index = 0; token_index < token_count-1; token_index ++ ) 
      {
        strcat(history[index2], token[token_index]);
        strcat(history[index2], " ");
      }
      index2++;
    }
    if(token[0] == NULL)
    {
      // do nothing if nothing is entered (will just print "msh>" again)
    }
    else if(strcmp("quit", token[0]) == 0 || strcmp("exit", token[0]) == 0)
    {
      exit(0); // exit with status zero (successful) if user enters "quit" or "exit"
    }
    else if(strcmp("cd", token[0]) == 0)
    {
      chdir(token[1]); // use chdir to change directory (cd), and pass in desired directory (token[1])
    }
    else if(strcmp("showpids", token[0]) == 0)
    {
      // loop through showpids array, displaying up to the last 15 pids
      int pid_index = 0;
      for( pid_index = 0; pid_index < index1; pid_index ++ )
      {
        printf("%d: %d\n", pid_index, showpids[pid_index]);
      }
    }
    else if(strcmp("history", token[0]) == 0)
    {
      // loop through history array, displaying up to the last 15 commands
      int history_index;
      for( history_index = 0; history_index < index2; history_index ++ )
      {
        printf("%d: %s\n", history_index, history[history_index]);
      }
    }
    else // since command wasn't explicitly defined in hw instructions, execute with exec function
    {
      pid_t pid = fork();

      if( pid == -1 ) // fork failed for some reason
      {
        perror("fork failed: ");
        exit( EXIT_FAILURE );
      }
      else if(pid == 0) // in child process (since fork() returns 0 in child process)
      {
        // call exec function with different paths if needed, searching in a certain order
        int ret = execvP(token[0], "./", &token[0]); // search current working directory first
        if( ret == -1 )
        {
          ret = execvP(token[0], "/usr/local/bin", &token[0]); // search /user/local/bin directory second
          if(ret == -1)
          {
            ret = execvP(token[0], "/usr/bin", &token[0]); // search /user/bin directory third
            if(ret == -1)
            {
              ret = execvP(token[0], "/bin", &token[0]); // search /bin directory fourth
              if(ret == -1)
              {              
                printf("%s\n\n", strcat(token[0], ": Command not found."));
              }
            }       
          }
        }
        exit(0);
      }
      else // in parent process
      {
        int status;
        wait(&status); // wait for child process to finish
        showpids[index1] = pid; // store child PID (since fork() returns child PID in parent process)
        index1++;
      }
      fflush(NULL);
    }
    memset(token, '\0', sizeof(token)); // clear token array by setting null terminator

    free( working_root );
  }
  memset(history, '\0', sizeof(history)); // clear history array by setting null terminator
  memset(showpids, 0, sizeof(showpids)); // clear showpids array by setting null terminator
  
  return 0;
}
