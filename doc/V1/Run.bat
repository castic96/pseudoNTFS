@echo off
echo Pokud probehne bez chyb, vytvori se soubor Dokumentace.pdf.
echo V pripade chyb spust rucne "pdflatex Dokumentace.tex" a oprav pripadne chyby.
del Dokumentace.pdf 1>nul 2>nul
pdflatex Dokumentace.tex 1>nul
pdflatex Dokumentace.tex 1>nul