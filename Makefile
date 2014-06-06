all: test

dson_tokenizer.c:
	re2c -o dson_tokenizer.c dson_tokenizer.re

dson_parser.c:
	lemon -q dson_parser.lemon

test: dson_tokenizer.c dson_parser.c dson.c test.c
	gcc -o test dson_tokenizer.c dson_parser.c test.c dson.c -lm

clean:
	rm -f dson_tokenizer.c dson_parser.c test
