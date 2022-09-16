#!/bin/bash
shopt -s expand_aliases
source ~/.bashrc
#alias jq=./jq-win64.exe

vot=votacionRH0941234
let seed=1234
let iter=1
cd "Resultados"
for repeat in {1..10000}
do 
    ((seed+=$repeat*2))
    if((iter>5000)) && ((iter<=6000))
    then
    echo "---------######## $iter/10000 ######----------"
    seed=$(jq ".seed" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    ite=$(jq ".numero_de_iteraciones" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    fit=$(jq ".fitness" "./$vot/resultados_rep_${repeat}_seed_${seed}.json")
    tie=$(jq ".tiempo" "./$vot/resultados_rep_${repeat}_seed_${seed}.json")
    echo "$seed, $ite, $fit, $tie" >> valores_verif_votacionRH0941234_6.csv
    fi
    ((iter+=1))
done