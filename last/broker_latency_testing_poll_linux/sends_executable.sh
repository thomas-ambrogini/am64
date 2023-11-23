#!/bin/bash

source_dir="examples/broker_latency_testing_poll_linux/am64x-evm/"
destination_user="root"
destination_host="192.168.1.2"
destination_dir="/lib/firmware/pdk-ipc/broker_communication_linux_poll_testing/"
file_name="embedded_kafka.release.out"

# List of files to copy
files=("m4fss0-0_freertos/ti-arm-clang/" "r5fss0-0_freertos/ti-arm-clang/" "r5fss0-1_freertos/ti-arm-clang/" "r5fss1-0_freertos/ti-arm-clang/" "r5fss1-1_freertos/ti-arm-clang/")
files_name_destination=("m4_exec.out" "r500_exec.out" "r501_exec.out" "r510_exec.out" "r511_exec.out")

for ((i=0; i<${#files[@]}; i++)); do 
    file="${files[i]}"
    name_destination="${files_name_destination[i]}"
    scp "${source_dir}${file}${file_name}" "${destination_user}@${destination_host}:${destination_dir}${name_destination}"
done 

# for file in "${files[@]}"; do
#     scp "${source_dir}${file}${file_name}" "${destination_user}@${destination_host}:${destination_dir}"
# done 

if [ $? -eq 0 ]; then
    echo "Files copied successfully."
else
    echo "Error copyting files."
fi 
