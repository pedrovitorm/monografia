set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Quantidade"
set autoscale
set grid
set title "Estoques"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan"
