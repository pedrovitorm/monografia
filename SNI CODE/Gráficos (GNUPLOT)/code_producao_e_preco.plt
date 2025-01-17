set datafile separator ","
set key below
set xlabel "Itr." font "Arial,12," offset -24,1
set ylabel "Valores" font "Arial,12," offset 2,0
set key font "Arial,12,"
set xtics font "Arial,12,"
set ytics font "Arial,12,"
set autoscale
set grid

set terminal png size 1000, 250
set output 'grafico_producao_e_preco.png'
set multiplot layout 1, 2

set title "Preço médio" offset 0,-1
plot '../Dados/mercado.csv' using 1:3 with lines title "Preço médio" lt rgb "red" lw 3

set title "Produção e Consumo"
plot '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green" lw 3, \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple" lw 3, \
     '../Dados/mercado.csv' using 1:15 with lines title "Máximo" lt rgb "red" lw 3

unset multiplot