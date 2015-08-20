########################################################################
# Makefile for lv
########################################################################

lvlibdir=	$(libdir)/lv
CFLAGS=	@CFLAGS@ -DLV_HELP_PATH=\"$(lvlibdir)\" -I$(srcdir) @DEFS@
if MINGW
CFLAGS += -DUSE_UNICODE_IO
endif

bin_PROGRAMS = lv

lv_SOURCES = ascii.h attr.h begin.h big5.h boolean.h command.h conf.h \
	console.h conv.h ctable.h ctable_e.h ctable_t.h decode.h \
	dfa.h display.h encode.h escape.h eucjapan.h fetch.h \
	file.h find.h guess.h guesslocale.h hz.h ichar.h import.h \
	iso2022.h iso2cn.h iso2jp.h iso2kr.h iso8859.h istr.h \
	itable.h itable_e.h itable_t.h kana.h keybind.h nfa.h \
	position.h raw.h re.h screen.h shiftjis.h str.h stream.h \
	unimap.h unirev.h utf.h uty.h version.h

lv_SOURCES += itable.c ctable.c uty.c istr.c stream.c file.c guess.c decode.c \
	encode.c escape.c iso2022.c iso8859.c iso2cn.c iso2jp.c iso2kr.c \
	kana.c eucjapan.c shiftjis.c big5.c hz.c raw.c \
	fetch.c screen.c command.c display.c find.c re.c \
	nfa.c dfa.c conv.c version.c conf.c lv.c console.c guesslocale.c
# Unicode sources
lv_SOURCES += utf.c unimap.c unirev.c

# MinGW specific sources
if MINGW
lv_SOURCES += iscygpty.h iscygpty.c
endif




#------------------------------------------------------------
# the following rules are just for maintainer

#
# big5-cns map files
#

big5cns.map: cjk.inf.gz
	$(PERL) $(srcdir)/big5cns.pl > $@

CJKINFURL = ftp://ftp.ora.com/pub/examples/nutshell/cjkv/doc/

