DOCUMENTO = jderobot
TEMPORALES = *idx *aux *lof *log *lot *toc *bbl *blg *~ *out *rel

all: pdf 

dvi: ${DOCUMENTO}.tex
	latex ${DOCUMENTO}

ps: dvi
	dvips -o ${DOCUMENTO}.ps ${DOCUMENTO}.dvi

pdf: 
	pdflatex ${DOCUMENTO}
	pdflatex ${DOCUMENTO}
	bibtex ${DOCUMENTO}.aux
	pdflatex ${DOCUMENTO}
	bibtex ${DOCUMENTO}.aux
	pdflatex ${DOCUMENTO}
	rm -f $(TEMPORALES) *dvi
#version:
#	tar cvfz nuevaversion.tgz ${DOCUMENTO}.tex  ./figs/* Makefile 

clean:
	rm -f $(TEMPORALES) *pdf *dvi

release:

	rm -f $(TEMPORALES) *pdf *dvi
	tar -zcvf newrelease.tgz *
