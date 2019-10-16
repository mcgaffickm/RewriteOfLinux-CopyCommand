/*
 * Emptys out a char array
 * @param a the array to be cleaned
 * @param size the number of spaces to be cleared
 * 
 * */
 void clear(char * a, int size)
 {
	//Loops through making everything a space
	for(int j = 0; j < size; j++)
	{
		a[j] = ' ';
	} 
 }

/*
 * Clears first nonspace element of a string
 * @param a the string to be emptied
 * 
 * */
 void clearFirst(char * a)
 {
	int getStringSize(char * a);
	int found = 0;
	
	//Finds the first word of a string and removes it
	for(int j = 0; j < getStringSize(a); j++)
	{
		if(a[j] == '\0')
		{
			return;
		}
		
		if(!found){
			if(a[j] != ' ')
			{
				found++;
				a[j] = ' ';
			}
		}
		else
		{
			if(a[j] == ' ')
			{
				return;
			}
			else
			{
				a[j] = ' ';
			}
		}
	} 
 }

/*
 * Combines one array into another, assumed sizes are fine
 * @param a the empty array
 * @param b the array to be copied over
 * */
void combine(char * a, char * b)
{
	int getStringSize(char * a);
	
	//Loops through and adds b's elements into a at the beginning
	for(int j = 0; j < getStringSize(b); j++)
	{
		a[j] = b[j];
		if(a[j] == '\0')
			return;
	}
}

/*
 * Takes the b array and puts a at the end, assumes there are no spaces
 * @param a changed array
 * @param b the array to be copied over
 * */
void combine2(char * a, char * b)
{
	int getStringSize(char * a);
	
	//Gets a to the empty spot
	while(*a != '\0')
	{
		a++;
	}
	
	//Perserves a's data except ending spaces and adds b to the end
	combine(a,b);
}

/*
 * Compares to string
 * @param a is the user string
 * @param b is the constant string
 * @return 1 for equal and 0 for not equal
 * 
 * */
int compareMethod(char * a, char * b)
{
	int j = 0;
	
	//Loops until not equal or ending is met
	while(1)
	{
		if(a[j] == b[j]) //Equal
		{
			if(a[j] == '\0') break;
		}
		else //Not equal
		{
			return 0;
		}
		j++;
	}
	return 1;	
}

/*
 * Gets the length of the string
 * @param a is the string
 * @return the size of the string
 * 
 * */
int getStringSize(char * a)
{
	int counter = 0;
	
	//Counts size of string, for clearing and combine methods
	while(1)
	{
		counter++;
		if(a[counter-1] == '\0')
		{
			return counter;
		}
	}
}
/*
 * Test if one + more values remain
 * @param the array to test
 * @return 0 for false, 1 for true
 * */
int hasNext(char * a)
{
	//Checks that there is a non space element remaining in a string
	for(int j = 0; j < getStringSize(a); j++)
	{
		if(a[j] == '\0') break;
		if(a[j] != ' ')
		{
			return 1;
		}
	}
	return 0;
}


/*
 * Tests to see if a valid flag was used
 * @param a user entered flag
 * @return 1 for true and 0 for false
 * 
 * */
int testFlag(char a)
{
	//Compares entered flag to list of available flags
	const char flags[6] = {"bfhir"};
	int count = 0;
	while(1)
	{
		if(flags[count] == '\0') {break;}
		if(a == flags[count]) return 1;
		count++;
	}
	return 0;
}
