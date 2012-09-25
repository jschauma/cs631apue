# $Id: Makefile,v 1.1 2004/07/19 03:45:51 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/fs1.eps		\
		pics/fs2.eps		\
		pics/fs3.eps		\
		pics/fs4.eps		\
		pics/links1.eps		\
		pics/links2.eps		\
		pics/links3.eps		\
		pics/links4.eps 	\
		pics/subdirs1.eps	\
		pics/subdirs2.eps	\
		pics/subdirs3.eps 	\
		pics/tdd.eps		\
		pics/strftime.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .gif .jpg

all:	$(FIGURES) $(TARGET).dvi
#	${BIBTEX} $(TARGET)

#show: ${TARGET}.dvi
#	xdvipresent xga ${TARGET}.dvi -paper 25.8x19.3 -geometry 1024x764-0-0
show: ${FIGURES} ${TARGET}.pdf
	open ${TARGET}.pdf

slides:	#example.knuth.eps example.knuth2.eps example.knuth3.eps

.tex.dvi:
	$(LATEX) $<
	$(LATEX) $<

.fig.eps:
	fig2dev -L eps $< > $@

.gif.eps:
	giftopnm $< | pnmtops > $@

.jpg.eps:
	convert $< $@

ps:	$(TARGET).dvi $(TARGET).ps

# PS will be upside down - add the following line after each "%%Page: "
# directive:
# 180 rotate -612 -792 translate
.dvi.ps:
	$(DVIPS) $@ $<

pdf:  ${FIGURES} $(TARGET).tex $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

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
