<<<<<<< HEAD
<<<<<<< HEAD
=======
OUTPUT := ./
ifeq ("$(origin O)", "command line")
  ifneq ($(O),)
	OUTPUT := $(O)/
  endif
endif

>>>>>>> refs/remotes/origin/cm-10.0
=======
include ../../scripts/Makefile.include
include ../config/utilities.mak

>>>>>>> refs/remotes/origin/master
MAN1_TXT= \
	$(filter-out $(addsuffix .txt, $(ARTICLES) $(SP_ARTICLES)), \
		$(wildcard perf-*.txt)) \
	perf.txt
MAN5_TXT=
MAN7_TXT=

MAN_TXT = $(MAN1_TXT) $(MAN5_TXT) $(MAN7_TXT)
<<<<<<< HEAD
<<<<<<< HEAD
MAN_XML=$(patsubst %.txt,%.xml,$(MAN_TXT))
MAN_HTML=$(patsubst %.txt,%.html,$(MAN_TXT))

DOC_HTML=$(MAN_HTML)
=======
=======
>>>>>>> refs/remotes/origin/master
_MAN_XML=$(patsubst %.txt,%.xml,$(MAN_TXT))
_MAN_HTML=$(patsubst %.txt,%.html,$(MAN_TXT))

MAN_XML=$(addprefix $(OUTPUT),$(_MAN_XML))
MAN_HTML=$(addprefix $(OUTPUT),$(_MAN_HTML))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

ARTICLES =
# with their own formatting rules.
SP_ARTICLES =
API_DOCS = $(patsubst %.txt,%,$(filter-out technical/api-index-skel.txt technical/api-index.txt, $(wildcard technical/api-*.txt)))
SP_ARTICLES += $(API_DOCS)
SP_ARTICLES += technical/api-index

<<<<<<< HEAD
<<<<<<< HEAD
DOC_HTML += $(patsubst %,%.html,$(ARTICLES) $(SP_ARTICLES))

DOC_MAN1=$(patsubst %.txt,%.1,$(MAN1_TXT))
DOC_MAN5=$(patsubst %.txt,%.5,$(MAN5_TXT))
DOC_MAN7=$(patsubst %.txt,%.7,$(MAN7_TXT))
=======
=======
>>>>>>> refs/remotes/origin/master
_DOC_HTML = $(_MAN_HTML)
_DOC_HTML+=$(patsubst %,%.html,$(ARTICLES) $(SP_ARTICLES))
DOC_HTML=$(addprefix $(OUTPUT),$(_DOC_HTML))

_DOC_MAN1=$(patsubst %.txt,%.1,$(MAN1_TXT))
_DOC_MAN5=$(patsubst %.txt,%.5,$(MAN5_TXT))
_DOC_MAN7=$(patsubst %.txt,%.7,$(MAN7_TXT))

DOC_MAN1=$(addprefix $(OUTPUT),$(_DOC_MAN1))
DOC_MAN5=$(addprefix $(OUTPUT),$(_DOC_MAN5))
DOC_MAN7=$(addprefix $(OUTPUT),$(_DOC_MAN7))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

# Make the path relative to DESTDIR, not prefix
ifndef DESTDIR
prefix?=$(HOME)
endif
bindir?=$(prefix)/bin
htmldir?=$(prefix)/share/doc/perf-doc
pdfdir?=$(prefix)/share/doc/perf-doc
mandir?=$(prefix)/share/man
man1dir=$(mandir)/man1
man5dir=$(mandir)/man5
man7dir=$(mandir)/man7

ASCIIDOC=asciidoc
ASCIIDOC_EXTRA = --unsafe
MANPAGE_XSL = manpage-normal.xsl
XMLTO_EXTRA =
INSTALL?=install
RM ?= rm -f
DOC_REF = origin/man
HTML_REF = origin/html

infodir?=$(prefix)/share/info
MAKEINFO=makeinfo
INSTALL_INFO=install-info
DOCBOOK2X_TEXI=docbook2x-texi
DBLATEX=dblatex
<<<<<<< HEAD
=======
XMLTO=xmlto
>>>>>>> refs/remotes/origin/master
ifndef PERL_PATH
	PERL_PATH = /usr/bin/perl
endif

-include ../config.mak.autogen
-include ../config.mak

