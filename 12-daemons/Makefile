# $Id: Makefile,v 1.1 2005/11/01 03:37:40 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/datatransfer.eps pics/daemon.eps pics/syslog.eps \
		pics/locking.eps pics/locking-structures.eps \
		pics/mmap.eps pics/mmap-timing.eps \
		pics/aio1.eps pics/aio2.eps pics/aio3.eps \
		pics/aio4.eps pics/aio5.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png .jpg .gif

all:	$(FIGURES) $(TARGET).dvi
#	${BIBTEX} $(TARGET)

#show: ${TARGET}.dvi
#	xdvipresent xga ${TARGET}.dvi -paper 25.8x19.3 -geometry 1024x764-0-0
show: ${FIGURES} ${TARGET}.pdf
	open ${TARGET}.pdf

slides:

.tex.dvi:
	$(LATEX) $<
	$(LATEX) $<

.fig.eps:
	fig2dev -L eps $< > $@

ps:	$(TARGET).dvi $(TARGET).ps

# PS will be upside down - add the following line after each "%%Page: "
# directive:
# 180 rotate -612 -792 translate
.dvi.ps:
	$(DVIPS) $@ $<

.jpg.eps:
	jpegtopnm $< | pnmtops > $@

.gif.eps:
	giftopnm $< | pnmtops > $@

.png.eps:
	pngtopnm $< | pnmtops > $@

pdf:  ${FIGURES} $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

txt: $(TARGET).dvi $(TARGET).ps $(TARGET).pdf $(TARGET).txt

html:
	latex2html $(LATEX2HTMLOPTS) $(TARGET).tex

.ps.pdf:
	$(PS2PDF) $< $@

.pdf.txt:
	$(PDFTOTEXT) $< $@

clean:
	rm -f **.bbl *.blg *.log *.aux *.dvi *.ps *.pdf *.toc *.bak *.lof ${FIGURES}
	rm -fr $(TARGET)/
	rm -f *.o *.out

###
### Code Examples
###

CFLAGS=-Wall -fPIC

help:
	@echo "The following targets are available:"
	@echo "code-clean:  remove all object files, executables, libraries etc."
	@echo "evil:        build an evil dynamic library into lib3/libldtest.so"
	@echo "libldtest:   build a dynamic library into lib/libldtest.so"
	@echo "libldtest2:  build a second dynamic library into lib2/libldtest.so"
	@echo "rpath:       build an executable with a fixed rpath"

code-clean:
	rm -f *.o libldtest* a.out*
	rm -fr lib lib2 lib3

libldtest.a: ldtest1.o ldtest2.o
	ar -vq libldtest.a ldtest1.o ldtest2.o
	ar -t libldtest.a

rpath: lib/libldtest.so
	cc -Wall main.c -L./lib -lldtest -Wl,-rpath,./lib


libldtest: lib/libldtest.so

lib/libldtest.so: ldtest1.o ldtest2.o
	mkdir -p lib
	cc -shared -Wl,-soname,libldtest.so.1 -o lib/libldtest.so.1.0 ldtest1.o ldtest2.o
	ln -sf libldtest.so.1.0 lib/libldtest.so.1
	ln -sf libldtest.so.1 lib/libldtest.so

libldtest2: lib2/libldtest.so

lib2/libldtest.so: ldtest1.2.o ldtest2.o
	mkdir -p lib2
	cc -shared -Wl,-soname,libldtest.so.1 -o lib2/libldtest.so.1.0 ldtest1.2.o ldtest2.o
	ln -sf libldtest.so.1.0 lib2/libldtest.so.1
	ln -sf libldtest.so.1 lib2/libldtest.so

evil: evil.o ldtest1.o ldtest2.o
	mkdir -p lib3
	cc -shared -Wl,-soname,libldtest.so.1 -o lib3/libldtest.so.1.0 ldtest1.o ldtest2.o evil.o
	ln -sf libldtest.so.1.0 lib3/libldtest.so.1
	ln -sf libldtest.so.1 lib3/libldtest.so

libgreet: greet/libgreet.so
	cd greet && make
