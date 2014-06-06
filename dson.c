#include "dson.h"
#include "dson_tokenizer.h"
#include "dson_parser.h"
#include <stdlib.h>
#include <string.h>

void *dson_parserAlloc(void* (*)(size_t));
void dson_parser(void*, int, dson_token , dson_value**);
void dson_parserFree(void*, void (*)(void*));

dson_value* dson_parse_string(const char *str) {
	dson_tokenizer t;
	dson_token token;
	dson_value *value = NULL;

	void *p = dson_parserAlloc(malloc);
	t.start = (unsigned char*)str;
	t.end = t.start + strlen(str);

	while (DSON_END != dson_get_token(&t, &token)) {
		if (DSON_WHITESPACE == token.id) continue;
		dson_parser(p, token.id, token, NULL);
	}
	dson_parser(p, 0, token, &value);
	dson_parserFree(p, free);
	return value;
}


void dson_free_value(dson_value *value) {
	int i;
	switch (value->type) {
		case DSON_STRING:
			free(value->strval);
			break;
		case DSON_ARRAY:
			if (value->len) {
				for (i = 0; i < value->len; ++i) {
					dson_free_value(value->arrval[i]);
				}
				free(value->arrval);
			}
			break;
		case DSON_OBJECT:
			for (i = 0; i < value->len; ++i) {
				free(value->objval[i]->key);
				dson_free_value(value->objval[i]->value);
			}
			break;
	}
	free(value);
}
