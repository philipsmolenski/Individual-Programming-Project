program=$1
input=$2
num=$3

cat $input | grep -P "[^0-9>\s]" > /dev/null
if [ $? = 0 ]; then
	echo "ERROR"
	exit
fi;

tab=($((echo "NEW base"; cat $input; echo ?$num) | $program 2> /dev/null))
if [ $? = 1 ]; then
	echo "ERROR"
	exit
fi;

idx=0
for i in `(echo "NEW base"; cat $input; echo ; printf "%s? " "${tab[@]}") | $program`; do
	if [ $i = $num ]; then
		echo "${tab[$idx]}"
	fi;
	((idx++))
done; 