#!/bin/bash

# Nome do programa a ser executado
program="./TCPSender ../arquivo_100M.txt"

# Nome do arquivo de saída
output_file="output.txt"

# Executa o programa 10 vezes
for ((i=1; i<=10; i++))
do
    echo "Execução $i:"
    
    # Obtém o tempo de início em milissegundos
    start_time=$(date +%s%3N)

    # Executa o programa e redireciona a saída para um arquivo temporário e para o arquivo de saída
    $program > temp_output.txt 2>&1
    
    # Obtém o tempo de término em milissegundos
    end_time=$(date +%s%3N)

    # Calcula o tempo de execução em milissegundos
    execution_time=$((end_time - start_time))
    
    # Exibe o tempo de execução
    echo "Tempo de execução: $execution_time ms"
    
    # Escreve o tempo de execução no arquivo junto com a saída do programa
    echo -n "$execution_time, " >> "$output_file"
    #echo "" >> "$output_file"
    #echo "" >> "$output_file"
    cat temp_output.txt >> "$output_file"
    echo "" >> "$output_file"
    
    # Limpa o arquivo temporário
    rm temp_output.txt
done
