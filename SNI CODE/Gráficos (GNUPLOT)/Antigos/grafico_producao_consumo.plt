set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Produção e Consumo"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
     '../Dados/mercado.csv' using 1:15 with lines title "Prod_total" lt rgb "red"
