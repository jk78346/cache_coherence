#! bin/sh
# ===== configuration =====
CACHE_SIZE="8192"
CACHE_ASSOC="8"
BLK_SIZE="64"
PROC="4"
PROTO="1" # 0:MSI, 1:MESI, 2:Dragon
# INPUTFILE="../trace/canneal.04t.longTrace"
# INPUTFILE="../trace/canneal.04t.debug"
# =========================
if [ "$PROTO" = "0" ]
then
	PROTO_NAME="MSI"
elif [ "$PROTO" = "1" ]
then
	PROTO_NAME="MESI"
else
	PROTO_NAME="Dragon"
fi
# =========================
make clean
make
make clobber
echo "===== auto diff result: ====="
for INPUTFILE in "../trace/canneal.04t.debug" "../trace/canneal.04t.longTrace"
do
	INPUTTYPE=$(echo $INPUTFILE| cut -d'.' -f 5) # get substring 
	if [ "$INPUTFILE" = "../trace/canneal.04t.longTrace" ]
	then
		INPUTTYPE=$(echo $INPUTTYPE| cut -d'T' -f 1)
	fi
	OUTPUT_NAME="${PROTO_NAME}_${INPUTTYPE}.txt"
	OUTPUTFILE="../result/${OUTPUT_NAME}"
	DIFFFILE="../result/error.txt"
	VAL_NAME="${PROTO_NAME}_${INPUTTYPE}.val"
	VALFILE="../val.v2/${VAL_NAME}"
	if [ -f $OUTPUTFILE ] # delete the output file first if it already exist
	then
		rm $OUTPUTFILE
	fi
	./smp_cache $CACHE_SIZE $CACHE_ASSOC $BLK_SIZE $PROC $PROTO $INPUTFILE >> $OUTPUTFILE 2>&1
	if [ -f $DIFFFILE ] # delete the tmp file first if it already exist
	then
		rm $DIFFFILE
	fi
	diff $VALFILE $OUTPUTFILE >> $DIFFFILE
	diff $DIFFFILE "../result/pass.txt"
	if [ $? -eq 0 ]
	then 
		echo "pass on ${PROTO_NAME} with ${INPUTTYPE}"
		rm $DIFFFILE
	else
		echo "fail on ${PROTO_NAME} with ${INPUTTYPE}"
	fi
done