<<<<<<< HEAD
=======
_tmp_tool_path := $(call get-executable,$(ASCIIDOC))
ifeq ($(_tmp_tool_path),)
	missing_tools = $(ASCIIDOC)
endif

_tmp_tool_path := $(call get-executable,$(XMLTO))
ifeq ($(_tmp_tool_path),)
	missing_tools += $(XMLTO)
endif

>>>>>>> refs/remotes/origin/master
#
# For asciidoc ...
#	-7.1.2,	no extra settings are needed.
#	8.0-,	set ASCIIDOC8.
#

#
# For docbook-xsl ...
#	-1.68.1,	set ASCIIDOC_NO_ROFF? (based on changelog from 1.73.0)
#	1.69.0,		no extra settings are needed?
#	1.69.1-1.71.0,	set DOCBOOK_SUPPRESS_SP?
#	1.71.1,		no extra settings are needed?
#	1.72.0,		set DOCBOOK_XSL_172.
#	1.73.0-,	set ASCIIDOC_NO_ROFF
#

#
# If you had been using DOCBOOK_XSL_172 in an attempt to get rid
# of 'the ".ft C" problem' in your generated manpages, and you
# instead ended up with weird characters around callouts, try
# using ASCIIDOC_NO_ROFF instead (it works fine with ASCIIDOC8).
#

ifdef ASCIIDOC8
ASCIIDOC_EXTRA += -a asciidoc7compatible
endif
ifdef DOCBOOK_XSL_172
ASCIIDOC_EXTRA += -a perf-asciidoc-no-roff
MANPAGE_XSL = manpage-1.72.xsl
else
	ifdef ASCIIDOC_NO_ROFF
	# docbook-xsl after 1.72 needs the regular XSL, but will not
	# pass-thru raw roff codes from asciidoc.conf, so turn them off.
	ASCIIDOC_EXTRA += -a perf-asciidoc-no-roff
	endif
endif
ifdef MAN_BOLD_LITERAL
XMLTO_EXTRA += -m manpage-bold-literal.xsl
endif
ifdef DOCBOOK_SUPPRESS_SP
XMLTO_EXTRA += -m manpage-suppress-sp.xsl
endif

SHELL_PATH ?= $(SHELL)
# Shell quote;
SHELL_PATH_SQ = $(subst ','\'',$(SHELL_PATH))

#
# Please note that there is a minor bug in asciidoc.
# The version after 6.0.3 _will_ include the patch found here:
#   http://marc.theaimsgroup.com/?l=perf&m=111558757202243&w=2
#
# Until that version is released you may have to apply the patch
# yourself - yes, all 6 characters of it!
#

QUIET_SUBDIR0  = +$(MAKE) -C # space to separate -C and subdir
QUIET_SUBDIR1  =

ifneq ($(findstring $(MAKEFLAGS),w),w)
PRINT_DIR = --no-print-directory
else # "make -w"
NO_SUBDIR = :
endif

ifneq ($(findstring $(MAKEFLAGS),s),s)
<<<<<<< HEAD
ifndef V
	QUIET_ASCIIDOC	= @echo '   ' ASCIIDOC $@;
	QUIET_XMLTO	= @echo '   ' XMLTO $@;
	QUIET_DB2TEXI	= @echo '   ' DB2TEXI $@;
	QUIET_MAKEINFO	= @echo '   ' MAKEINFO $@;
	QUIET_DBLATEX	= @echo '   ' DBLATEX $@;
	QUIET_XSLTPROC	= @echo '   ' XSLTPROC $@;
	QUIET_GEN	= @echo '   ' GEN $@;
	QUIET_STDERR	= 2> /dev/null
	QUIET_SUBDIR0	= +@subdir=
	QUIET_SUBDIR1	= ;$(NO_SUBDIR) echo '   ' SUBDIR $$subdir; \
=======
ifneq ($(V),1)
	QUIET_ASCIIDOC	= @echo '  ASCIIDOC '$@;
	QUIET_XMLTO	= @echo '  XMLTO    '$@;
	QUIET_DB2TEXI	= @echo '  DB2TEXI  '$@;
	QUIET_MAKEINFO	= @echo '  MAKEINFO '$@;
	QUIET_DBLATEX	= @echo '  DBLATEX  '$@;
	QUIET_XSLTPROC	= @echo '  XSLTPROC '$@;
	QUIET_GEN	= @echo '  GEN      '$@;
	QUIET_STDERR	= 2> /dev/null
	QUIET_SUBDIR0	= +@subdir=
	QUIET_SUBDIR1	= ;$(NO_SUBDIR) \
			   echo '  SUBDIR   ' $$subdir; \
