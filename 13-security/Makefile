TARGET	=	slides

LATEX	=	latex
DVIPS	=	dvips -o
PS2PDF	=	ps2pdf
PDFTOTEXT=	pdftotext

FIGURES=					\
		pics/Cbc_decryption.eps		\
		pics/Cbc_encryption.eps		\
		pics/Ecb_decryption.eps		\
		pics/Ecb_encryption.eps		\
		pics/Public-key-crypto-1.eps	\
		pics/Public_key_encryption.eps	\
		pics/Public_key_shared_secret.eps \
		pics/aes.eps			\
		pics/ecb.eps			\
		pics/encryption-handshake.eps	\
		pics/exim.eps			\
		pics/exploits_of_a_mom.eps	\
		pics/gotofail.eps		\
		pics/ivec.eps			\
		pics/random_number.eps		\
		pics/sony-ps3.eps		\
		pics/symmetric-key-crypto.eps	\
		pics/uber.eps

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
