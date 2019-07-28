program=$1
tests="$2/"
valgrind=false

loop_pattern="$tests*.in"

leaks=()
goodans=()
wrong_out=()
wrong_err=()
out_of_memory=()

for i in $loop_pattern; do
	helper="${i%.in}"
	input="${helper#$tests}"

	echo
	echo "Sprawdzanie testu $input"
	echo

	if $valgrind; then
		valgrind --error-exitcode=69 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --log-file="/dev/null" ./$program < $i > /tmp/out.out 2> /tmp/out.err
	else
		./$program < $i > /tmp/out.out 2> /tmp/out.err
	fi

	exit_code=$?

	if [ $exit_code == 69 ]
		then 
		echo "Wycieki pamięci w $input"
		leaks+=($input)
		echo
		continue
	elif [ $exit_code == 1 ]
		then
		echo "Przekroczony limit pamięci w $input"
		out_of_memory+=($input)
		echo
		continue 
	elif [ $exit_code == 0 ]
		then 
		if diff ${i%in}out /tmp/out.out > /dev/null
			then
			echo "Poprawne wyjście standardowe w $input"
		else 
			echo "Niepoprawne wyjście standardowe w $input"
			wrong_out+=($input)
			echo
			continue
		fi 
		
		if diff ${i%in}err /tmp/out.err > /dev/null
			then
			echo "Poprawne wyjście diagnostyczne w $input"
			goodans+=($input)
			continue
		else
			echo "Niepoprawne wyjście diagnostyczne w $input"
			wrong_err+=($input)
			echo
			continue
		fi
	fi
done;

echo
echo "Poprawne odpowiedzi w: ${goodans[@]}"
echo 
echo "Niepoprawne wyjście standardowe w: ${wrong_out[@]}"
echo
echo "Niepoprawne wyjście diagnostyczne w: ${wrong_err[@]}"
echo 
echo "Przekroczony limit pamięci w: ${out_of_memory[@]}"
echo
echo "Wycieki pamięci w: ${leaks[@]}"