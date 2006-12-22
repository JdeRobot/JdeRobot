


/*************************************************************************
 ** Sample1
 ** Load test.cfg and increment the "X" setting
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

		/* Increment "x" */
		cout << "increment \"x\"..";
		Setting s = cfg.lookup("x");
		long x = s;
		s = ++x;
		cout << "ok (x=" << x << ")" << endl;

		// Save the configuration
		cout << "saving [test.cfg]..";
		cfg.saveFile("test.cfg");
		cout << "ok" << endl;

		cout << "Done!" << endl;
	} catch (...) {
		cout << "failed" << endl;
	}

	return 0;
}

/***************************************************************************/
