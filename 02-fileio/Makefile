# $Id: Makefile,v 1.1 2005/01/17 20:10:57 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/stdstreams.eps pics/pipeline.eps \
		pics/creation.eps pics/tape.eps pics/reading.eps \
		pics/open-files.eps pics/open-files-sharing.eps \
		pics/dup.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png .jpg .gif

all:	$(FIGURES) $(TARGET).dvi
#	${BIBTEX} $(TARGET)

show: ${FIGURES} ${TARGET}.dvi
	xsetroot -solid white
	xdvipresent xga ${TARGET}.dvi -rulecolor white -bg white -hl white -paper 25.8x19.3 -fullscreen -geometry 1440x900+70-0 -p 740
#show: ${TARGET}.pdf
#	open ${TARGET}.pdf

slides:	#example.knuth.eps example.knuth2.eps example.knuth3.eps

.tex.dvi: ${FIGURES}
	$(LATEX) $<
	$(LATEX) $<

.fig.eps:
	fig2dev -L eps $< > $@

.png.eps:
	pngtopnm $< | pnmtops > $@

.jpg.eps:
	jpegtopnm $< | pnmtops > $@

.gif.eps:
	giftopnm $< | pnmtops > $@

ps:	$(TARGET).dvi $(TARGET).ps

# PS will be upside down - add the following line after each "%%Page: "
# directive:
# 180 rotate -612 -792 translate
.dvi.ps:
	$(DVIPS) $@ $<

pdf:  $(FIGURES) $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

txt: $(TARGET).dvi $(TARGET).ps $(TARGET).pdf $(TARGET).txt

html:
	latex2html $(LATEX2HTMLOPTS) $(TARGET).tex

.ps.pdf:
	$(PS2PDF) $< $@

.pdf.txt:
	$(PDFTOTEXT) $< $@

clean:
	rm -f *.out **.bbl *.blg *.log *.aux *.dvi *.ps *.pdf *.toc *.bak *.lof ${FIGURES}
	rm -fr $(TARGET)/

###
### Code examples
###

code-clean:
	rm -f a.out fifo file.hole file.nohole hole newfile openmax openex rwex
	rm -fr tmp
	rm -f async-cat.c ascat scat out

openmax: openmax.c
	cc -Wall $< -o $@

openex: openex.c
	cc -Wall $< -o $@

rwex: rwex.c
	cc -Wall $< -o $@

hole: hole.c
	cc -Wall $< -o $@

catio: tmpfiles ../01-intro/simple-cat.c
	for n in 1048576 32768 16384 4096 512 256 128 64 1 ; do		\
		echo "BUFFSIZE = $$n";					\
		cc -Wall -DBUFFSIZE=$$n ../01-intro/simple-cat.c;	\
		i=$$(( $$i + 1 ));					\
		time ./a.out <tmp/file$$i >tmp/file$$i.copy;		\
		echo;							\
	done;

tmpfiles: tmp/file9

tmp/file9:
	mkdir -p tmp
	for n in 0 1 2 3 4 5 6 7 8 9; do				\
		dd if=/dev/urandom of=tmp/file$$n count=204800;		\
	done;

sync: tmpfiles scat
	time ./scat <tmp/file1 >out

scat: sync-cat.c
	cc -Wall sync-cat.c -o scat


async: tmpfiles ascat
	time ./ascat <tmp/file1 >out

ascat: async-cat.c
	cc -Wall $< -o $@

async-cat.c: sync-cat.c
	sed -e 's|\(.*O_SYNC.*\)|//\1|' sync-cat.c > async-cat.c
