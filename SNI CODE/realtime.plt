set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set title "Dados em Tempo Real"
set autoscale
set grid

i = 0
max_iter = 100  # Definir número máximo de iterações

# Loop para atualizar o gráfico por um número definido de iterações
while (i < max_iter) {
    plot 'mercado.csv' using 1:2 with lines title "Salario Medio" lt rgb "blue", \
         'mercado.csv' using 1:3 with lines title "Preco Medio" lt rgb "red", \
         'mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
         'mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
         'mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange", \
         'mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown"
    pause 0.1
    i = i + 1
}

# Após terminar, sai do Gnuplot
exit
