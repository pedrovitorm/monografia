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
    plot '../Dados/mercado.csv' using 1:2 with lines title "Salario Medio" lt rgb "blue", \
         '../Dados/mercado.csv' using 1:3 with lines title "Preco Medio" lt rgb "red", \
         '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
         '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
         '../Dados/mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange", \
         '../Dados/mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown", \
         #'../Dados/mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan", \
         #'../Dados/mercado.csv' using 1:9 with lines title "Capital Medio" lt rgb "magenta", \
         '../Dados/mercado.csv' using 1:10 with lines title "Riqueza Media" lt rgb "yellow"
    pause 0.05
    i = i + 1
}

# Após terminar, sai do Gnuplot
exit
