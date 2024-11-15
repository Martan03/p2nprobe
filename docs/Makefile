NAME:=manual

$(NAME).pdf: $(NAME).tex $(NAME).bib
	pdflatex $(NAME)
	biber $(NAME)
	pdflatex $(NAME)
	pdflatex $(NAME)

.PHONY:
clean:
	rm -f $(NAME).aux $(NAME).bbl $(NAME).bcf $(NAME).blg $(NAME).log \
		$(NAME).run.xml $(NAME).pdf

