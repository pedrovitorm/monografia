set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Capital medio das empresas"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:9 with lines title "Capital medio" lt rgb "magenta"
