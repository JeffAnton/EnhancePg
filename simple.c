/*
 * Example from Enhancing Postgres by Jeff Anton (Perforce Open Logic)
 */

#include "postgres.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(simple);

Datum
simple(PG_FUNCTION_ARGS)
{
    const char *out = "Simple function"; 
    PG_RETURN_TEXT_P(cstring_to_text(out));
}
