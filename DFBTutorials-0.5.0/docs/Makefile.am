## Makefile.am for DFBTutorials/docs

docs_tutorials = \
	$(top_srcdir)/src/simple/simple.c	\
	$(top_srcdir)/src/image/image.c		\
	$(top_srcdir)/src/text/text.c		\
	$(top_srcdir)/src/sprite/sprite.c	\
	$(top_srcdir)/src/keybuffer/keybuffer.c

EXTRA_DIST = \
	html/image.html		\
	html/index.html		\
	html/keybuffer.html	\
	html/simple.html	\
	html/sprite.html	\
	html/text.html		\
	html/cimlogo.png	\
	html/directfb.png

all-local: stamp-docs

stamp-docs: $(top_srcdir)/tools/gentut.pl $(docs_tutorials) Makefile.am
	rootme=`pwd`; \
	cd $(srcdir) && cat $(docs_tutorials) | $(PERL) $(top_srcdir)/tools/gentut.pl \
	&& echo timestamp > $$rootme/stamp-docs

distclean-local:
	rm -f html/*.html stamp-docs
