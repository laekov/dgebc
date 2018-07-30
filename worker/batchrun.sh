export LD_LIBRARY_PATH=../engine 
trap ctrl_c INT

for ((i=0;i<$1;++i)) 
do
	bin/main -w 5 -p 90$(printf "%02d" $i) -h e1.sc.team -q 2333 2>&1 | tee $i.log &
done

function ctrl_c() {
	killall main;
	echo 'killed';
	exit
}
