TARGET	=	slides
BIBLIO	=	slides.bib

LATEX	=	latex
BIBTEX	=	bibtex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/pipe1.eps			\
		pics/pipe2.eps			\
		pics/pipe3.eps			\
		pics/fifo1.eps			\
		pics/fifo2.eps			\
		pics/socketpair1.eps		\
		pics/socketpair2.eps		\
		pics/memory-layout.eps		\
		pics/ipcflow.eps		\
		pics/shmflow.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png .jpg .gif

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

.png.eps:
	pngtopnm $< | pnmtops > $@

.gif.eps:
	giftopnm $< | pnmtops > $@

.jpg.eps:
	jpegtopnm $< | pnmtops > $@

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
	rm -fr read send socket

udgram: udgramsend.c udgramread.c
	cc -Wall udgramsend.c -o send
	cc -Wall udgramread.c -o read

dgram: dgramsend.c dgramread.c
	cc -Wall dgramsend.c -o send
	cc -Wall dgramread.c -o read

stream: streamread.c streamwrite.c
	cc -Wall streamwrite.c -o send
	cc -Wall streamread.c -o read
