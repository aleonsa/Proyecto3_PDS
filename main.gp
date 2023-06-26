#GNUPLOT plotting

#set term corel
set autoscale
set multiplot layout 3,1 rowsfirst
set grid
set style data lines

#------------- Graf 1 ----------------
set title "Voice"
#unset label
set xlabel   "n"
set ylabel   "v(n)"
plot  "v.dat"
unset xlabel
unset ylabel
unset title

# #------------- Graf 2 ----------------
# set title "Voice Spectrum"
# # unset label
# set xlabel   "w"
# set ylabel   "|v|"
# plot  "v_spec.dat"
# unset xlabel
# unset ylabel
# unset title

# unset xlabel
# unset ylabel
# unset title

#------------- Graf 3 ----------------
set title "Voice + Tones"
#unset label
set xlabel   "n"
set ylabel   "vt(n)"
plot  "vt.dat"
unset xlabel
unset ylabel
unset title

# #------------- Graf 4 ----------------
# set title "Voice + Tones Spectrum"
# # unset label
# set xlabel   "w"
# set ylabel   "|vt|"
# plot  "vt_spec.dat"
# unset xlabel
# unset ylabel
# unset title

# unset xlabel
# unset ylabel
# unset title

#------------- Graf 1 ----------------
set title "Filtered Voice"
#unset label
set xlabel   "n"
set ylabel   "vf(n)"
plot  "vf.dat"
unset xlabel
unset ylabel
unset title

# #------------- Graf 2 ----------------
# set title "Filtered Voice Spectrum"
# # unset label
# set xlabel   "w"
# set ylabel   "|vf|"
# plot  "vf_spec.dat"
# unset xlabel
# unset ylabel
# unset title

# unset xlabel
# unset ylabel
# unset title