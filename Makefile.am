########################################################################
# Makefile for lv
########################################################################

SUBDIRS = src

EXTRA_DIST = .editorconfig .gitignore GPL.txt hello.sample hello.sample.gif \
	     index.html lv.1 lv.hlp README.md relnote.html build/README.txt

EXTRA_DIST += src/Makefile.dos src/Makefile.mingw src/Makefile.msvc

EXTRA_DIST += \
	src/big5.map \
	src/big5.rev \
	src/big5cns.map \
	src/big5cns.pl \
	src/d2u.pl \
	src/d2uall.sh \
	src/gb2312.map \
	src/gb2312.rev \
	src/iso885910.map \
	src/iso885910.rev \
	src/iso885911.map \
	src/iso885911.rev \
	src/iso885913.map \
	src/iso885913.rev \
	src/iso885914.map \
	src/iso885914.rev \
	src/iso885915.map \
	src/iso885915.rev \
	src/iso885916.map \
	src/iso885916.rev \
	src/iso88592.map \
	src/iso88592.rev \
	src/iso88593.map \
	src/iso88593.rev \
	src/iso88594.map \
	src/iso88594.rev \
	src/iso88595.map \
	src/iso88595.rev \
	src/iso88596.map \
	src/iso88596.rev \
	src/iso88597.map \
	src/iso88597.rev \
	src/iso88598.map \
	src/iso88598.rev \
	src/iso88599.map \
	src/iso88599.rev \
	src/jis.map \
	src/jis0208.rev \
	src/jis0212.rev \
	src/ksc5601.map \
	src/ksc5601.rev \
	src/map8859.pl \
	src/mapbig5.pl \
	src/mapgb.pl \
	src/mapjis.pl \
	src/mapksc.pl \
	src/rev0208.pl \
	src/rev0212.pl \
	src/rev8859.pl \
	src/revbig5.pl \
	src/revgb.pl \
	src/revksc.pl

$(srcdir)/lv.hlp: $(srcdir)/lv.1
	nroff -c -mandoc $< > $@
