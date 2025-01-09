set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Quantidade"
set autoscale
set grid
set title "Base monetaria"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:13 with lines title "Quantidade" lt rgb "blue"
