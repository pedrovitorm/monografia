set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Taxa (%)"
set autoscale
set grid
set title "Taxa de desemprego"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:12 with lines title "Desemprego %" lt rgb "orange"