cjk.inf.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(CJKINFURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi

#
# map files
#

map: big5.map gb2312.map jis.map ksc5601.map iso88592.map iso88593.map iso88594.map iso88595.map iso88596.map iso88597.map iso88598.map iso88599.map iso885910.map iso885911.map iso885913.map iso885914.map iso885915.map iso885916.map

big5.map: BIG5.TXT.gz
	$(PERL) $(srcdir)/mapbig5.pl > $@

gb2312.map: GB2312.TXT.gz
	$(PERL) $(srcdir)/mapgb.pl > $@

jis.map: JIS0208.TXT.gz JIS0212.TXT.gz
	$(PERL) $(srcdir)/mapjis.pl > $@

ksc5601.map: KSX1001.TXT.gz
	$(PERL) $(srcdir)/mapksc.pl > $@

iso88592.map: 8859-2.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 2 -i 'zcat 8859-2.TXT.gz |' > $@

iso88593.map: 8859-3.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 3 -i 'zcat 8859-3.TXT.gz |' > $@

iso88594.map: 8859-4.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 4 -i 'zcat 8859-4.TXT.gz |' > $@

iso88595.map: 8859-5.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 5 -i 'zcat 8859-5.TXT.gz |' > $@

iso88596.map: 8859-6.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 6 -i 'zcat 8859-6.TXT.gz |' > $@

iso88597.map: 8859-7.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 7 -i 'zcat 8859-7.TXT.gz |' > $@

iso88598.map: 8859-8.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 8 -i 'zcat 8859-8.TXT.gz |' > $@

iso88599.map: 8859-9.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 9 -i 'zcat 8859-9.TXT.gz |' > $@

iso885910.map: 8859-10.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 10 -i 'zcat 8859-10.TXT.gz |' > $@

iso885911.map: 8859-11.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 11 -i 'zcat 8859-11.TXT.gz |' > $@

iso885913.map: 8859-13.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 13 -i 'zcat 8859-13.TXT.gz |' > $@

iso885914.map: 8859-14.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 14 -i 'zcat 8859-14.TXT.gz |' > $@

iso885915.map: 8859-15.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 15 -i 'zcat 8859-15.TXT.gz |' > $@

iso885916.map: 8859-16.TXT.gz
	$(PERL) $(srcdir)/map8859.pl -s 16 -i 'zcat 8859-16.TXT.gz |' > $@

#
# rev files
#

rev: big5.rev gb2312.rev jis0208.rev jis0212.rev ksc5601.rev iso88592.rev iso88593.rev iso88594.rev iso88595.rev iso88596.rev iso88597.rev iso88598.rev iso88599.rev iso885910.rev iso885911.rev iso885913.rev iso885914.rev iso885915.rev iso885916.rev

big5.rev: BIG5.TXT.gz
	$(PERL) $(srcdir)/revbig5.pl > $@

gb2312.rev: GB2312.TXT.gz
	$(PERL) $(srcdir)/revgb.pl > $@

jis0208.rev: JIS0208.TXT.gz
	$(PERL) $(srcdir)/rev0208.pl > $@

jis0212.rev: JIS0212.TXT.gz
	$(PERL) $(srcdir)/rev0212.pl > $@

ksc5601.rev: KSX1001.TXT.gz
	$(PERL) $(srcdir)/revksc.pl > $@

iso88592.rev: 8859-2.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 2 -i 'zcat 8859-2.TXT.gz |' > $@

iso88593.rev: 8859-3.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 3 -i 'zcat 8859-3.TXT.gz |' > $@

iso88594.rev: 8859-4.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 4 -i 'zcat 8859-4.TXT.gz |' > $@

iso88595.rev: 8859-5.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 5 -i 'zcat 8859-5.TXT.gz |' > $@

iso88596.rev: 8859-6.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 6 -i 'zcat 8859-6.TXT.gz |' > $@

iso88597.rev: 8859-7.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 7 -i 'zcat 8859-7.TXT.gz |' > $@

iso88598.rev: 8859-8.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 8 -i 'zcat 8859-8.TXT.gz |' > $@

iso88599.rev: 8859-9.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 9 -i 'zcat 8859-9.TXT.gz |' > $@

iso885910.rev: 8859-10.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 10 -i 'zcat 8859-10.TXT.gz |' > $@

iso885911.rev: 8859-11.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 11 -i 'zcat 8859-11.TXT.gz |' > $@

iso885913.rev: 8859-13.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 13 -i 'zcat 8859-13.TXT.gz |' > $@

iso885914.rev: 8859-14.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 14 -i 'zcat 8859-14.TXT.gz |' > $@

iso885915.rev: 8859-15.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 15 -i 'zcat 8859-15.TXT.gz |' > $@

iso885916.rev: 8859-16.TXT.gz
	$(PERL) $(srcdir)/rev8859.pl -s 16 -i 'zcat 8859-16.TXT.gz |' > $@

#
# Unicode mapping tables
#

BIG5URL = ftp://ftp.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/OTHER

BIG5.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(BIG5URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi


GBURL = ftp://ftp.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/GB

GB2312.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(GBURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi


JISURL = ftp://ftp.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS

JIS0201.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(JISURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
JIS0208.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(JISURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
JIS0212.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(JISURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi


KSCURL = ftp://ftp.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/KSC

KSX1001.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(KSCURL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi


ISO8859URL = ftp://ftp.unicode.org/Public/MAPPINGS/ISO8859

8859-1.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-2.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-3.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-4.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-5.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-6.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-7.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-8.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-9.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-10.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-11.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-13.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-14.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-15.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi
8859-16.TXT.gz:
	if [ ! -f ${@:.gz=} ]; then wget -O ${@:.gz=} $(ISO8859URL)/${@:.gz=} ; fi ; \
	if [ -f ${@:.gz=} ]; then gzip ${@:.gz=} ; fi

