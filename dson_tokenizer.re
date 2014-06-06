#include <stdlib.h>
#include "dson_tokenizer.h"
#include "dson_parser.h"

static void dson_unquote(dson_token *token)
{
	/* Unescaping is guaranteed to consume
	 * more characters than it produces */
	unsigned char *out = (unsigned char*)malloc(token->token_len + 1),
	              *p = out;
	int i;
	if (!out) {
		token->token = (unsigned char*)"memory allocation error";
		token->token_len = 0;
		token->freeme = 0;
		return;
	}

	token->token_len--;
	for(i = 1; i < token->token_len; i++) {
		char c = token->token[i];
		if (token->token[i] != '\\') {
			*(p++) = c;
			continue;
		}
		if ((i + 1) >= token->token_len) {
			/* EOL */
			*(p++) = '\\';
			goto done;
		}
		switch (token->token[++i]) {
			case 'r':	*(p++) = '\r';		break;
			case 'n':	*(p++) = '\n';		break;
			case 't':	*(p++) = '\t';		break;
			case 'b':	*(p++) = '\b';		break;
			case '/':
			case '"':
			case '\\':	*(p++) = '\\';		break;
			case 'u': // \u012345
			{
				unsigned long val = 0;
				int j = i + 6;

				do {
					++i;
					if ((i >= token->token_len) ||
					    (token->token[i] < '0') ||
					    (token->token[i] > '7')) {
						*(p++) = 'u';
						goto done;
					}
					val = (val << 3) | (token->token[i] - '0');
				} while (i < j);

				if (val < 0x7F) {
					*(p++) = val;
				} else if (val < 0x7FF) {
					*(p++) = 0xC0 |  (val >>  6);
					*(p++) = 0x80 | ( val        & 0x3F);
				} else if (val < 0xFFFF) {
					*(p++) = 0xE0 |  (val >> 12);
					*(p++) = 0x80 | ((val >>  6) & 0x3F);
					*(p++) = 0x80 | ( val        & 0x3F);
				} else if (val < 0x10FFFF) {
					*(p++) = 0xF0 |  (val >> 18);
					*(p++) = 0x80 | ((val >> 12) & 0x3F);
					*(p++) = 0x80 | ((val >>  6) & 0x3F);
					*(p++) = 0x80 | ( val        & 0x3F);
				} else {
					/* Error, no unicode above 0x10FFFF */
					*(p++) = 'u';
					goto done;
				}
				break;
			}
			default:
				*(p++) = token->token[i];
		}
	}

done:
	token->token = out;
	token->token_len = p - out;
	*p = 0;

	token->freeme = 1;
}

#define RET(num)       { token->token_len = t->start - token->token; \
                         token->id = DSON_##num; \
                         return (DSON_##num); }
#define RET_UNESC(num) { token->token_len = t->start - token->token; \
                         token->id = DSON_##num; dson_unquote(token); \
                         return (DSON_##num); }

int dson_get_token(dson_tokenizer *t, dson_token *token)
{
	unsigned char *marker = t->start;
	token->freeme = 0;
	token->token = t->start;

/*!re2c
	re2c:define:YYCTYPE  = "unsigned char";
	re2c:define:YYCURSOR = (t->start);
	re2c:define:YYLIMIT  = (t->end);
	re2c:define:YYMARKER = marker;
	re2c:yyfill:enable   = 0;

	'such'		{ RET(SUCH);  }
	'is'		{ RET(IS);    }
	'wow'		{ RET(WOW);   }

	'so'		{ RET(SO);    }
	('and'|'also')	{ RET(ALSO);   }
	'many'		{ RET(MANY);  }

	'very'		{ RET(VERY);  }

	'yes'		{ RET(YES);   }
	'no'		{ RET(NO);    }
	'empty'		{ RET(EMPTY); }

	EOF		= [\000];
	ONUM		= [0-7];
	ONUM_INIT	= [1-7];
	ESCQ		= [\\]["];
	ESCSEQ		= [\\].;
	ANYNOEOF	= [\001-\377];

	(["] (ESCQ|ESCSEQ|ANYNOEOF\[\\"])* ["])		{ RET_UNESC(STR); }
	([-]? ([0]|ONUM_INIT ONUM*) [.] ONUM+)		{ RET(DNUM); }
	([-]? ([0]|ONUM_INIT ONUM*)          )		{ RET(LNUM); }
	[,\.\!\?]					{ RET(COMMA); }
	[\r\n\t ]+					{ RET(WHITESPACE); }
	EOF						{ RET(END); }
*/
}

