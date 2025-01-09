set terminal png size 520,260
set output 'grafico_full_hd.png'
set datafile separator ","
set key below
set xlabel "Iteração" font ",15," offset -22,0
set ylabel "Valores" font ",15,"
set autoscale
set grid

plot '../Dados/mercado.csv' using 1:2 with lines title "Salário Médio" lt rgb "blue" lw 3, \
     '../Dados/mercado.csv' using 1:3 with lines title "Preço Médio" lt rgb "red" lw 3, \
     '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green" lw 3, \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple" lw 3, \
     '../Dados/mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange" lw 3, \
     '../Dados/mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown" lw 3
