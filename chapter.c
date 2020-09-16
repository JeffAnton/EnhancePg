#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"

PG_MODULE_MAGIC;

typedef struct Chapter
{
	int32		f;
	int32		s;
} Chapter;


PG_FUNCTION_INFO_V1(chapter_in);

Datum
chapter_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	char	   *p;
	int32		f, s;
	Chapter    *result;

	p = NULL;
	f = strtol(str, &p, 10);
	s = 0;
	if (p && p > str) {
		if (*p == '.') {
			s = strtol(p+1, NULL, 10);
		} else if (*p != '\0') {
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					 errmsg("invalid input syntax for type %s: \"%s\"",
							"chapter", str)));
		}
	} else {
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"chapter", str)));
	}

	result = (Chapter *) palloc(sizeof(Chapter));
	result->f = f;
	result->s = s;
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(chapter_out);

Datum
chapter_out(PG_FUNCTION_ARGS)
{
	Chapter    *chapter = (Chapter *) PG_GETARG_POINTER(0);
	char	   *result;

	if (chapter->s > 0)
		result = psprintf("%d.%d", chapter->f, chapter->s);
	else
		result = psprintf("%d", chapter->f);
	PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(chapter_recv);

Datum
chapter_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Chapter    *result;

	result = (Chapter *) palloc(sizeof(Chapter));
	result->f = pq_getmsgint(buf, sizeof(int32));
	result->s = pq_getmsgint(buf, sizeof(int32));
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(chapter_send);

Datum
chapter_send(PG_FUNCTION_ARGS)
{
	Chapter    *chapter = (Chapter *) PG_GETARG_POINTER(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendint32(&buf, chapter->f);
	pq_sendint32(&buf, chapter->s);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}


/*****************************************************************************
 * Operator class for defining B-tree index
 *
 * It's essential that the comparison operators and support function for a
 * B-tree index opclass always agree on the relative ordering of any two
 * data values.  Experience has shown that it's depressingly easy to write
 * unintentionally inconsistent functions.  One way to reduce the odds of
 * making a mistake is to make all the functions simple wrappers around
 * an internal three-way-comparison function, as we do here.
 *****************************************************************************/

static int
chapter_abs_cmp_internal(Chapter * a, Chapter * b)
{
	if (a->f < b->f)
		return -1;
	if (a->f > b->f)
		return 1;
	if (a->s < b->s)
		return -1;
	if (a->s > b->s)
		return 1;
	return 0;
}


PG_FUNCTION_INFO_V1(chapter_abs_lt);

Datum
chapter_abs_lt(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(chapter_abs_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(chapter_abs_le);

Datum
chapter_abs_le(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(chapter_abs_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(chapter_abs_eq);

Datum
chapter_abs_eq(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(chapter_abs_cmp_internal(a, b) == 0);
}

PG_FUNCTION_INFO_V1(chapter_abs_ge);

Datum
chapter_abs_ge(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(chapter_abs_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(chapter_abs_gt);

Datum
chapter_abs_gt(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_BOOL(chapter_abs_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(chapter_abs_cmp);

Datum
chapter_abs_cmp(PG_FUNCTION_ARGS)
{
	Chapter    *a = (Chapter *) PG_GETARG_POINTER(0);
	Chapter    *b = (Chapter *) PG_GETARG_POINTER(1);

	PG_RETURN_INT32(chapter_abs_cmp_internal(a, b));
}
