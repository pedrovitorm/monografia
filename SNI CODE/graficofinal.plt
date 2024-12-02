set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set title "Dados em Tempo Real"
set autoscale
set grid

plot 'mercado.csv' using 1:2 with lines title "Salario Medio" lt rgb "blue", \
        'mercado.csv' using 1:3 with lines title "Preco Medio" lt rgb "red", \
        'mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
        'mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
        'mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange", \
        'mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown", \
        #'mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan", \
        #'mercado.csv' using 1:9 with lines title "Capital Medio" lt rgb "magenta", \
        'mercado.csv' using 1:10 with lines title "Riqueza Media" lt rgb "yellow"
