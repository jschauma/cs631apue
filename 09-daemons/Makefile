# $Id: Makefile,v 1.1 2005/11/01 03:37:40 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/daemon.eps pics/syslog.eps pics/elf.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png

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
