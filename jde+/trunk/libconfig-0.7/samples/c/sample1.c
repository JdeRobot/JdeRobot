

/*************************************************************************
 ** Sample1
 ** Load test.cfg and increment the "X" setting
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

		/* Get the "x" setting from the configuration.. */
		printf("increment \"x\"..");
		config_setting_t *setting = config_lookup(&cfg, "x");
		if (!setting) printf("failed\n");
		else {
			long x = config_setting_get_int(setting);
			x++;
			config_setting_set_int(setting, x);
			printf("ok (x=%lu)\n", x);

			/* Save the changes */
			printf("saving [test.cfg]..");
			config_save_file(&cfg, "test.cfg");
			printf("ok\n");

			printf("Done!\n");
		}
	}

	/* Free the configuration */
	config_destroy(&cfg);

    return 0;
}

/***************************************************************************/
