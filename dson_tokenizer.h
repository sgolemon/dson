#ifndef LIBDSON_TOKENIZER_H
#define LIBDSON_TOKENIZER_H

#define DSON_WHITESPACE	0x101
#define DSON_END	0x102

typedef struct _dson_token {
        unsigned int id;
        unsigned char *token;
        int token_len;
        int freeme:1;
} dson_token;

typedef struct _dson_tokenizer {
        unsigned char *start, *end;
} dson_tokenizer;

int dson_get_token(dson_tokenizer *t, dson_token *token);

#endif
