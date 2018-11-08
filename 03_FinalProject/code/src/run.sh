#! bin/sh
CACHE_SIZE="8192"
CACHE_ASSOC="8"
BLK_SIZE="64"
PROC="4"
PROTO="0" # 0:MSI, 1:MESI, 2:Dragon
INPUTFILE="../trace/canneal.04t.longTrace"
# INPUTFILE="../trace/canneal.04t.debug"
make clean
make
make clobber
./smp_cache $CACHE_SIZE $CACHE_ASSOC $BLK_SIZE $PROC $PROTO $INPUTFILE
