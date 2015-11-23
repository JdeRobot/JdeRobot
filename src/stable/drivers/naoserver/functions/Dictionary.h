#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

class Dictionary {
private:
	struct dictionary{	// Dictionary struct to store properties in the manner 'key = value'
		char **keys;						// Key of the property
		char **values;					// Value of the property
		int count;							// Number of entries in the dictionary
	};

	struct dictionary dic;

public:
	void createDictionary(void);
	void freeDictionary(void);

	bool setValue(const char *key, const char *value);

	char *getValue(const char *key);
	bool getValueString(char *key,char **value);
	bool getValueInt(const char *key,int *value);
	bool getValueUnsignedInt(const char *key,unsigned int *value);
	bool getValueLong(char *key,long *value);
	bool getValueDouble(char *key,double *value);
	bool getValueFloat(const char *key,float *value);
	bool isValue(char *key);

	void printKeys(void);
	void printValues(void);
	void printDictionary(void);

	bool loadFromFile(const char *fileName);
	bool saveToFile(const char *fileName);
};

#endif

