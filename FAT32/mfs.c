// Names:          Madison Adams and Hemanta Lawaju
// IDs:             1001543026   and  1001716565
// Class/Section:           CSE 3320-003

// The MIT License (MIT)
// 
// Copyright (c) 2020 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NUM_ARGUMENTS 10 // just like Mav shell, it only supports ten arguments

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

// each record can be represented by:
struct __attribute__((__packed__)) DirectoryEntry 
{
  char DIR_Name[11];
  uint8_t DIR_Attr;
  uint8_t Unused1[8];
  uint16_t DIR_FirstClusterHigh;
  uint8_t Unused2[4];
  uint16_t DIR_FirstClusterLow;
  uint32_t DIR_FileSize;
};

char    BS_OEMName[8];
int16_t BPB_BytesPerSec;
int8_t  BPB_SecPerClus;
int16_t BPB_RsvdSecCnt;
int8_t  BPB_NumFATs;
int16_t BPB_RootEntCnt;
char    BS_VolLab[11];
int32_t BPB_FATSz32;
int32_t BPB_RootClus;

int32_t RootDirSectors = 0;
int32_t FirstDataSector = 0;
int32_t FirstSectorofCluster = 0;

FILE * fp;

char * to_upper(char * str)
{
  char * temp = ( char * )( malloc(strlen( str )) );
  strcpy( temp, str );
	char * s = temp;
	while( *s )
	{
		*s = toupper( ( unsigned char ) *s);
		s++;
	}
	return temp;	
}
// provided functions from FAT.pdf slides and class GitHub
int compare(char * IMG_Name, char * input)
{
	char *temp_IMG_Name = (char *)( malloc(12) );
	strcpy( temp_IMG_Name, IMG_Name);
	*(temp_IMG_Name + 11) = '\0';

	char *temp_input = (char*)( malloc(strlen( input )));
	strcpy(temp_input, input);
	
	char *expanded_name = (char*)( malloc(12) );
	memset( (void*)expanded_name, ' ', 12);
	
	char *token = strtok(temp_input, ".");

	strncpy( expanded_name, token, strlen(token));
	token = strtok( NULL, "." );

	if( token!=NULL)
	{	
		strncpy( ( expanded_name + 8 ), token, strlen( token ));
	
		*(expanded_name + 11 ) = '\0';
	}	
	else
	{
		*(expanded_name  + 11 ) = '\0';		
	}
	if( strcmp( temp_IMG_Name, expanded_name ) == 0)
	{
		free(temp_IMG_Name);
		free(temp_input);
		free(expanded_name);
		return 1;
	}
	else
	{
		free(temp_IMG_Name);
		free(temp_input);
		free(expanded_name);
		return 0;
	}
}
/*
 *Function    : LBAToOffset
 *Parameters  : The current sector number that points to a block of data
 *Returns     : The value of the address for that block of data
 *Description : Finds the starting address of a block of data given the sector number
 *corresponding to that data block
*/
int LBAToOffset(int32_t sector)
{
  if(sector < 2)
  {
    sector = 2;
  }
  return (( sector - 2 ) * BPB_BytesPerSec) + (BPB_BytesPerSec * BPB_RsvdSecCnt) + (BPB_NumFATs * BPB_FATSz32 * BPB_BytesPerSec);
}
/*
  Name: NextLB
  Purpose: Given a logical block address, look up into the first FAT and
  return the logical block address of the block in the file.  If
  there is no further blocks then return -1
*/
int16_t NextLB( uint32_t sector )
{
  uint32_t FATAddress = ( BPB_BytesPerSec * BPB_RsvdSecCnt ) + ( sector * 4 );
  int16_t val;
  fseek( fp, FATAddress, SEEK_SET );
  fread( &val, 2, 1, fp );
  return val;
}

