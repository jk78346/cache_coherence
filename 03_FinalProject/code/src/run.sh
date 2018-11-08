#! bin/sh
CACHE_SIZE="4096"
CACHE_ASSOC="2"
BLK_SIZE="128"
PROC="4"
PROTO="0" # 0:MSI, 1:MESI, 2:Dragon
INPUTFILE="../trace/canneal.04t.debug"
make clean
make
make clobber
./smp_cache $CACHE_SIZE $CACHE_ASSOC $BLK_SIZE $PROC $PROTO $INPUTFILE
