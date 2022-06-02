coolingrates=(0.98 0.989 0.998)
temp_inital=(100000 1000 10)

m=(30 35 40 45 50)
pTresh=(0.1 0.2 0.3 0.4 0.5)
pr=(0.1 0.2 0.3 0.4 0.5)

vot=votacion31466


mkdir -p "Resultados/$vot"
cd build/x64-debug
let seed=1234
for repeat in {1..5}
do 
    for m_val in "${m[@]}"
    do
        mkdir "../../Resultados/$vot/m_$m_val"
        for pTresh_val in "${pTresh[@]}"
        do
            mkdir "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val"
            for pr_val in "${pr[@]}"
            do
                mkdir "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val"
                echo "---------######## m = $m_val, pTresh = $pTresh_val, pr = $pr_val donde $repeat de 5 ######----------"
                ((seed+=$repeat*2))
                ./algortimoc++.exe $m_val $pTresh_val $pr_val $seed || cat "$seed.txt" "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_m_${m_val}_pTresh_${pTresh_val}_pr_" "${seed}.txt" 
                cp resultados.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
                cp hist.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
                del resultados.json
                del hist.json
            done
        done
        
    done
done
echo "---------######## Completado hay que deletear los archivos de hist y resultados ######----------"