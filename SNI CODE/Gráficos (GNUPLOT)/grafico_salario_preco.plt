set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Preco medio"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:3 with lines title "Preco medio" lt rgb "red"
