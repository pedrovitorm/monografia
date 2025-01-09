# Configurações gerais
set datafile separator ","
set key outside
set xlabel "Iteracao"
set ylabel "Valores"
set autoscale
set grid
set title "Graficos em Tempo Real"

# Ajuste do tamanho da janela
set terminal wxt size 1200, 800  # Largura e altura em pixels
set multiplot layout 5, 2 title "Análise das Variáveis em Tempo Real"  # 3 linhas, 2 colunas

# Gráfico 1: Variáveis principais - Preço Médio
set title "Preco medio"
plot '../Dados/mercado.csv' using 1:3 with lines title "Preco medio" lt rgb "red"

# Gráfico 2: Producao e Consumo
set title "Produção e Consumo"
plot '../Dados/mercado.csv' using 1:4 with lines title "Produzidos" lt rgb "green", \
     '../Dados/mercado.csv' using 1:5 with lines title "Consumidos" lt rgb "purple", \
     '../Dados/mercado.csv' using 1:15 with lines title "Prod_total" lt rgb "red"

# Gráfico 3: Contratações e Demissões
set title "Taxa de desemprego"
plot '../Dados/mercado.csv' using 1:12 with lines title "desemprego %" lt rgb "orange"

# Gráfico 4: Estoques
set title "Estoques"
plot '../Dados/mercado.csv' using 1:8 with lines title "Estoques" lt rgb "cyan"

# Gráfico 5: Riqueza Média
set title "Riqueza media"
plot '../Dados/mercado.csv' using 1:10 with lines title "Riqueza media" lt rgb "yellow"

# Gráfico 6: Variável Customizada (Exemplo)
set title "Capital medio das empresas"
plot '../Dados/mercado.csv' using 1:9 with lines title "Capital medio" lt rgb "magenta"

# Gráfico 7: Variável Customizada (Exemplo)
set title "Numero de empresas ativas"
plot '../Dados/mercado.csv' using 1:14 with lines title "ativas" lt rgb "green"

# Gráfico 8: Variável Customizada (Exemplo)
set title "Base monetaria"
plot '../Dados/mercado.csv' using 1:13 with lines title "Quantidade" lt rgb "blue"

# Gráfico 9: Variável Customizada (Exemplo)
set title "Salario improdutivo"
plot '../Dados/mercado.csv' using 1:16 with lines title "Salario" lt rgb "red"

# Gráfico 10: Variável Customizada (Exemplo)
set title "Salario produtivo"
plot '../Dados/mercado.csv' using 1:17 with lines title "Salario" lt rgb "blue"

unset multiplot
