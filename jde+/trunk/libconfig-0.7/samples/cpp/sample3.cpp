


/*************************************************************************
 ** Sample3
 ** Load test.cfg and try to add a setting "foo"..
 **   on success save to testfoo.cfg
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

		/* Add setting "foo" */
		cout << "add setting \"foo\"..";
		Setting &root = cfg.getRoot();
		Setting &foo  = root.add("foo",TypeInt);
		foo = 1234;
		cout << "ok" << endl;

		/* Save to "testfoo.cfg" */
		cout << "saving [testfoo.cfg]..";
		cfg.saveFile("testfoo.cfg");
		cout << "ok" << endl;

		cout << "Done!" << endl;
	} catch (...) {
		cout << "failed" << endl;
	}

	return 0;
}

/***************************************************************************/
