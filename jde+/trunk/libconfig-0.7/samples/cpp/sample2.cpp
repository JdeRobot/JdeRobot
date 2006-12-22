


/*************************************************************************
 ** Sample2
 ** Load test.cfg and access the "values" array
 *************************************************************************/

#include <iostream>
#include <libconfig.h++>

using namespace libconfig;
using namespace std;

/***************************************************************************/

int main()
{
	Config cfg;
	try {
		/* Load the configuration.. */
		cout << "loading [test.cfg]..";
		cfg.loadFile("test.cfg");
		cout << "ok" << endl;

		// Display the "values" array
		cout << "display the \"values\" array..";
		Setting s = cfg.lookup("values");
		long value1 = s[0];
		long value2 = s[1];
		cout << "[" << value1 << "," << value2 << "]..";
		cout << "ok" << endl;

		cout << "Done!" << endl;
	} catch (...) {
		cout << "failed" << endl;
	}

	return 0;
}


/***************************************************************************/
