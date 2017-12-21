# $Id: Makefile,v 1.1 2004/07/27 15:33:29 jschauma Exp $

TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/process-tree.eps	\
		pics/session1.eps	\
		pics/session2.eps	\
		pics/session3.eps	\
		pics/session4.eps	\
		pics/session5.eps	\
		pics/shithappens.eps	\
		pics/jobcontrol.eps	\
		pics/jc1.eps		\
		pics/jc2.eps		\
		pics/jc3.eps		\
		pics/jc4.eps		\
		pics/jc5.eps		\
		pics/jc6.eps		\
		pics/pscat.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png .jpg

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

.jpg.eps:
	jpegtopnm $< | pnmtops > $@

.png.eps:
	pngtopnm $< | pnmtops > $@

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
