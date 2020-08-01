#!/bin/bash -ex

INFILE=$1
INNAME="${INFILE%.*}"
EIGHTK="$INNAME-1.8.wav"
EIGHTKMOD="$INNAME-1.8-mod.wav"
SIXTEENK="$INNAME-16.wav"
NOISE="$INNAME-noise.wav"
FINAL="$INNAME.f.wav"

sox ${INFILE} -b 8 ${EIGHTK} rate 8k channels 1

./sndmanip -p -i ${EIGHTK} -o ${EIGHTKMOD}

FLAGS="
r   
k   
p   
c   
m 
rp  
rc  
rm 
kr  
kp  
kc 
km 
pc  
pm 
kpr 
kprm 
kpc 
kpcm 
kprc
kprcm
"

for FLAG in ${FLAGS}; do
	./sndmanip -${FLAG} -i ${EIGHTK} -o ${FLAG}.${EIGHTKMOD}
	sox ${FLAG}.${EIGHTKMOD} -b 16 ${FLAG}.${SIXTEENK} norm
	sox ${FLAG}.${SIXTEENK} ${FLAG}.${NOISE} trim 0 1
	sox ${FLAG}.${NOISE} -n noiseprof ${FLAG}.${NOISE}.prof
	sox ${FLAG}.${SIXTEENK} ${FLAG}.${FINAL} noisered ${FLAG}.${NOISE}.prof .2
	rm -rf ${FLAG}.${EIGHTKMOD} ${FLAG}.${SIXTEENK} ${FLAG}.${NOISE} ${FLAG}.${NOISE}.prof
done

#~ rm -rf ${EIGHTK}
