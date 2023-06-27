#GNUPLOT plotting

#set term corel
set autoscale
set multiplot layout 3,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Voice"
#unset label

plot 'response.dat' using 1:2 with lines title 'Notch 300Hz',\
    'response.dat' using 1:3 with lines title 'Notch 1200Hz'
set xlabel 'Frecuencia (Hz)'
set ylabel 'Magnitud'

unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "Voz con tonos"
#unset label
set xlabel   "w"
set ylabel   "magnitud"
plot  "vt.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "Voz filtrada"
#unset label
set xlabel   "w"
set ylabel   "magnitud"
plot  "output_signal.dat"
unset xlabel
unset ylabel
unset title


#