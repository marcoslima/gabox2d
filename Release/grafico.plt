reset;
set terminal jpeg font arial 26 size 1400,750; 

set grid xtics x2tics ytics y2tics
show label;

set xlabel "Geração";
set ylabel "Pontuação";
set output "dados.jpg"
plot	"GA_20100911043027_log.dat" with lines title  "pts"
