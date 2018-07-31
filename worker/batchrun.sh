export LD_LIBRARY_PATH=../engine 
trap ctrl_c INT

function ctrl_c() {
	killall main;
	echo 'killed';
	exit
}

for ((i=0;i<$1;++i)) 
do
	bin/main -w 4 -p 90$(printf "%02d" $i) -h e1.sc.team -q 2333 2>&1 | tee $i.log &
done

while true
do
	sleep 1000
done
