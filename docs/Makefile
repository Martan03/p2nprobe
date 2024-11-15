NAME:=manual

$(NAME).pdf: $(NAME).tex $(NAME).bib
	pdflatex $(NAME)
	biber $(NAME)
	pdflatex $(NAME)
	pdflatex $(NAME)

.PHONY:
clean:
	rm -f *.aux *.bbl *.bcf *.blg *.log *.run.xml *.pdf *.fdb_latexmk *.fls \
		*.out *.synctex.gz *.toc

