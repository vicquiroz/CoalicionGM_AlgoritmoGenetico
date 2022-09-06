#!/bin/bash
#nuestros valores a verificar son:
# m=30
# pr=0.15
# p_tresh = 0.2
# y nuestro valor optimo es = 8621.89

vot=votacion22105
cd out/
mkdir -p "ResultadosVerificacion/$vot"
#para windows
cd build/x64-release
#para linux
#cd build/linux-debug/

let seed=1234
let iter=1

for repeat in {1..50}
do 
    echo "---------######## $iter/50 ######----------"
    ./algortimoc++.exe 30 0.2 0.15 $seed || echo "---------######## error en: seed= $seed donde $repeat de 50 ######----------"
    #para linux
    #./algortimoc++ 30 0.2 0.15 $seed || cat  "---------######## error en: seed= $seed donde $repeat de 50 ######----------"
    cp resultados.json "../../ResultadosVerificacion/$vot/resultados_rep_${repeat}_seed_${seed}.json" 
    cp hist.json "../../ResultadosVerificacion/$vot/histograma_rep_${repeat}_seed_${seed}.json" 
    rm resultados.json
    rm hist.json
    ((seed+=$repeat*2))
    ((iter+=1))
done