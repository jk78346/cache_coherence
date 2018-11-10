#!bin/bash
# ===== configuration =====
GRID="0" # gird testing or fix number testing as above
CACHE_SIZE="8192"
CACHE_ASSOC="8"
BLK_SIZE="64"
PROC="4"
declare -a PROTOS=("0" "1" "2") # 0:MSI, 1:MESI, 2:Dragon
declare -a DATASET=("../trace/canneal.04t.debug" "../trace/canneal.04t.longTrace")
# =========================
declare -a SIZES=("262144" "524288" "1048576" "2097152") # 256KB, 512KB, 1MB, 2MB @ 8-way, 64B
declare -a ASSOCS=("4" "8" "16") # 4-way, 8-way, 16-way @ 1MB, 64B
declare -a BLKS=("64" "128" "256") # 64B, 128B, 256B @ 1MB, 8-way
# =========================
# INPUTFILE="../trace/canneal.04t.longTrace"
# INPUTFILE="../trace/canneal.04t.debug"
make clean
make
make clobber
# Cache size: vary from 256KB, 512KB, 1MB, 2MB while keeping the cache associativity at 8 and block size at 64B.
# Cache associativity: vary from 4‐way, 8‐way, and 16‐way while keeping the cache size at 1MB and block size at 64B.
# Cache block size: vary from 64B, 128B, and 256B, while keeping the cache size at 1MB and cache associativity at 8 way.
execute(){
	for PROTO in ${PROTOS[@]}  #"2" #"0" "1"
	do
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
		for INPUTFILE in  ${DATASET[@]}
		do
			INPUTTYPE=$(echo $INPUTFILE| cut -d'.' -f 5) # get substring 
			if [ "$INPUTFILE" = "../trace/canneal.04t.longTrace" ]
			then
				INPUTTYPE=$(echo $INPUTTYPE| cut -d'T' -f 1)
			fi
			if [ "$CACHE_SIZE" = "8192" ]
			then
				SIZE_NAME="8KB"
			elif [ "$CACHE_SIZE" = "262144" ]
			then
				SIZE_NAME="256KB"
			elif [ "$CACHE_SIZE" = "524288" ]
			then
				SIZE_NAME="512KB"
			elif [ "$CACHE_SIZE" = "1048576" ]
			then
				SIZE_NAME="1MB"
			elif [ "$CACHE_SIZE" = "2097152" ]
			then
				SIZE_NAME="2MB"
			fi
			OUTPUT_NAME="${PROTO_NAME}_${INPUTTYPE}_${SIZE_NAME}_${CACHE_ASSOC}way_${BLK_SIZE}B.txt"
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
			if [ "$GRID" = "0" ] # do fix testing
			then
				diff $VALFILE $OUTPUTFILE >> $DIFFFILE
				diff $DIFFFILE "../result/pass.txt"
				if [ $? -eq 0 ]
				then 
					echo "pass on ${PROTO_NAME} with ${INPUTTYPE}"
					rm $DIFFFILE
				else
					echo "fail on ${PROTO_NAME} with ${INPUTTYPE}"
				fi
			fi
		done
	done
}

PARA_RESET(){
	CACHE_SIZE="1048576"
	CACHE_ASSOC="8"
	BLK_SIZE="64"
}

echo "===== auto diff result: ====="
if [ "$GRID" = "0" ]
then
	execute
else
	PARA_RESET
	for CACHE_SIZE in ${SIZES[@]}
	do
		execute	
	done

	PARA_RESET
	for CACHE_ASSOC in ${ASSOCS[@]}
	do
		execute
	done

	PARA_RESET
	for BLK_SIZE in ${BLKS[@]}
	do
		execute
	done
fi