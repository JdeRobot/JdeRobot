

/*************************************************************************
 ** Sample3
 ** Load test.cfg and try to add a setting "foo"..
 **   on success save to testfoo.cfg
 *************************************************************************/

#include <stdio.h>
#include <libconfig.h>

struct config_t cfg;

/***************************************************************************/

int main()
{
	/* Initialize the configuration */
	config_init(&cfg);

	/* Load the file */
	printf("loading [test.cfg]..");
	if (!config_load_file(&cfg, "test.cfg")) printf("failed\n");
	else {
		printf("ok\n");

		/* Add setting "foo" */
		printf( "add setting \"foo\".." );
		config_setting_t *setting = config_setting_add(cfg.root,"foo",CONFIG_TYPE_INT);
		if (!setting) printf("failed\n");
		else {
			config_setting_set_int(setting,1234);
			printf("ok\n");

			/* Save to "testfoo.cfg" */
			printf("saving [testfoo.cfg]..");
			config_save_file(&cfg, "testfoo.cfg");
			printf("ok\n");

			printf("Done!\n");
		}
	}

	/* Free the configuration */
	config_destroy(&cfg);

	return 0;
}

/***************************************************************************/
