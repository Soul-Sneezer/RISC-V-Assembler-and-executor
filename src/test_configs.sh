#!/bin/bash

configs=("config1" "config2" "config3") 
tests=()

for config in "${configs[@]}"; do
    ./risk -config $config -o output_$config 
    size=$(stat -c%s "output_$config")
    echo "Size for $config: $size bytes"
done
