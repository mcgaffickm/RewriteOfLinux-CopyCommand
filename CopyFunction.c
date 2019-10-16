//Everything else is included in there
#include "flags.h"


//Makes the goto cmd more concise and clear of what direction it goes
#define BackTo goto
#define ForwardTo goto

	
/*
 * Main method for input reading
 * */
int main()
{
	//Declaration section
	char input[256];
	char cmd[3], flag[3], readName[50], destination[50];
	char CMD1[3] = "cp\0", CMD2[3] = "ex\0";
	char temp;
	char source[50];
	
	int flagOn = 0;
	int counter;
	int totalSize;

	struct fileNames *currentPos;
	struct fileNames * current;

	//Heading
	printf("Copy Function Program");
	
	///////////////////////////////////////////////////////////////////////////////
	//User input section
	reinput: //Label for return without a loop
		
		//////////
		//A mini clear section, resets everything possible
		clear(cmd, 3);
		clear(flag, 3);
		clear(readName, 50);
		clear(destination, 50);
		totalSize = 0;
		counter = 0;
		struct fileNames firstElement;
		currentPos = NULL;
		errno = 0;
		reset();
		//////////
		
		//Line beginning
		printf("\nUser/");
	
		//Reads in all of a single command line up to 256 characters
		while(1)
		{
			temp = getchar();
			if((temp == '\n') || (temp == EOF))
			{
				input[counter] = '\0';
				break;
			}
			input[counter] = temp;
			counter++;
		}
	///////////////////////////////////////////////////////////////////////////////
	//Test for cmd section
	sscanf(input, "%s", cmd);
	clearFirst(input);
	
	//Checks to see what cmd was entered if a valid one was
	if(compareMethod(cmd, CMD2) == 1) //Exit command
	{
		printf("Program Terminated...");
		return 0;
	}
	else if(compareMethod(cmd, CMD1) == 1) //Copy command
	{
		ForwardTo copyFlag;
	}
	else //Not a valid cmd
	{
		printf("Command is not valid.  Please use cp to copy and ex to exit.");
		BackTo reinput;
	}

	///////////////////////////////////////////////////////////////////////////////
	//Flag test section
	copyFlag:
	
		//Must have next
		if(!hasNext(input))
		{
			printf("Incorrect format: cp [-flag] [source files] [destination files]");
			BackTo reinput;
		}
	
		//Now that the cp is correct, checks for correct flags
		sscanf(input, "%s", readName);
		clearFirst(input);
		
		//No flag detected
		if(readName[0] != '-')
		{
			flagOn = 0;
			ForwardTo files;
		}
		
		//Moves the flag input to flag if flags are to be detected
		flag[0] = readName[0];
		flag[1] = readName[1];
		flag[2] = readName[2];
		
		//Clears the file name array
		clear(readName, 3);
		
		//Invalid flag test
		if(testFlag(flag[1]) == 0)
		{
			printf("You have entered an invalid flag, use cp -h to view the help guide");
			BackTo reinput;
		}
		else
		{
			//Bool for input later
			flagOn = 1;
			
			//Help flag
			if(flag[1] == 'h')
			{
				help();
				BackTo reinput;
			}
			
			//Recursive is handled separately
			if(flag[1] == 'r')
			{
				ForwardTo Recursive;
			}
			
			//Interractive flag, sets int in flags.h
			if(flag[1] == 'i')
			{
				set(1);
			}
			
			//Force flag, sets int in flags.h
			if(flag[1] == 'f')
			{
				set(2);
			}
			
			//Backup flag, sets int in flags.h
			if(flag[1] == 'b')
			{
				set(3);
			}
			
			ForwardTo files;
		}
	///////////////////////////////////////////////////////////////////////////////
	//Files inputs section
	files:
	
		//Must have next
		if(!hasNext(input))
		{
			printf("Incorrect format: cp [-flag] [source files] [destination files]");
			BackTo reinput;
		}
		
		//First determine if a file name still needs to be read
		if(flagOn)
		{
			sscanf(input, "%s", readName);
			clearFirst(input);
		}
		
		//Places the first name into the struct
		combine(firstElement.fileName, readName);
		currentPos = &firstElement;
		totalSize++;
		
		//Must have next
		if(!hasNext(input))
		{	
			printf("Incorrect format: cp [-flag] [source files] [destination files]");
			BackTo reinput;
		}
		
		//Now the loops begins for files
		//Reads everything and adds it to the link list, up until the last element of files to be copied
		while(hasNext(input))
		{
			//Adds to the link list and connects the nodes
			sscanf(input, "%s", readName);
			clearFirst(input);
			totalSize++;
			current = (struct fileNames * ) malloc(sizeof(firstElement));
			combine(current->fileName, readName);
			currentPos->nextFile = current;
			currentPos = currentPos->nextFile;
		}
		
		//No more files to be copied
		currentPos->nextFile = NULL;
		
		////////
		//Destination mini section
		combine(destination, currentPos->fileName);
		
		///////////////////////////////////////////////////////////////////////////////
		//Copy Section
			//Single file to single file copy
			if(totalSize == 2)
			{
				if(copyFiles(firstElement.fileName, destination)==0)
				{printf("Copy Successful");}
			}
			//Directory copy, multi files to one directory
			else
			{
				if(flag[1] == 'b')
				{
					printf("Backup will not occur.  Please use -r to copy directories for backups.\n");
					reset();
				}
				if(copyDestination(firstElement, destination, totalSize)==0)
				{printf("Copy Successful");}
			}
		
		///////////////////////////////////////////////////////////////////////////////
		//Finished, so restart
		BackTo reinput;
		//////////////////////////////////////////////////////////////////////////////
		//Special recursive section
		Recursive:
		
			//Clears the source array
			clear(source, 50);
			
			//Must have next
			if(!hasNext(input))
			{	
				printf("Incorrect format: cp [-flag] [source files] [destination files]");
				BackTo reinput;
			}
			
			//Reads next input for source directory
			sscanf(input, "%s", source);
			clearFirst(input);
			
			//Must have next
			if(!hasNext(input))
			{	
				printf("Incorrect format: cp [-flag] [source files] [destination files]");
				BackTo reinput;
			}
			
			//Reads next input for the destination directory
			sscanf(input, "%s", destination);
			
			//Calls and checks the recursive copy method
			if(recursive(source, destination) == 0)
			{
				printf("Recursive Copy Successful");
			}

		//Returns to restart again
		BackTo reinput;
}

