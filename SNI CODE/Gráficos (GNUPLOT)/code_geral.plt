set datafile separator ","
set key below
set xlabel "Itr." font "Arial,12," offset -24,1
set ylabel "Valores" font "Arial,12," offset 2,0
set key font "Arial,12,"
set xtics font "Arial,12,"
set ytics font "Arial,12,"
set autoscale
set grid

set terminal png size 1000, 850
set output 'grafico_geral.png'
set multiplot layout 4, 2

set title "Preço médio" offset 0,-1
plot '../Dados/mercado.csv' using 1:3 with lines title "Preço médio" lt rgb "red" lw 3

set title "Produção e Consumo"
plot '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green" lw 3, \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple" lw 3, \
     '../Dados/mercado.csv' using 1:15 with lines title "Máximo" lt rgb "red" lw 3

set title "Taxa de desemprego"
plot '../Dados/mercado.csv' using 1:12 with lines title "Desemprego %" lt rgb "orange" lw 3

set title "Estoques"
plot '../Dados/mercado.csv' using 1:8 with lines title "Estoques" lt rgb "brown" lw 3

set title "Riqueza média"
plot '../Dados/mercado.csv' using 1:10 with lines title "Riqueza média" lt rgb "dark-grey" lw 3

set title "Capital médio das empresas"
plot '../Dados/mercado.csv' using 1:9 with lines title "Capital médio" lt rgb "magenta" lw 3

set title "Número de empresas ativas"
plot '../Dados/mercado.csv' using 1:14 with lines title "Ativas" lt rgb "green" lw 3

set title "Salário improdutivo e produtivo"
plot '../Dados/mercado.csv' using 1:16 with lines title "Salário impr." lt rgb "red" lw 3, \
     '../Dados/mercado.csv' using 1:17 with lines title "Salário prod." lt rgb "blue" lw 3

unset multiplot