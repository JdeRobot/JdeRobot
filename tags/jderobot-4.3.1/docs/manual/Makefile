all: jdecmanual.bbl pdf

pdf: jdecmanual.tex
	pdflatex jdecmanual.tex

ps: jdecmanual.dvi
	dvips -o jdecmanual.ps -ta4 jdecmanual.dvi

jdecmanual.bbl: jdecmanual.aux jdecmanual.bib
	bibtex jdecmanual

jdecmanual.aux:
	pdflatex jdecmanual.tex

jdecmanual.dvi: jdecmanual.tex
	latex jdecmanual.tex

html: jdecmanual.tex
	rm -rf html; mkdir html; latex2html -dir html jdecmanual.tex

clean:
	rm -f jdecmanual.dvi jdecmanual.ps *aux *idx *log *out *toc *bbl *blg jdecmanual.pdf

version:
	tar -zcvf nuevaversion.tgz jdecmanual.tex makefile *sty *bib figs/*

