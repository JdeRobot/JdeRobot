/* Routines for threating with dictionary struct */

#include "Dictionary.h"

/* Initialization of a dictionary struct */
void
Dictionary::createDictionary(void)
{
	dic.keys = NULL;
	dic.values = NULL;

	dic.count = 0;
}

/* Free reserved memory for a dictionary struct */
void
Dictionary::freeDictionary(void)
{
	// Free each key and value in the dictionary
	for (int i = 0; i < dic.count; i++){
		free(dic.keys[i]);
		free(dic.values[i]);
	}

	// Free the dictionary struct
	free(dic.keys);
	free(dic.values);

	dic.keys = NULL;
	dic.values = NULL;
}

/* Append a peer key/value to a dictionary. */
bool
Dictionary::setValue(const char *key, const char *value)
{
	int i = 0;
	bool found = false;

	// Searching for the given key
	while ((i < dic.count) && (!found))
		// If key found stop searching
		if (!strcmp(dic.keys[i++], key))
			found = true;

	// If key not present in dictionary . append it
	if (!found){
		// Append a new key/value entry to the dictionary
		dic.keys = (char**) realloc(dic.keys, sizeof(char*)*(dic.count+1));

		if (dic.keys == NULL)
			return false;

		dic.values = (char**) realloc(dic.values, sizeof(char*)*(dic.count+1));

		if (dic.values == NULL)
			return false;

		// Calculate 'key' string size
		i = 0;
		while (key[i] != '\0') ++i;

		if (i == 0) return false;

		// Append a field to insert the new key/value
		dic.keys[dic.count] = (char*) malloc(sizeof(char)*(i + 1));

		if (dic.keys[dic.count] == NULL)
			return false;

		// Calculate 'value' string size
		i = 0;
		while (value[i] != '\0') ++i;

		if (i == 0) return false;

		dic.values[dic.count] = (char*) malloc(sizeof(char)*(i+1));

		if (dic.values[dic.count] == NULL)
			return false;

		// Append key/value to the new entrys in the dictionary
		strcpy(dic.keys[dic.count], key);
		strcpy(dic.values[dic.count], value);

		dic.count = dic.count+1;

		return true;
	}
	else {
		// If key present in dictionary . reserve memory & update value
		free(dic.values[i-1]);
		dic.values[i-1] = (char*) malloc(sizeof(char)*(strlen(value)+1));
		strcpy(dic.values[i-1], value);

		return true;
	}
}

/* Returns the value of the given key */
char *
Dictionary::getValue(const char *key)
{
	int i = 0;
	bool found = false;

	// Searhing for the key int the dictionary
	while ((i < dic.count) && (!found))
		// If key found stop searching
		if (!strcmp(dic.keys[i++], key))
			found = true;

	// If the key exists, return its asociated value
	if (found)
		return dic.values[i-1];
	else
		return NULL;
}

/* Gives the value of the given key, return the operation state */
bool
Dictionary::getValueString(char *key,char **value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	*value = val;
	return true;
}

/* Gives the value (int) of the given key, return the operation state */
bool
Dictionary::getValueInt(const char *key,int *value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	*value = atoi(val);
	return true;
}

/* Gives the value (unsigned int) of the given key, return the operation state */
bool
Dictionary::getValueUnsignedInt(const char *key,unsigned int *value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	*value = atoi(val);
	return true;
}

/* Gives the value (long) of the given key, return the operation state */
bool
Dictionary::getValueLong(char *key,long *value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	*value = atol(val);
	return true;
}

/* Gives the value (float) of the given key, return the operation state */
bool
Dictionary::getValueDouble(char *key,double *value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	*value = atof(val);
	return true;
}

/* Gives the value (float) of the given key, return the operation state */
bool
Dictionary::getValueFloat(const char *key,float *value)
{
	char *val = getValue(key);
	if (val == NULL) return false;
	//*value = atof(val);
	std::istringstream b(val);
	b >> *value;
	return true;
}

/* Checks if the given key is in the dictionary */
bool
Dictionary::isValue(char *key)
{
	char *value = getValue(key);
	if (value == NULL) return false;
	return true;
}

/* Prints all the keys of the dictionary */
void
Dictionary::printKeys(void)
{
	for (int i = 0; i < dic.count; i++)
		printf("%s ", dic.keys[i]);
}


/* Prints all the values of the dictionary */
void
Dictionary::printValues(void)
{
	for (int i = 0; i < dic.count; i++)
		printf("%s ", dic.values[i]);
}

void
Dictionary::printDictionary(void)
{
	for (int i = 0; i < dic.count; i++)
		printf("%s = %s\n", dic.keys[i], dic.values[i]);
}

/* Creates a dictionary from a given propertie file (key = value) */
// TODO: this method is memory-eater. Does not release memory!!!!!
bool
Dictionary::loadFromFile(const char *fileName)
{
	FILE *file;
	char *key,*value,*aux, *line;

	key = (char *)malloc(sizeof(char)*64);
	value = (char *)malloc(sizeof(char)*1024);
	aux = (char *)malloc(sizeof(char)*1024);
	line = (char *)malloc(sizeof(char)*1088);

	if ((file = fopen(fileName, "r")) == NULL)
		return false;

	// Read (from file) and insert (into the dictionary) each peer key/value
	while (!feof(file))
	{
		//First read a whole line
		fgets(line, 1088, file);
//		printf("input: line=<%s> ... ",line);

		//Scan the line
		key = strtok(line, "=");				//Everything before '='
		value = strtok(NULL, "\0\n");			//Everything after '='
		key = strtok(key, "\t ");				//Eliminate spaces betwen KEY and '='

		if (value != NULL)
		{
			int extra = strspn(value,"\t ");//Extra = number of spaces at the begining of the string
			strcpy(value,value+extra); //Eliminate spaces betwen "=" and VALUE
			value = strtok(value,"\n");//Eliminate end of line

			//Eliminate spaces at the end of the string

		  	//Reverse string
		  	strcpy(aux,value);
		  	for(int i=0;i<(int)strlen(value);i++)
				aux[i] = value[strlen(value)-1-i];

			//Eliminate starting spaces
			extra = strspn(aux,"\t ");
			strcpy(aux,aux+extra);

			//reverse again
		  	strcpy(value,aux);
			for(int i=0;i<(int)strlen(aux);i++)
				value[i] = aux[strlen(aux)-1-i];

//			printf("parsed: key=<%s> value=<%s> extra=%d",key,value,extra);
		}
//		printf("\n");

		//Check for valid assignments
		if ((key == NULL) || (value == NULL))		continue;

		//Ignore comment lines
		if ((key[0] == '#') || ((key[0] == '/') && (key[1] == '/')))		continue;

		setValue(key, value);
	}
	fclose(file);

	return true;
}

/* Stores a dictionary in a file */
bool
Dictionary::saveToFile(const char *fileName)
{
	FILE *file;

	if ((file = fopen(fileName, "w")) == NULL)
		return false;

	// Store each peer key/value
	for (int i = 0; i < dic.count; i++)
		fprintf(file, "%s = %s\n", dic.keys[i], dic.values[i]);

	fclose(file);

	return true;
}
