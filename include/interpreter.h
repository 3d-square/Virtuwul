#ifndef _INTERPRETER_
#define _INTERPRETER_

#include <stdlib.h>
#include "token.h"

double absolute(double);

token *do_operation(TYPE, token *, token *);

int interpreter(token *, size_t);

#endif
