COMPILER = gcc

PIDGIN_GTK_CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pidgin -I/usr/include/libpurple

SOURCES = \
	pidgin-redmine.c

all:	pidgin-redmine.so

pidgin-redmine.so: ${SOURCES}
	${COMPILER} ${PIDGIN_GTK_CFLAGS} -DPURPLE_PLUGINS -Wall -I. -g -O2 -pipe ${SOURCES} -o pidgin-redmine.so -shared -fPIC -DPIC

clean:
	rm pidgin-redmine.so

install:
	cp pidgin-redmine.so ~/.purple/plugins