>>>>>>> refs/remotes/origin/master
			  $(MAKE) $(PRINT_DIR) -C $$subdir
	export V
endif
endif

all: html man

html: $(DOC_HTML)

$(DOC_HTML) $(DOC_MAN1) $(DOC_MAN5) $(DOC_MAN7): asciidoc.conf

man: man1 man5 man7
man1: $(DOC_MAN1)
man5: $(DOC_MAN5)
man7: $(DOC_MAN7)

<<<<<<< HEAD
<<<<<<< HEAD
info: perf.info perfman.info

pdf: user-manual.pdf
=======
info: $(OUTPUT)perf.info $(OUTPUT)perfman.info

pdf: $(OUTPUT)user-manual.pdf
>>>>>>> refs/remotes/origin/cm-10.0

install: install-man

install-man: man
	$(INSTALL) -d -m 755 $(DESTDIR)$(man1dir)
#	$(INSTALL) -d -m 755 $(DESTDIR)$(man5dir)
#	$(INSTALL) -d -m 755 $(DESTDIR)$(man7dir)
	$(INSTALL) -m 644 $(DOC_MAN1) $(DESTDIR)$(man1dir)
#	$(INSTALL) -m 644 $(DOC_MAN5) $(DESTDIR)$(man5dir)
#	$(INSTALL) -m 644 $(DOC_MAN7) $(DESTDIR)$(man7dir)

install-info: info
	$(INSTALL) -d -m 755 $(DESTDIR)$(infodir)
<<<<<<< HEAD
	$(INSTALL) -m 644 perf.info perfman.info $(DESTDIR)$(infodir)
=======
	$(INSTALL) -m 644 $(OUTPUT)perf.info $(OUTPUT)perfman.info $(DESTDIR)$(infodir)
>>>>>>> refs/remotes/origin/cm-10.0
	if test -r $(DESTDIR)$(infodir)/dir; then \
	  $(INSTALL_INFO) --info-dir=$(DESTDIR)$(infodir) perf.info ;\
	  $(INSTALL_INFO) --info-dir=$(DESTDIR)$(infodir) perfman.info ;\
=======
info: $(OUTPUT)perf.info $(OUTPUT)perfman.info

pdf: $(OUTPUT)user-manual.pdf

install: install-man

check-man-tools:
ifdef missing_tools
	$(error "You need to install $(missing_tools) for man pages")
endif

do-install-man: man
	$(call QUIET_INSTALL, Documentation-man) \
		$(INSTALL) -d -m 755 $(DESTDIR)$(man1dir); \
#		$(INSTALL) -d -m 755 $(DESTDIR)$(man5dir); \
#		$(INSTALL) -d -m 755 $(DESTDIR)$(man7dir); \
		$(INSTALL) -m 644 $(DOC_MAN1) $(DESTDIR)$(man1dir); \
#		$(INSTALL) -m 644 $(DOC_MAN5) $(DESTDIR)$(man5dir); \
#		$(INSTALL) -m 644 $(DOC_MAN7) $(DESTDIR)$(man7dir)

install-man: check-man-tools man

ifdef missing_tools
  DO_INSTALL_MAN = $(warning Please install $(missing_tools) to have the man pages installed)
else
  DO_INSTALL_MAN = do-install-man
endif

try-install-man: $(DO_INSTALL_MAN)

install-info: info
	$(call QUIET_INSTALL, Documentation-info) \
		$(INSTALL) -d -m 755 $(DESTDIR)$(infodir); \
		$(INSTALL) -m 644 $(OUTPUT)perf.info $(OUTPUT)perfman.info $(DESTDIR)$(infodir); \
	if test -r $(DESTDIR)$(infodir)/dir; then \
		$(INSTALL_INFO) --info-dir=$(DESTDIR)$(infodir) perf.info ;\
		$(INSTALL_INFO) --info-dir=$(DESTDIR)$(infodir) perfman.info ;\
>>>>>>> refs/remotes/origin/master
	else \
	  echo "No directory found in $(DESTDIR)$(infodir)" >&2 ; \
	fi

