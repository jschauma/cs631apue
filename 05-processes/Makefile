TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/lifetime1.eps	\
		pics/lifetime2.eps	\
		pics/lifetime3.eps	\
		pics/lifetime4.eps	\
		pics/lifetime5.eps	\
		pics/lifetime6.eps	\
		pics/process1.eps	\
		pics/process2.eps 	\
		pics/process3.eps	\
		pics/process4.eps	\
		pics/process5.eps	\
		pics/process6.eps	\
		pics/process7.eps	\
		pics/process8.eps	\
		pics/process9.eps	\
		pics/fork1.eps		\
		pics/fork2.eps		\
		pics/zombies.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png .jpg

all:	$(FIGURES) $(TARGET).dvi

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

.png.eps:
	pngtopnm $< | pnmtops > $@

.jpg.eps:
	convert $< $@

ps:	$(TARGET).dvi $(TARGET).ps

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
