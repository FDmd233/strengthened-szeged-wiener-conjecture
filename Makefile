PAPER=Szeged_Wiener_2026-09-08

.PHONY: all paper verify clean

all: paper

paper:
	latexmk -xelatex -interaction=nonstopmode -halt-on-error $(PAPER).tex

verify:
	cd certificate && ./verify.sh

clean:
	latexmk -C $(PAPER).tex
