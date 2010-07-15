#!/bin/sh
##
## emacs manyrun.sh
## 
## Made by bengreve
## Login   <bengreve@confiance.imag.fr>
## 
## Started on  Thu Jul 15 10:45:33 2010 bengreve
## Last update Thu Jul 15 10:45:33 2010 bengreve
##

#!/bin/sh

if [ $# -ne 2 ]
then
  echo "Usage: `basename $0` <nb_runs_per_nb_threads> <max_nb_threads>"
  exit $E_BADARGS; 
fi

for j in `seq 1 $2`; do 
    echo "Running $1 run with $j threads."
    for i in `seq 1 $1`; 
    do 
	./tuplespace_multi $j || break ;
    done 
    if [ $i -ne $1 ]; then
	exit 1; 
    fi
done;

