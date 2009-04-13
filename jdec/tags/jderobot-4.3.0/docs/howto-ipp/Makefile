DOCUMENTO = howto-ipp
TEMPORALES = *idx *aux *lof *log *lot *toc *bbl *blg *~ *out

all: pdf 

dvi: ${DOCUMENTO}.tex
	latex ${DOCUMENTO}

ps: dvi
	dvips -o ${DOCUMENTO}.ps ${DOCUMENTO}.dvi

pdf: 
	#pdflatex ${DOCUMENTO}
	pdflatex ${DOCUMENTO}
	pdflatex ${DOCUMENTO}
	pdflatex ${DOCUMENTO}
	rm -f $(TEMPORALES) *dvi

clean:
	rm -f $(TEMPORALES) *pdf *dvi

release:

	rm -f $(TEMPORALES) *pdf *dvi
	tar -zcvf newrelease.tgz *


