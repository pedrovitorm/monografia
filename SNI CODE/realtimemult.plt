set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set title "Dados em Tempo Real"
set autoscale
set grid

i = 0
max_iter = 500  # Definir número máximo de iterações

# Configuração para o layout de multiplot
set multiplot layout 2, 1 title "Gráficos em Tempo Real"  # 2 linhas, 1 coluna

# Gráfico 1 (com as variáveis principais)
set title "Variáveis principais"
plot 'mercado.csv' using 1:2 with lines title "Salario Medio" lt rgb "blue", \
     'mercado.csv' using 1:3 with lines title "Preco Medio" lt rgb "red", \
     'mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
     'mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
     'mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange", \
     'mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown"

# Gráfico 2 (com Estoques e Riqueza Média)
set title "Estoques e Riqueza Média"
plot 'mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan", \
     'mercado.csv' using 1:10 with lines title "Riqueza Media" lt rgb "yellow"

# Loop para atualizar os gráficos em tempo real
while (i < max_iter) {
    # Atualiza o gráfico 1 (com as variáveis principais)
    plot 'mercado.csv' using 1:2 with lines title "Salario Medio" lt rgb "blue", \
         'mercado.csv' using 1:3 with lines title "Preco Medio" lt rgb "red", \
         'mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
         'mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
         'mercado.csv' using 1:6 with lines title "Contratados" lt rgb "orange", \
         'mercado.csv' using 1:7 with lines title "Demitidos" lt rgb "brown"

    # Atualiza o gráfico 2 (com Estoques e Riqueza Média)
    plot 'mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan", \
         'mercado.csv' using 1:10 with lines title "Riqueza Media" lt rgb "yellow"

    pause 0.05
    i = i + 1
}

# Após terminar, sai do Gnuplot
unset multiplot
exit
