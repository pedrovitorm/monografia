set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Quantidade"
set autoscale
set grid
set title "Numero de empresas ativas"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:14 with lines title "Ativas" lt rgb "green"
