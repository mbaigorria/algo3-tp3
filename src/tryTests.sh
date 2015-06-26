#!/bin/bash

make > /dev/null

for i in $( ls tests ); do
	echo ---------------------
	echo Testing $i
	echo "backtracking $(./backtracking/backtracking < tests/$i)"
	echo "greedy       $(./greedy/greedy < tests/$i)"
	echo "local        $(./local/local < tests/$i)"
	echo "grasp        $(./grasp/grasp < tests/$i)"
done

echo ---------------------