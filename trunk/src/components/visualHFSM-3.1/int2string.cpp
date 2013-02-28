#include <sstream>

using namespace std;

string int2string(int n){
	std:: stringstream flujo;

	flujo << n;

	return (flujo.str());
}
