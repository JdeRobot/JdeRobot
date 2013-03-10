#include "int2string.h"

string int2string(int n){
	std:: stringstream flujo;

	flujo << n;

	return (flujo.str());
}
