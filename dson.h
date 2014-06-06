#ifndef LIBDSON_H
#define LIBDSON_H

#define DSON_NULL	0
#define DSON_BOOL	1
#define DSON_LONG	2
#define DSON_DOUBLE	3
#define DSON_STRING	4
#define DSON_ARRAY	5
#define DSON_OBJECT	6

typedef struct _dson_value dson_value;
typedef struct _dson_pair   dson_pair;

struct _dson_pair {
	char *key;
	dson_value *value;
};

struct _dson_value {
	unsigned char type;
	union {
		char bval;
		long lval;
		double dval;
		char *strval;
		dson_value **arrval;
		dson_pair **objval;
	};
	int len;
};

#ifdef __cplusplus
extern "C" {
#endif
dson_value* dson_parse_string(const char *str);
void dson_free_value(dson_value *value);
#ifdef __cplusplus
}
#endif

#endif /* LIBDSON_H */
