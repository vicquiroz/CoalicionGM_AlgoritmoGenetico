m=(20 30 40 50 60)
pTresh=(0.1 0.15 0.2 0.25 0.3)
pr=(0.01 0.05 0.1 0.15 0.2)

vot=votacion22105
cd out/

mkdir -p "Resultados/$vot"
cd build/x64-release
let seed=1234
let iter=1
for repeat in {1..5}
do 
    for m_val in "${m[@]}"
    do
        mkdir -p "../../Resultados/$vot/m_$m_val"
        for pTresh_val in "${pTresh[@]}"
        do
            mkdir -p "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val"
            for pr_val in "${pr[@]}"
            do
                mkdir -p "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val"
                echo "---------######## $iter/625 ######----------"
                ((seed+=$repeat*2))
                ./algortimoc++.exe $m_val $pTresh_val $pr_val $seed || echo "---------######## error en:  m = $m_val, pTresh = $pTresh_val, pr = $pr_val, seed= $seed donde $repeat de 5 ######----------"
                cp resultados.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/resultados_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
                cp hist.json "../../Resultados/$vot/m_$m_val/pTresh_$pTresh_val/pr_$pr_val/histograma_rep_${repeat}_m_${m_val}_pTresh_${pTresh_val}_pr_${pr_val}.json" 
                rm resultados.json
                rm hist.json
                ((iter+=1))
            done
        done
        
    done
done
echo "---------######## Completado hay que deletear los archivos de hist y resultados ######----------"