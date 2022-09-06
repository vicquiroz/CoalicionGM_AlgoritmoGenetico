#!/bin/bash
shopt -s expand_aliases
source ~/.bashrc
#alias jq=./jq-win64.exe

vot=votacion22105
let seed=1234
let iter=1
cd "./out/ResultadosVerificacion"
for repeat in {1..50}
do 
    
    echo "---------######## $iter/50 ######----------"
    m=$(jq ".m" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    pt=$(jq ".pmutacion_threshold" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    p=$(jq ".pr" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    seed=$(jq ".seed" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    ite=$(jq ".numero_de_iteraciones" "./$vot/resultados_rep_${repeat}_seed_${seed}.json") 
    fit=$(jq ".fitness" "./$vot/resultados_rep_${repeat}_seed_${seed}.json")
    echo "$m, $pt, $p, $seed, $ite, $fit" >> valores_verif.csv
    #cp hist.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
    ((seed+=$repeat*2))
    ((iter+=1))
done