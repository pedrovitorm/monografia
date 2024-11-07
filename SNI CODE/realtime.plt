set datafile separator ","
while (1) {
    plot 'test.csv' using 1:2 with lines title "Seno"
    pause 0.2
}