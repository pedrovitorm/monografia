set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Salário Produtivo e Salário Improdutivo"
set terminal wxt size 1200, 800

plot '../Dados/mercado.csv' using 1:16 with lines title "Salario Improdutivo" lt rgb "red", \
     '../Dados/mercado.csv' using 1:17 with lines title "Salario Produtivo" lt rgb "blue"
