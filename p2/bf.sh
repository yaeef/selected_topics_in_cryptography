#!/bin/bash

# Función para ejecutar la búsqueda
run_search() {
    local alumno=$1
    local p=$2
    local a=$3
    local b=$4
    local qx=$5
    local qy=$6
    local gx=$7
    local gy=$8

    echo "-------------------------------------------------------"
    echo "Analizando Alumno: $alumno"
    echo "Ejecutando: ./practica -2 -a $a -b $b -p $p -x $qx -y $qy -z 1 -X $gx -Y $gy -Z 1"
    
    # Ejecución del binario
    ./practica -2 -a "$a" -b "$b" -p "$p" -x "$qx" -y "$qy" -z 1 -X "$gx" -Y "$gy" -Z 1
    
    echo -e "-------------------------------------------------------\n"
}

# --- LISTA DE EJEMPLOS RÁPIDOS (P < 10,000) ---

run_search "CAMPOS NAVA KEVIN EDUARDO" 4111 4108 3 1864 2844 1232 3963
run_search "CRISOSTOMO AGULLAR RICARDO" 524287 524284 8 380433 213685 497927 502736
run_search "DE LA PEÑA SALAZAR IAN RAFAEL" 1009 1006 8 638 53 436 973
run_search "FLORES PATIÑO ITZEL" 2039 2036 21 1877 1332 1183 965
run_search "GASCA FRAGOSO PEDRO" 61001 60998 19 29888 9161 42763 56247
run_search "GILES MACIAS ALEXIS" 1009 1006 4 44 528 256 155
run_search "GONZALEZ CRUZ JOSHUA RAUL" 524287 524284 504 168065 517006 341005 411111
run_search "GUERRERO SANCHEZ DIEGO ISAAC" 1013 1010 9 455 276 93 784
run_search "GUEVARA BADILLO ARELI ALEJANDRA" 16001 15998 36 5119 5678 4971 2914
run_search "HERNANDEZ ZAMORA ALEJANDRO" 1009 1006 8 793 222 436 973
run_search "LOPEZ REYES JOSE ROBERTO" 16001 15998 11 15049 8690 3041 8982
run_search "MORA SANTILLAN RICARDO" 131071 131068 1005 45216 122150 20473 81394
run_search "MORALES RAMOS IVAR ALEJANDRO" 1013 1010 36 183 856 57 883
run_search "MORENO JARAMILLO JUAN PABLO" 1009 1006 4 993 899 256 155
run_search "NUÑEZ MARTINEZ MIGUEL ANGEL" 7027 7024 1004 2109 4901 2367 1315
run_search "ORDUÑA ESTRADA VICTOR MANUEL" 1009 1006 4 550 155 256 155
run_search "PATLAN GUALO LUIS EDUARDO" 1009 1006 4 308 941 256 155
run_search "PEREZ ORTIZ ERICK JULIAN" 16001 15998 36 12494 5428 4971 2914
run_search "RAMIREZ MARTINEZ ALEJANDRO" 16001 15998 11 8355 2169 3041 8982
run_search "VITALES HERRERA HATZIRY" 2039 2036 21 464 1541 1183 965
