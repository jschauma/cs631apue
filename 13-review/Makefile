# $Id: Makefile,v 1.1 2005/01/17 20:10:57 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/unix_architecture.eps \
		pics/pipeline.eps pics/process-tree.eps \
		pics/epoch-fail.eps \
		pics/stdstreams.eps \
		pics/open-files.eps pics/open-files-sharing.eps \
		pics/dup.eps pics/fs1.eps pics/fs2.eps pics/fs3.eps \
		pics/fs4.eps pics/links1.eps pics/links2.eps \
		pics/links3.eps pics/links4.eps pics/subdirs1.eps pics/subdirs2.eps \
		pics/subdirs3.eps pics/strftime.eps \
		pics/lifetime6.eps pics/process9.eps pics/fork1.eps \
		pics/fork2.eps pics/session1.eps pics/session2.eps \
		pics/session3.eps pics/session4.eps pics/session5.eps \
		pics/pscat.eps pics/jc3.eps pics/jc4.eps pics/jc2.eps \
		pics/jc6.eps  pics/jc5.eps pics/jc1.eps \
		pics/thats-all-folks.eps


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

pdf:  ${FIGURES} $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

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
