#!/bin/bash -ex

INFILE=$1
INNAME="${INFILE%.*}"
EIGHTK="$INNAME-1.8.wav"
EIGHTKMOD="$INNAME-1.8-mod.wav"
SIXTEENK="$INNAME-16.wav"
NOISE="$INNAME-noise.wav"
FINAL="$INNAME.f.wav"

sox ${INFILE} -b 8 ${EIGHTK} rate 8k channels 1

./sndmanip -k -i ${EIGHTK} -o ${EIGHTKMOD}

#~ ./sndmanip -r -i ${EIGHTK} -o r.${EIGHTKMOD}
#~ ./sndmanip -k -i ${EIGHTK} -o k.${EIGHTKMOD}
#~ ./sndmanip -p -i ${EIGHTK} -o p.${EIGHTKMOD}
#~ ./sndmanip -c -i ${EIGHTK} -o c.${EIGHTKMOD}
#~ ./sndmanip -rp -i ${EIGHTK} -o rp.${EIGHTKMOD}
#~ ./sndmanip -rc -i ${EIGHTK} -o rc.${EIGHTKMOD}
#~ ./sndmanip -kr -i ${EIGHTK} -o kr.${EIGHTKMOD}
#~ ./sndmanip -kp -i ${EIGHTK} -o kp.${EIGHTKMOD}
#~ ./sndmanip -kc -i ${EIGHTK} -o kc.${EIGHTKMOD}
#~ ./sndmanip -pc -i ${EIGHTK} -o pc.${EIGHTKMOD}
#~ ./sndmanip -kpr -i ${EIGHTK} -o kpr.${EIGHTKMOD}
#~ ./sndmanip -kpc -i ${EIGHTK} -o kpc.${EIGHTKMOD}
#~ ./sndmanip -kprc -i ${EIGHTK} -o kprc.${EIGHTKMOD}

sox ${EIGHTKMOD} -b 16 ${SIXTEENK} norm
sox ${SIXTEENK} ${NOISE} trim 0 1
sox ${NOISE} -n noiseprof ${NOISE}.prof
sox ${SIXTEENK} ${FINAL} noisered ${NOISE}.prof .2

rm -rf ${EIGHTK} ${EIGHTKMOD} ${SIXTEENK} ${NOISE} ${NOISE}.prof
