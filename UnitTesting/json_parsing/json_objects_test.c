
#include "json_objects.h"

int main(){

	// build objects with sprintf and templates in json_object.h
    char built[512];
    sprintf(built, simp_obj_builder, 4.88, true ? "true" : "false", 41);
    printf ("%s\n", built);


    // read objects with json_read_object and objects defined in json_object.h
    int status = json_read_object(built, simp_obj, NULL);
    printf("status = %d, count = %d, data = %f, boolflag = %d\n",
	   status, count, data, boolflag);
    if (status != 0)
	printf("%s\n", json_error_string(status));


	return 0;
}