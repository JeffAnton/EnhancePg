#
# Example from Enhancing Postgres by Jeff Anton (Perforce Open Logic)
#
# Postgresql datatype example Makefile based on src/tutorial
#
# Have the Postgresql source tree on your system and the pg_config
# executable in your path
# Usually, packages from Linux vendors do not include all the headers
# you may need to build C functions for Postgres
#
# Note also that your Postgres source tree should be the same major
# version as the Postgres version you are running.
#

MODULES = simple chapter
DATA_built = simple.sql chapter.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

%.sql: %.source
	rm -f $@; \
	C=`pwd`; \
	sed -e "s:_OBJWD_:$$C:g" < $< > $@
