#include <stdio.h>
#include "dson.h"

void dson_indent(int indent) {
	while (indent--) {
		printf("  ");
	}
}

void dson_dump(dson_value* value, int indent) {
	dson_indent(indent);
	switch (value->type) {
		case DSON_NULL:
			printf("NULL\n");
			break;
		case DSON_BOOL:
			printf("bool(%s)\n", value->bval ? "true" : "false");
			break;
		case DSON_LONG:
			printf("long(%ld)\n", value->lval);
			break;
		case DSON_DOUBLE:
			printf("double(%f)\n", value->dval);
			break;
		case DSON_STRING:
			printf("string(%d) \"%s\"\n", value->len, value->strval);
			break;
		case DSON_ARRAY:
		{
			int i;
			printf("Array (%d) [\n", value->len);
			for (i = 0; i < value->len; ++i) {
				dson_dump(value->arrval[i], indent + 1);
			}
			dson_indent(indent);
			printf("]\n");
			break;
		}
		case DSON_OBJECT:
		{
			int i;
			printf("Object (%d) {\n", value->len);
			for (i = 0; i < value->len; ++i) {
				dson_indent(indent + 1);
				printf("%s:\n", value->objval[i]->key);
				dson_dump(value->objval[i]->value, indent + 2);
			}
			dson_indent(indent);
			printf("}\n");
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	int i;

	if (argc <= 1) {
		fprintf(stderr, "Usage: %s [DSON string]\n\n", argv[0]);
		fprintf(stderr, "\t%s 'such \"doge\" is \"good\" wow'\n", argv[0]);
		return 1;
	}
	dson_value *value = dson_parse_string(argv[1]);

	if (!value) {
		fprintf(stderr, "Unable to parse '%s'\n", argv[1]);
		return 1;
	}
	dson_dump(value, 0);
	dson_free_value(value);

	return 0;
}
