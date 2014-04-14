#!/bin/bash

RESULTS=results.txt
LOGS=logs
PLAYER_FILE=$1
ENGINE=$2
shift
shift
ENGINE_PARAMS=$@

function check
{
	P0_BASE=`basename $1`
	P1_BASE=`basename $2`

	LOGFILE="$LOGS/${P0_BASE}_vs_${P1_BASE}.log"

	if [ -e $LOGFILE ];
	then
		echo "$LOGFILE exists. Skipping..."
		return
	fi
	$ENGINE --player0 $1 --player1 $2 ${ENGINE_PARAMS} 2>&1 | tee $LOGFILE
	RES=${PIPESTATUS[0]}
	if [ "$RES" -eq 0 ];
	then
		P0_PTS="1"
		P1_PTS="1"
	elif [ "$RES" -eq 1 ];
	then
		P0_PTS="3"
		P1_PTS="0"
	else
		P0_PTS="0"
		P1_PTS="3"
	fi
	echo "$P0_BASE, $P1_BASE, $P0_PTS, $P1_PTS" >> $RESULTS
}

if [[ ! -e $PLAYER_FILE ]];
then
	echo "Couldn't find player list in $PLAYER_FILE"
	exit 1
fi

if [[ ! -e $ENGINE ]];
then
	echo "Couldn't find engine - $ENGINE"
	exit 1
fi

mkdir -p $LOGS

PLAYERS_1=`cat $PLAYER_FILE | sort -R` 
PLAYERS_2=`cat $PLAYER_FILE | sort -R`

for player in $PLAYERS_1;
do
	if [ ! -e $player ];
	then
		echo "Couldn't find player - $player"
		exit 2
	fi
done


for P1 in $PLAYERS_1;
do
	for P2 in $PLAYERS_2;
	do
		if [[ "$P1" = "$P2" ]];
		then
			continue
		fi
		if [[ "$P1" > "$P2" ]];
		then
			continue
		fi
		check $P1 $P2
		check $P2 $P1
	done
done
