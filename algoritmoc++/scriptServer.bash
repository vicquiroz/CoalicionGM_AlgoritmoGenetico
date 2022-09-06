#!/bin/bash
m=(50)
pTresh=(0.40)
pr=(0.19)

vot=votacionRH0750158
mkdir -p "Resultados/$vot"
let seed=1234
let iter=1
for repeat in {1..1000}
do 
    for m_val in "${m[@]}"
    do
        mkdir -p "Resultados/$vot/m_$m_val"
        for pTresh_val in "${pTresh[@]}"
        do
            mkdir -p "Resultados/$vot/m_$m_val/pTresh_$pTresh_val"
            for pr_val in "${pr[@]}"
            do
                mkdir -p "Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val"
                echo "-# $iter/1000 #-"
                ((seed+=$repeat*2))
                ./algortimoc++ $m_val $pTresh_val $pr_val $seed || touch "Resultados/$vot/ERRORseed=$seed_iter=$iter.txt"
                cp resultados.json "Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_seed_$seed.json" 
                cp hist.json "Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_rep_${repeat}_seed_$seed.json" 
                rm resultados.json
                rm hist.json
                ((iter+=1))
            done
        done
    done
done
echo "---------######## Completado! ######----------"
