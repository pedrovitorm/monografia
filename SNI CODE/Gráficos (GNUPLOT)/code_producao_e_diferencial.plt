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
set output 'grafico_producao_e_diferencial.png'
set multiplot layout 1, 2

set title "Produção e Consumo"
plot '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green" lw 3, \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple" lw 3, \
     '../Dados/mercado.csv' using 1:15 with lines title "Máximo" lt rgb "red" lw 3

set title "Salário improdutivo e produtivo"
plot '../Dados/mercado.csv' using 1:16 with lines title "Salário impr." lt rgb "red" lw 3, \
     '../Dados/mercado.csv' using 1:17 with lines title "Salário prod." lt rgb "blue" lw 3

unset multiplot