install-pdf: pdf
<<<<<<< HEAD
	$(INSTALL) -d -m 755 $(DESTDIR)$(pdfdir)
<<<<<<< HEAD
	$(INSTALL) -m 644 user-manual.pdf $(DESTDIR)$(pdfdir)
=======
	$(INSTALL) -m 644 $(OUTPUT)user-manual.pdf $(DESTDIR)$(pdfdir)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	$(call QUIET_INSTALL, Documentation-pdf) \
		$(INSTALL) -d -m 755 $(DESTDIR)$(pdfdir); \
		$(INSTALL) -m 644 $(OUTPUT)user-manual.pdf $(DESTDIR)$(pdfdir)
>>>>>>> refs/remotes/origin/master

#install-html: html
#	'$(SHELL_PATH_SQ)' ./install-webdoc.sh $(DESTDIR)$(htmldir)

<<<<<<< HEAD
../PERF-VERSION-FILE: .FORCE-PERF-VERSION-FILE
	$(QUIET_SUBDIR0)../ $(QUIET_SUBDIR1) PERF-VERSION-FILE

-include ../PERF-VERSION-FILE
=======
>>>>>>> refs/remotes/origin/master

#
# Determine "include::" file references in asciidoc files.
#
<<<<<<< HEAD
<<<<<<< HEAD
doc.dep : $(wildcard *.txt) build-docdep.perl
=======
$(OUTPUT)doc.dep : $(wildcard *.txt) build-docdep.perl
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(OUTPUT)doc.dep : $(wildcard *.txt) build-docdep.perl
>>>>>>> refs/remotes/origin/master
	$(QUIET_GEN)$(RM) $@+ $@ && \
	$(PERL_PATH) ./build-docdep.perl >$@+ $(QUIET_STDERR) && \
	mv $@+ $@

<<<<<<< HEAD
<<<<<<< HEAD
-include doc.dep

cmds_txt = cmds-ancillaryinterrogators.txt \
=======
-include $(OUPTUT)doc.dep

_cmds_txt = cmds-ancillaryinterrogators.txt \
>>>>>>> refs/remotes/origin/cm-10.0
=======
-include $(OUPTUT)doc.dep

_cmds_txt = cmds-ancillaryinterrogators.txt \
>>>>>>> refs/remotes/origin/master
	cmds-ancillarymanipulators.txt \
	cmds-mainporcelain.txt \
	cmds-plumbinginterrogators.txt \
	cmds-plumbingmanipulators.txt \
	cmds-synchingrepositories.txt \
	cmds-synchelpers.txt \
	cmds-purehelpers.txt \
	cmds-foreignscminterface.txt
<<<<<<< HEAD
<<<<<<< HEAD

$(cmds_txt): cmd-list.made

cmd-list.made: cmd-list.perl ../command-list.txt $(MAN1_TXT)
=======
=======
>>>>>>> refs/remotes/origin/master
cmds_txt=$(addprefix $(OUTPUT),$(_cmds_txt))

$(cmds_txt): $(OUTPUT)cmd-list.made

$(OUTPUT)cmd-list.made: cmd-list.perl ../command-list.txt $(MAN1_TXT)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	$(QUIET_GEN)$(RM) $@ && \
	$(PERL_PATH) ./cmd-list.perl ../command-list.txt $(QUIET_STDERR) && \
	date >$@