int main()
{
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );

  struct DirectoryEntry dir[16];
  int cwd = 0; // current working directory
  int i = 0;

  while( 1 )
  {
    // Print out the mfs prompt
    printf ("mfs> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;                                         
                                                           
    char *working_str  = strdup( cmd_str );                

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    if(token[0] == NULL)
    {
      // do nothing if nothing is entered (will just print "mfs>" again)
    }
    else if(strcmp("quit", token[0]) == 0 || strcmp("exit", token[0]) == 0)
    {
      exit(0); // exit with status zero (successful) if user enters "quit" or "exit"
    }
    else if( strcmp("close", token[0]) == 0 )
    {
      if( fp ) // fp is open, so close it and set it to NULL
      {
        fclose(fp);
        fp = NULL;
      }
      else // fp isn't open
      {
        printf("Error: File system not open.\n");
      }
    }
    else if( fp == NULL && strcmp("open", token[0]) != 0 ) // fp hasn't been opened yet
    {
      printf("Error: File system image must be opened first.\n");
    }
    else // need to open image or run a command on open image
    {
      if( strcmp("open", token[0]) == 0 ) // fp either needs to be opened or already has been opened
      {
        char filename[100];
        if(token[1] == NULL)
        {
          printf("Error: Must enter a filename.\n");
          continue;
        }
        sscanf( token[1] , "%s" , filename ); 
        filename[strlen(filename)] = '\0'; // set last element to be null terminator

        FILE * fp2 = fopen( filename , "r" ); // try to open file and see if it opens using fp2, if it does, store fp2 in fp

        if( fp2 ) // fp2 exists
        {
          // check if fp has already opened an image
          if( fp )
          {
            printf("Error: File system image already open.\n");
          }
          else if( fp2 ) // fp isn't open and equals NULL
          {
            fp = fp2;

            // fill FAT32 variables
            fseek( fp, 3, SEEK_SET);
            fread( BS_OEMName, 8, 1, fp );

            // referenced in bpb
            fseek( fp, 11, SEEK_SET);
            fread( &BPB_BytesPerSec, 2, 1, fp );

            // referenced in bpb
            fseek( fp, 13, SEEK_SET);
            fread( &BPB_SecPerClus, 1, 1, fp);

            // referenced in bpb
            fseek( fp, 14, SEEK_SET );
            fread( &BPB_RsvdSecCnt, 2, 1, fp);	

            // referenced in bpb
            fseek( fp, 16, SEEK_SET);	
            fread( &BPB_NumFATs, 1, 1, fp);

            fseek( fp, 17, SEEK_SET);
            fread( &BPB_RootEntCnt, 2, 1, fp );

            fseek( fp, 71, SEEK_SET);
            fread( BS_VolLab, 11, 1, fp );

            // referenced in bpb
            fseek( fp, 36, SEEK_SET );
            fread( &BPB_FATSz32, 4, 1, fp);

            fseek( fp, 44, SEEK_SET);
            fread( &BPB_RootClus, 4, 1, fp );

            RootDirSectors = ((BPB_RootEntCnt * 32) + (BPB_BytesPerSec - 1)) / BPB_BytesPerSec;
            FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32) + RootDirSectors;

            int N = LBAToOffset(FirstDataSector); // not sure if this is right!!
            FirstSectorofCluster = ((N - 2) * BPB_SecPerClus) + FirstDataSector; // not sure if this is right!!

            // store initial cwd (current working directory)
            cwd = LBAToOffset(BPB_RootClus);

            // fill dir array
            fseek(fp, cwd, SEEK_SET);
            for(i = 0; i < 16; i++)
            {
              fread(dir[i].DIR_Name, 1, 11, fp);
              fread(&dir[i].DIR_Attr, 1, 1, fp);
              fread(&dir[i].Unused1, 1, 8, fp);
              fread(&dir[i].DIR_FirstClusterHigh, 2, 1, fp);
              fread(&dir[i].Unused2, 1, 4, fp);
              fread(&dir[i].DIR_FirstClusterLow, 2, 1, fp);
              fread(&dir[i].DIR_FileSize, 4, 1, fp);
            }
          }
        }
        else // fp2 doesn't exist so don't do anything
        {
          printf("Error: File system image not found.\n");
        }
      }
      else if( strcmp("history", token[0]) == 0 )
      {
        // add code for history
        /* old command (get code from msh.c - assignment/lab 1 - and merge here into mfs.c) */
      }
      else if( strcmp("bpb", token[0]) == 0 )
      {
        // prints information about file system in base 10 and hexadecimal
        printf("BPB_BytesPerSec: %d\n", BPB_BytesPerSec);
        printf("BPB_BytesPerSec: %x\n\n", BPB_BytesPerSec);

        printf("BPB_SecPerClus: %d\n", BPB_SecPerClus);
        printf("BPB_SecPerClus: %x\n\n", BPB_SecPerClus);

        printf("BPB_RsvdSecCnt: %d\n", BPB_RsvdSecCnt);
        printf("BPB_RsvdSecCnt: %x\n\n", BPB_RsvdSecCnt);

        printf("BPB_NumFATs: %d\n", BPB_NumFATs);
        printf("BPB_NumFATs: %x\n\n", BPB_NumFATs);

        printf("BPB_FATSz32: %d\n", BPB_FATSz32);
        printf("BPB_FATSz32: %x\n\n", BPB_FATSz32);
      }
      else if( strcmp("stat", token[0]) == 0 )
      {
        fseek( fp, cwd, SEEK_SET ); 
        fread( dir, 16, sizeof( struct DirectoryEntry ), fp );
        bool exists = false;
        char * input;

        if(token[1])
        {
          input = ( char * ) malloc( strlen( token[1] + 1 ) );
          strcpy( input, token[1] );	
        }
        else
        {
          printf("Error: Must enter a filename.\n");
          continue;
        }
        char * upper_input = to_upper( input );
        for( i = 0; i < 16; i++ )
        {
          char * IMG_Name = ( char * ) malloc( 12 );
          strncpy( IMG_Name, dir[i].DIR_Name, 11 );
          * ( IMG_Name + 11 ) = '\0';
          if( compare( IMG_Name, upper_input) ) 
          {
            printf( "File Attribute	 	Size		Starting Cluster Number\n" );
            printf( "%d			%d		%d\n", dir[i].DIR_Attr, dir[i].DIR_FileSize, dir[i].DIR_FirstClusterLow );
            exists = true;
            free( IMG_Name );
            break;
          }
          free( IMG_Name );
        }
        if(!exists)
        {
          printf("Error: File not found\n");
        }
      }
      else if( strcmp("get", token[0]) == 0 )
      {
        bool exists = false;
        char * filename;
        char * new_filename;

        // check if a filename was entered
        if(token[1] == NULL)
        {
          printf("Error: Must enter a filename.\n");
          continue;
        }
        filename = ( char * ) malloc( strlen( token[1] + 1 ) );
        strcpy(filename, token[1]);

        // check if a new_filename was also entered
        if(token[2])
        {
          new_filename = ( char * ) malloc( strlen( token[2] + 1 ) );
          strcpy(new_filename, token[2]);
        }
        else // user entered "get <filename>" and not "get <filename> <new filename>"
        {
          new_filename = ( char * ) malloc( strlen( token[1] + 1 ) );
          strcpy(new_filename, filename);
        }

        char * upper_filename = to_upper( filename );
        char * upper_new_filename = to_upper( new_filename );
        
        // loop through dir
        for (i = 0; i < 16; i++)
        {
          char * IMG_Name = ( char * ) malloc( 12 );
          strncpy( IMG_Name, dir[i].DIR_Name, 11 );
          * ( IMG_Name + 11 ) = '\0';
          if ( compare(IMG_Name, upper_filename) ) // found correct dir
          {
            FILE * fp2 = fopen(new_filename, "w");
            fseek(fp, LBAToOffset(dir[i].DIR_FirstClusterLow), SEEK_SET);
            uint8_t ptr[dir[i].DIR_FileSize];
            fread(&ptr, dir[i].DIR_FileSize, 1, fp);
            fwrite(&ptr, dir[i].DIR_FileSize, 1, fp2);
            fclose(fp2);
            exists = true;
            free(IMG_Name);
            break;
          }
          free(IMG_Name);
        }
        if(!exists)
        {
          printf("Error: File not found\n");
        }
        free(filename);
        free(new_filename);
      }
      else if( strcmp("cd", token[0]) == 0 )
      {        
        bool exists = false;
        char * input;

        // check if a directory was entered
        if(token[1] == NULL)
        {
          printf("Error: Must enter a directory.\n");
          continue;
        }
        // check if directory entered is ..
        if( strcmp("..", token[1]) == 0 )
        {
          // check if you can't go back anymore in the directory (in root)
          if(cwd == LBAToOffset(BPB_RootClus))
          {
            printf("You are in root directory and cannot go further back.\n");
            continue;
          }
          // loop through dir
          for (i = 0; i < 16; i++)
          {
            char * IMG_Name = ( char * ) malloc( 12 );
            strncpy( IMG_Name, dir[i].DIR_Name, 2 );
            * ( IMG_Name + 2 ) = '\0';
            if ( strcmp(IMG_Name, token[1]) == 0 ) // found correct dir
            {
              if(dir[i].DIR_FirstClusterLow != BPB_RootClus)
              {
                cwd = LBAToOffset(dir[i].DIR_FirstClusterLow);                        
              }
              else
              {
                cwd = LBAToOffset(BPB_RootClus);
              }
              free(IMG_Name);
              break;
            }
            free(IMG_Name);
          }
          fseek(fp, cwd, SEEK_SET);
          // fill dir array
          for(i = 0; i < 16; i++)
          {
            fread(dir[i].DIR_Name, 1, 11, fp);
            fread(&dir[i].DIR_Attr, 1, 1, fp);
            fread(&dir[i].Unused1, 1, 8, fp);
            fread(&dir[i].DIR_FirstClusterHigh, 2, 1, fp);
            fread(&dir[i].Unused2, 1, 4, fp);
            fread(&dir[i].DIR_FirstClusterLow, 2, 1, fp);
            fread(&dir[i].DIR_FileSize, 4, 1, fp);
          }
          continue;
        }
        else if( strcmp(".", token[1]) == 0 )
        {
          continue;
        }

        input = ( char * ) malloc( strlen( token[1] + 1 ) );
        strcpy(input, token[1]);

        char * upper_input = to_upper( input );

        // loop through dir
        for (i = 0; i < 16; i++)
        {
          char * IMG_Name = ( char * ) malloc( 12 );
          strncpy( IMG_Name, dir[i].DIR_Name, 11 );
          * ( IMG_Name + 11 ) = '\0';
          if ( compare(IMG_Name, upper_input) ) // found correct dir
          {
            cwd = LBAToOffset(dir[i].DIR_FirstClusterLow);

            // fill dir array
            fseek(fp, cwd, SEEK_SET);
            for(i = 0; i < 16; i++)
            {
              fread(dir[i].DIR_Name, 1, 11, fp);
              fread(&dir[i].DIR_Attr, 1, 1, fp);
              fread(&dir[i].Unused1, 1, 8, fp);
              fread(&dir[i].DIR_FirstClusterHigh, 2, 1, fp);
              fread(&dir[i].Unused2, 1, 4, fp);
              fread(&dir[i].DIR_FirstClusterLow, 2, 1, fp);
              fread(&dir[i].DIR_FileSize, 4, 1, fp);
            }
            exists = true;
            free(IMG_Name);
            break;
          }
          free(IMG_Name);
        }
        if(!exists)
        {
          printf("Error: Directory not found\n");
        }
        free(input);
      }
      else if( strcmp("ls", token[0]) == 0 )
      {
        int temp_cwd = cwd;
        struct DirectoryEntry temp_dir[16];
        bool ls_parent = false;
        if(token[1])
        {
          // check if directory entered is ..
          if( strcmp("..", token[1]) == 0 )
          {
            // loop through dir
            for (i = 0; i < 16; i++)
            {
              char * IMG_Name = ( char * ) malloc( 12 );
              strncpy( IMG_Name, dir[i].DIR_Name, 2 );
              * ( IMG_Name + 2 ) = '\0';
              if ( strcmp(IMG_Name, token[1]) == 0 ) // found correct dir
              {
                if(dir[i].DIR_FirstClusterLow != BPB_RootClus)
                {
                  temp_cwd = LBAToOffset(dir[i].DIR_FirstClusterLow);                        
                }
                else
                {
                  temp_cwd = LBAToOffset(BPB_RootClus);
                }
                fseek(fp, temp_cwd, SEEK_SET);
                for(i = 0; i < 16; i++)
                {
                  fread(temp_dir[i].DIR_Name, 1, 11, fp);
                  fread(&temp_dir[i].DIR_Attr, 1, 1, fp);
                  fread(&temp_dir[i].Unused1, 1, 8, fp);
                  fread(&temp_dir[i].DIR_FirstClusterHigh, 2, 1, fp);
                  fread(&temp_dir[i].Unused2, 1, 4, fp);
                  fread(&temp_dir[i].DIR_FirstClusterLow, 2, 1, fp);
                  fread(&temp_dir[i].DIR_FileSize, 4, 1, fp);
                }
                ls_parent = true;
                free(IMG_Name);
                break;
              }
              free(IMG_Name);
            }
          }
          else if( strcmp(".", token[1]) == 0 )
          {
            // do nothing
          }
        }
        fseek(fp, cwd, SEEK_SET);

        if(ls_parent)
        {
          // loop through temp_dir
          for (i = 0; i < 16; i++)
          {
            // should not list deleted files or system volume names
            if((temp_dir[i].DIR_Name[0] != 0xFFFFFFE5) && (temp_dir[i].DIR_Name[0] != 0x05) && (temp_dir[i].DIR_Name[0] != 0x00) && (temp_dir[i].DIR_Name[0] != 0x20) && 
            ((temp_dir[i].DIR_Attr == 0x01) || (temp_dir[i].DIR_Attr == 0x10) || (temp_dir[i].DIR_Attr == 0x20)))
            {
              char * IMG_Name = ( char * ) malloc( 12 );
              strncpy( IMG_Name, temp_dir[i].DIR_Name, 11 );
              * ( IMG_Name + 11 ) = '\0';
              printf("%s\n", IMG_Name);
              free(IMG_Name);
            }
          }
        }
        else
        {
          // loop through dir
          for (i = 0; i < 16; i++)
          {
            // should not list deleted files or system volume names
            if((dir[i].DIR_Name[0] != 0xFFFFFFE5) && (dir[i].DIR_Name[0] != 0x05) && (dir[i].DIR_Name[0] != 0x00) && (dir[i].DIR_Name[0] != 0x20) && 
            ((dir[i].DIR_Attr == 0x01) || (dir[i].DIR_Attr == 0x10) || (dir[i].DIR_Attr == 0x20)))
            {
              char * IMG_Name = ( char * ) malloc( 12 );
              strncpy( IMG_Name, dir[i].DIR_Name, 11 );
              * ( IMG_Name + 11 ) = '\0';
              printf("%s\n", IMG_Name);
              free(IMG_Name);
            }
          }
        }
      }
      else if( strcmp("read", token[0]) == 0 )
      {
        bool exists = false;
        char * filename;
        int position = 0;
        int num_of_bytes = 0;

        if(token[1] && token[2] && token[3]) // filename, position, and number of bytes were input
        {
          fseek( fp, cwd , SEEK_SET) ; 
          fread( dir, 16 ,sizeof( struct DirectoryEntry ), fp );
          
          char *input = (char *) malloc( strlen( token[1]+1 ) );
          strncpy( input, token[1], strlen(token[1]) );
          input[strlen(token[1])] = '\0';
          char *upper_input = to_upper(input);
          char *file = ( char *)(malloc( atoi(token[3]) ));

          int cluster, offset ;
          for( i = 0; i < 16; i++ )
          {
            char *dir_name = (char *) malloc( 12 );
            strncpy( dir_name, dir[i].DIR_Name, 11);
            dir_name[11] = '\0';

            if( compare(  dir_name, upper_input) ) 
            {
              cluster  = dir[i].DIR_FirstClusterLow ; 
              int size = atoi( token[3] );

              unsigned char buffer[512];
              
              // find the first block
              int block_number = atoi( token[2] ) / 512;
              int j;
              for( j = 0; j < block_number; j++ )
              {
                cluster = NextLB( cluster );
              }
              int num_bytes;
              offset = LBAToOffset( cluster ) + ( int )( atoi( token[2] ) % 512 );
              fseek( fp, offset, SEEK_SET );
              fread( buffer, 1, ( 512 - ( 512 - atoi( token[3])  % 512 ) ) , fp );

              for( i = 0; i <  ( 512 - ( 512 - atoi( token[3] ) % 512 ) ); i++ )
              {
                printf("%x ", buffer[i] );
              }
              size = size - ( 512 - ( atoi( token[2] ) % 512 ) );

              cluster = NextLB( cluster );

              // handle the middle of the file
              while( size >= 512 )
              {
                int offset = LBAToOffset( cluster );
                fseek( fp, offset, SEEK_SET );
                fread( buffer, 1, 512, fp );
                int k;
                for( k = 0; k < 512; k++ )
                {
                  printf("%x ", buffer[1] );
                }
                size = size - 512;
                cluster = NextLB( cluster );
              }

              // handle the last block of the file
              if( size )
              {
                int offset = LBAToOffset( cluster );
                fseek( fp, offset, SEEK_SET );
                fread( buffer, 1, size, fp );

                int l;
                for( l = 0; l < size; l++ )
                {
                  printf("%x ", buffer[1] );
                }
                printf("\n");
              }
            }
            free(dir_name);
          }
        }
        else
        {
          printf("Error: Please use right format. Must enter filename, position, and number of bytes.\n");
          continue;
        }
      }
      else // command not found
      {
        printf("Error: command not found.\n");
      }
    }
    free( working_root );
  }
  return 0;
}
