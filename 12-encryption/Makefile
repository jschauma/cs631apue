TARGET	=	slides

LATEX	=	latex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=	pics/symmetric-key-crypto.eps	\
		pics/aes.eps			\
		pics/ecb.eps			\
		pics/Public_key_encryption.eps	\
		pics/encryption-handshake.eps	\
		pics/Ecb_encryption.eps		\
		pics/Ecb_decryption.eps		\
		pics/Cbc_encryption.eps		\
		pics/Cbc_decryption.eps		\
		pics/ivec.eps

.SUFFIXES: .tex .dvi .ps .pdf .txt .fig .eps .png

all:	$(FIGURES) $(TARGET).dvi

slides:

.tex.dvi:
	$(LATEX) $<
	$(LATEX) $<

.fig.eps:
	fig2dev -L eps $< > $@

ps:	$(TARGET).dvi $(TARGET).ps

.dvi.ps:
	$(DVIPS) $@ $<

.png.eps:
	convert $< $@

pdf:  $(FIGURES) $(TARGET).dvi $(TARGET).ps $(TARGET).pdf

txt: $(TARGET).dvi $(TARGET).ps $(TARGET).pdf $(TARGET).txt

html:
	latex2html $(LATEX2HTMLOPTS) $(TARGET).tex

.ps.pdf:
	$(PS2PDF) $< $@

.pdf.txt:
	$(PDFTOTEXT) $< $@

clean:
	rm -f *.out **.bbl *.blg *.log *.aux *.dvi slides.ps *.pdf *.toc *.bak *.lof ${FIGURES}
	rm -fr $(TARGET)/
