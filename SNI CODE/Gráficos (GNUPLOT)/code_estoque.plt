set datafile separator ","
set key below
set xlabel "Itr." font "Arial,12," offset -50,1
set ylabel "Valores" font "Arial,12," offset 2,0
set key font "Arial,12,"
set xtics font "Arial,12,"
set ytics font "Arial,12,"
set autoscale
set grid

set terminal png size 1000, 250
set output 'grafico_estoque.png'
set multiplot layout 1, 1

set title "Estoques"
plot '../Dados/mercado.csv' using 1:8 with lines title "Estoques" lt rgb "brown" lw 3

unset multiplot