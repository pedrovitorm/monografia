set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Riqueza media"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:10 with lines title "Riqueza media" lt rgb "yellow"
