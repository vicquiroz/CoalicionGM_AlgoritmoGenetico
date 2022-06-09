#/bin/bash
shopt -s expand_aliases
source ~/.bashrc
#alias jq=./jq-win64.exe

m=(20 30 40 50 60)
pTresh=(0.1 0.15 0.2 0.25 0.3)
pr=(0.01 0.05 0.1 0.15 0.2)

vot=votacion22105

let iter=1
cd "./out/Resultados"
for repeat in {1..5}
do 
    for m_val in "${m[@]}"
    do
        for pTresh_val in "${pTresh[@]}"
        do
            for pr_val in "${pr[@]}"
            do
                echo "---------######## $iter/625 ######----------"
                m=$(jq ".m" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json") 
                pt=$(jq ".pmutacion_threshold" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json") 
                p=$(jq ".pr" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json") 
                seed=$(jq ".seed" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json") 
                ite=$(jq ".numero_de_iteraciones" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json") 
                fit=$(jq ".fitness" "./$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json")
                echo "$m, $pt, $p, $seed, $ite, $fit" >> prueba.csv
                #cp hist.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
                ((iter+=1))
            done
        done
        
    done
done
