#include "lexer.h"

#include <stdlib.h>
#include <string.h>

#include "xmalloc.h"

void
lexer_init (lexer_t* self, const char* source) {
  scanner_init(&self->scanner, source);
}

array_t*
lexer_tokenize (lexer_t* self) {
  array_t* tokens = array_init();

  while (true) {
    char c = scanner_next(&self->scanner);

    if (c == '\0') {
      break;
    }

    switch (c) {
      case ' ': {
        token_t* token = xmalloc(sizeof(token_t));
        token->type    = TOKEN_SPACE;
        token->value   = s_copy(" ");
        array_push(tokens, token);
        break;
      };

      default: {
        token_t*  token = xmalloc(sizeof(token_t));
        buffer_t* buf   = buffer_init(NULL);

        bool has_bang   = false;
        while (c != '\0' && c != ' ') {
          buffer_append_char(buf, c);
          c = scanner_next(&self->scanner);
        }

        if (buffer_size(buf)) {
          token->type  = TOKEN_STRING;
          token->value = s_copy(buffer_state(buf));
          array_push(tokens, token);
        } else {
          free(token);
        }

        buffer_free(buf);

        if (c == ' ') {
          token_t* s_token = xmalloc(sizeof(token_t));
          s_token->type    = TOKEN_SPACE;
          s_token->value   = s_copy(" ");
          array_push(tokens, s_token);
        }
        break;
      }
    }
  }

  return tokens;
}
