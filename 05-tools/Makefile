TARGET	=	slides

LATEX	=	latex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/terminal.eps		\
		pics/ide.eps			\
		pics/compiler.eps		\
		pics/ls.eps			\
		pics/ls-changed1.eps		\
		pics/ls-changed2.eps		\
		pics/ls-changed3.eps		\
		pics/ls-changed4.eps		\
		pics/compiler_monster.eps	\
		pics/makerbot.eps		\
		pics/debugging.eps		\
		pics/vim.eps

.SUFFIXES: .tex .dvi .ps .pdf .fig .eps .png .dot .jpg .gif

all:	$(FIGURES) $(TARGET).dvi

slides:

.tex.dvi:
	$(LATEX) $<
	$(LATEX) $<

.jpg.eps:
	convert $< $@

.gif.eps:
	convert $< $@

.png.eps:
	pngtopnm $< | pnmtops > $@

.dot.eps:
	dot -Tpng $< | pngtopnm | pnmtops > $@

.fig.eps:
	fig2dev -L eps $< > $@

ps:	$(TARGET).dvi $(TARGET).ps

.dvi.ps:
	$(DVIPS) $@ $<

pdf:  $(FIGURES) $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

.ps.pdf:
	$(PS2PDF) $< $@

clean:
	rm -f **.bbl *.blg *.log *.aux *.dvi *.ps *.pdf *.toc *.bak *.lof ${FIGURES} *.out
	rm -fr $(TARGET)/
