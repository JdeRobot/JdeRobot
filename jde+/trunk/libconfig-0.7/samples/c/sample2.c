


/*************************************************************************
 ** Sample2
 ** Load test.cfg and access the "values" array
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
	if (!config_load_file(&cfg, "test.cfg"))printf("failed\n");
	else {
		printf("ok\n");

		/* Display the "values" array */
		printf("display \"values\"..");
		config_setting_t *array = config_lookup(&cfg, "values");
		if (!array) printf("failed\n");
		else {
			long value1,value2;
			value1 = config_setting_get_int_elem(array, 0);
			value2 = config_setting_get_int_elem(array, 1);
			printf("[%lu %lu]..ok\n", value1, value2);

			printf("Done!\n");
		}

	}

	/* Free the configuration */
	config_destroy(&cfg);

	return 0;
}


/***************************************************************************/