<<<<<<< HEAD
clean:
<<<<<<< HEAD
	$(RM) *.xml *.xml+ *.html *.html+ *.1 *.5 *.7
	$(RM) *.texi *.texi+ *.texi++ perf.info perfman.info
	$(RM) howto-index.txt howto/*.html doc.dep
	$(RM) technical/api-*.html technical/api-index.txt
	$(RM) $(cmds_txt) *.made

$(MAN_HTML): %.html : %.txt
=======
	$(RM) $(MAN_XML) $(addsuffix +,$(MAN_XML))
	$(RM) $(MAN_HTML) $(addsuffix +,$(MAN_HTML))
	$(RM) $(DOC_HTML) $(DOC_MAN1) $(DOC_MAN5) $(DOC_MAN7)
	$(RM) $(OUTPUT)*.texi $(OUTPUT)*.texi+ $(OUTPUT)*.texi++
	$(RM) $(OUTPUT)perf.info $(OUTPUT)perfman.info
	$(RM) $(OUTPUT)howto-index.txt $(OUTPUT)howto/*.html $(OUTPUT)doc.dep
	$(RM) $(OUTPUT)technical/api-*.html $(OUTPUT)technical/api-index.txt
	$(RM) $(cmds_txt) $(OUTPUT)*.made

$(MAN_HTML): $(OUTPUT)%.html : %.txt
>>>>>>> refs/remotes/origin/cm-10.0
=======
CLEAN_FILES =									\
	$(MAN_XML) $(addsuffix +,$(MAN_XML))					\
	$(MAN_HTML) $(addsuffix +,$(MAN_HTML))					\
	$(DOC_HTML) $(DOC_MAN1) $(DOC_MAN5) $(DOC_MAN7)				\
	$(OUTPUT)*.texi $(OUTPUT)*.texi+ $(OUTPUT)*.texi++			\
	$(OUTPUT)perf.info $(OUTPUT)perfman.info				\
	$(OUTPUT)howto-index.txt $(OUTPUT)howto/*.html $(OUTPUT)doc.dep		\
	$(OUTPUT)technical/api-*.html $(OUTPUT)technical/api-index.txt		\
	$(cmds_txt) $(OUTPUT)*.made
clean:
	$(call QUIET_CLEAN, Documentation) $(RM) $(CLEAN_FILES)

$(MAN_HTML): $(OUTPUT)%.html : %.txt
>>>>>>> refs/remotes/origin/master
	$(QUIET_ASCIIDOC)$(RM) $@+ $@ && \
	$(ASCIIDOC) -b xhtml11 -d manpage -f asciidoc.conf \
		$(ASCIIDOC_EXTRA) -aperf_version=$(PERF_VERSION) -o $@+ $< && \
	mv $@+ $@

<<<<<<< HEAD
<<<<<<< HEAD
%.1 %.5 %.7 : %.xml
	$(QUIET_XMLTO)$(RM) $@ && \
	xmlto -m $(MANPAGE_XSL) $(XMLTO_EXTRA) man $<

%.xml : %.txt
=======
$(OUTPUT)%.1 $(OUTPUT)%.5 $(OUTPUT)%.7 : $(OUTPUT)%.xml
	$(QUIET_XMLTO)$(RM) $@ && \
	xmlto -o $(OUTPUT) -m $(MANPAGE_XSL) $(XMLTO_EXTRA) man $<

$(OUTPUT)%.xml : %.txt
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(OUTPUT)%.1 $(OUTPUT)%.5 $(OUTPUT)%.7 : $(OUTPUT)%.xml
	$(QUIET_XMLTO)$(RM) $@ && \
	$(XMLTO) -o $(OUTPUT). -m $(MANPAGE_XSL) $(XMLTO_EXTRA) man $<

$(OUTPUT)%.xml : %.txt
>>>>>>> refs/remotes/origin/master
	$(QUIET_ASCIIDOC)$(RM) $@+ $@ && \
	$(ASCIIDOC) -b docbook -d manpage -f asciidoc.conf \
		$(ASCIIDOC_EXTRA) -aperf_version=$(PERF_VERSION) -o $@+ $< && \
	mv $@+ $@

XSLT = docbook.xsl
XSLTOPTS = --xinclude --stringparam html.stylesheet docbook-xsl.css

<<<<<<< HEAD
<<<<<<< HEAD
user-manual.html: user-manual.xml
	$(QUIET_XSLTPROC)xsltproc $(XSLTOPTS) -o $@ $(XSLT) $<

perf.info: user-manual.texi
	$(QUIET_MAKEINFO)$(MAKEINFO) --no-split -o $@ user-manual.texi

user-manual.texi: user-manual.xml
	$(QUIET_DB2TEXI)$(RM) $@+ $@ && \
	$(DOCBOOK2X_TEXI) user-manual.xml --encoding=UTF-8 --to-stdout >$@++ && \
=======
=======
>>>>>>> refs/remotes/origin/master
$(OUTPUT)user-manual.html: $(OUTPUT)user-manual.xml
	$(QUIET_XSLTPROC)xsltproc $(XSLTOPTS) -o $@ $(XSLT) $<

$(OUTPUT)perf.info: $(OUTPUT)user-manual.texi
	$(QUIET_MAKEINFO)$(MAKEINFO) --no-split -o $@ $(OUTPUT)user-manual.texi

$(OUTPUT)user-manual.texi: $(OUTPUT)user-manual.xml
	$(QUIET_DB2TEXI)$(RM) $@+ $@ && \
	$(DOCBOOK2X_TEXI) $(OUTPUT)user-manual.xml --encoding=UTF-8 --to-stdout >$@++ && \
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	$(PERL_PATH) fix-texi.perl <$@++ >$@+ && \
	rm $@++ && \
	mv $@+ $@

<<<<<<< HEAD
<<<<<<< HEAD
user-manual.pdf: user-manual.xml
=======
$(OUTPUT)user-manual.pdf: $(OUTPUT)user-manual.xml
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(OUTPUT)user-manual.pdf: $(OUTPUT)user-manual.xml
>>>>>>> refs/remotes/origin/master
	$(QUIET_DBLATEX)$(RM) $@+ $@ && \
	$(DBLATEX) -o $@+ -p /etc/asciidoc/dblatex/asciidoc-dblatex.xsl -s /etc/asciidoc/dblatex/asciidoc-dblatex.sty $< && \
	mv $@+ $@

<<<<<<< HEAD
<<<<<<< HEAD
perfman.texi: $(MAN_XML) cat-texi.perl
=======
$(OUTPUT)perfman.texi: $(MAN_XML) cat-texi.perl
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(OUTPUT)perfman.texi: $(MAN_XML) cat-texi.perl
>>>>>>> refs/remotes/origin/master
	$(QUIET_DB2TEXI)$(RM) $@+ $@ && \
	($(foreach xml,$(MAN_XML),$(DOCBOOK2X_TEXI) --encoding=UTF-8 \
		--to-stdout $(xml) &&) true) > $@++ && \
	$(PERL_PATH) cat-texi.perl $@ <$@++ >$@+ && \
	rm $@++ && \
	mv $@+ $@

<<<<<<< HEAD
<<<<<<< HEAD
perfman.info: perfman.texi
=======
$(OUTPUT)perfman.info: $(OUTPUT)perfman.texi
>>>>>>> refs/remotes/origin/cm-10.0
=======
$(OUTPUT)perfman.info: $(OUTPUT)perfman.texi
>>>>>>> refs/remotes/origin/master
	$(QUIET_MAKEINFO)$(MAKEINFO) --no-split --no-validate $*.texi

$(patsubst %.txt,%.texi,$(MAN_TXT)): %.texi : %.xml
	$(QUIET_DB2TEXI)$(RM) $@+ $@ && \
	$(DOCBOOK2X_TEXI) --to-stdout $*.xml >$@+ && \
	mv $@+ $@

howto-index.txt: howto-index.sh $(wildcard howto/*.txt)
	$(QUIET_GEN)$(RM) $@+ $@ && \
	'$(SHELL_PATH_SQ)' ./howto-index.sh $(wildcard howto/*.txt) >$@+ && \
	mv $@+ $@

$(patsubst %,%.html,$(ARTICLES)) : %.html : %.txt
	$(QUIET_ASCIIDOC)$(ASCIIDOC) -b xhtml11 $*.txt

WEBDOC_DEST = /pub/software/tools/perf/docs

$(patsubst %.txt,%.html,$(wildcard howto/*.txt)): %.html : %.txt
	$(QUIET_ASCIIDOC)$(RM) $@+ $@ && \
	sed -e '1,/^$$/d' $< | $(ASCIIDOC) -b xhtml11 - >$@+ && \
	mv $@+ $@

# UNIMPLEMENTED
#install-webdoc : html
#	'$(SHELL_PATH_SQ)' ./install-webdoc.sh $(WEBDOC_DEST)

# quick-install: quick-install-man

# quick-install-man:
#	'$(SHELL_PATH_SQ)' ./install-doc-quick.sh $(DOC_REF) $(DESTDIR)$(mandir)

#quick-install-html:
#	'$(SHELL_PATH_SQ)' ./install-doc-quick.sh $(HTML_REF) $(DESTDIR)$(htmldir)
<<<<<<< HEAD

.PHONY: .FORCE-PERF-VERSION-FILE
=======
>>>>>>> refs/remotes/origin/master
