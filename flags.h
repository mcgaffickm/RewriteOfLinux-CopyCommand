#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "otherMethods.h"
#include <sys/stat.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

//Link list to hold file names
struct fileNames
	{
		char fileName[50];
		struct fileNames *nextFile;
	};
	
//Variable to hold flag condition type
int typeCopy;

//Declaration of method that uses and is used in method above it
int backup(char * src, char * dest);

/*
 * Sets the copy flag value
 * */
void set(int i)
{
	typeCopy = i;
}

/*
 * Resets the copy flag
 * */
void reset()
{
	typeCopy = 0;
}
	
/*
 * Use for the -i flag 
 * */
int interractive(char * dest)
{
	//Prompts user to overwrite current
	char input = ' ';
	printf("Do you wish to overwrite existing file: %s? (Y or N): ", dest);
	input = getchar();
	getchar(); //Flush \n
	if(input == 'Y') 
		{return 1;} //Continue
	else if(input == 'N') 
		{return 0;} //Abort
	else
	{
		printf("Input: %c not recognized.", input);
		return interractive(dest);
	}
}

/*
 * Used for -f flag
 * */
FILE * force(char * destination)
{
	//Removes the current file and replaces it with a new blank one to be copied into
	printf("Force Removal Occuring\n");
	remove(destination);
	int fd = creat(destination, 0777);
	if(fd <= 0)
		{
			printf("An error has occurred creating the destination file.");
			return NULL;
		}
	return fopen(destination, "w");
}

/*
 * Copys file to file
 * */
int copyFiles(char * file, char * destination)
{
	//Declaration
	char temp;
	int j = 1;
	FILE *src = fopen(file, "r");
	FILE * dest = fopen(destination, "rw");
	
	//Checks the source file
	if(src == NULL)
	{
		printf("An error has occurred when opening the source file.");
		return -1;
	}
	
	
	//Determines if the file exist (destination)
	if(dest == NULL)
	{
		//Creates the new file and test to make sure it can be opened
		int fd = creat(destination, 0777);
		if(fd <= 0)
		{
			printf("An error has occurred creating the destination file.");
			fclose(src);
			return -1;
		}
		
		//Opens for writing
		dest = fopen(destination, "w");
		if(dest == NULL)
		{
			printf("An error has occurred opening the destination file.");
			fclose(src);
			return -1;
		}
	}
	else
	{
		//Closes the file since it can't be opened for rw
		fclose(dest);
		
		//Prompts for -i flag if entered
		if(typeCopy == 1)
		{
			if(!interractive(destination))
			{
				
				fclose(src);
				return -1;
			}
		}
		
		//Uses -f removal if entered
		if(typeCopy == 2)
		{
			dest = force(destination);
		}
		
		//Opens for writing
		dest = fopen(destination, "w");
	}	

	//Loops through and places one bit into the the next file
	while(j)
	{
		errno = 0;
		temp = fgetc(src);
		if(temp == EOF)
		{
			break;
		}
		
		fputc(temp, dest);
		
		if(errno != 0)
		{
			printf("An error has occurred while copying the files.");
			return -1;
		}
	}
	
	//Closes the files
	fclose(src);
	fclose(dest);

	//Copy Backup for the file is -b was entered
	if(typeCopy == 3)
	{
		if(backup(file,destination) != 0)
		{
			printf("Backup Failed\n");
		}
	}
	
	return 0;
}

/*
 * Copies files to destination
 * 
 * */
int copyDestination(struct fileNames files, char * dest, int size)
{
	//Sets errno to zero and changes size to match source files
	errno = 0;
	struct fileNames * current = &files;
	size--;
	
	//First test the destination
	DIR * dir = opendir(dest);
	
	//Creates a new directory if needed
	if(dir == NULL)
	{
		//Removes the existing directory (NEEDS TO BE EMPTY)
		rmdir (dest);
		
		//Makes a new directory and checks to make sure that it doesn't exist
		int temp = mkdir(dest, 0777);
		if(temp)
		{
			printf("An error has occurred creating the destination directory");
			return -1;
		}
		
		//Opens the directory and checks it, which shouldn't ever occur as a failure
		dir = opendir(dest);
		if(dir == NULL)
		{
			printf("An error has occurred opening the destination directory");
			return -1;
		}
	}
	
	//No need to have this open
	closedir(dir);
	
	//The slash character and file name extender
	char c[] = "/";
	char bigger[250];
	
		
	//Loops through, creating name for destination and copying files
	while(1)
	{
		//If the element is same as destination, at the end of the link list
		if(compareMethod(current->fileName, dest))
		{
			break;
		}
		
		//Puts the path into the file name for the copy
		clear(bigger, 250);
		combine(bigger, dest);
		combine2(bigger,c);
		combine2(bigger, current->fileName);
		
		//Copies file name to the new path name 
		copyFiles(current->fileName,bigger);
		current = current->nextFile;
		size--;
	}
	return 0;
}


/*
 * Used for the -b flag 
 * */
int backup(char * src, char * dest)
{
	//The new backup beginning
	char c[] = "BACKUp~";
	
	//Creates a bigger value to hold the path name
	char bigger[300];
	clear(bigger, 300);
	combine(bigger, c);
	combine2(bigger, dest);
	
	//Removes backup flag so no infinite recursions
	reset();
	int test = copyFiles(src, bigger);
	
	//Turns flag back on for future backups
	set(3);
	return test;
}

/*
 * Use for the -h flag
 * 
 * */
void help()
{
	//Prints the help information
	printf("Description: the cp cmd copies files to a source destination or file to file.\n"
			"Format: cp [flag] [source file(s)] [destination directory/file]\n"
			"Flags:\n\t-b backup creates a second copied file with BACKUP~ name in current directory\n"
			"\t-i interactive prompts user for confirmation before overwriting data\n"
			"\t-f force deletes the destination file and creates a new one with same name\n"
			"\t-r recursive copies an entire directory to the destination directory.");
}
	
/*
 * Use for the -r flag
 * */
int recursive(char * src, char * dest)
{
	//Copies directory to directory
	//The destination should not exist, this also clears out .. and .
	if(mkdir(dest, 0777))
	{	
			return -1;
	}
	
	//Declares the structs to read file names
	struct dirent dc;
	struct dirent *entry = (struct dirent *) malloc(sizeof(dc));
	
	//Declares arrays
	char c[] = "/";
	char temp[500];
	char temp2[500];
	
	//Opens current directory
	DIR * dir = opendir(src);
	
	//This only should occur on the first pass
	if(dir == NULL)
	{return -1;}

	//Loops through while elements exist
	while((entry = readdir(dir)) != NULL)
	{
		//Resets
		clear(temp, 500);
		clear(temp2, 500);
	
		
		//Makes the source path
		combine(temp, src);
		combine2(temp, c);
		combine2(temp, entry->d_name);
		
		//Makes the destination path
		combine(temp2, dest);
		combine2(temp2, c);
		combine2(temp2, entry->d_name);
		
		//Determines if it is a directory inside of a directory
		if(opendir(temp) != NULL)
		{
			//Calls this method again and loops through to check subfolder
			recursive(temp, temp2);
		}
		else
		{
			//Copies the file to the source destination
			if(copyFiles(temp, temp2) != 0){return -1;} //Fail test
		}
	}
	//Closes directory
	closedir(dir);
	return 0;
}
