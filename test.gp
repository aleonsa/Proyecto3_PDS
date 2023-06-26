#GNUPLOT plotting

#set term corel
set autoscale
set multiplot layout 2,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Voice"
#unset label
set xlabel   "w"
set ylabel   "magnitud"
plot  "filtro.dat"
unset xlabel
unset ylabel
unset title

#------------- Graf 2 ----------------
set title "Voice"
#unset label
set xlabel   "w"
set ylabel   "magnitud"
plot  "espec.dat"
unset xlabel
unset ylabel
unset title


#