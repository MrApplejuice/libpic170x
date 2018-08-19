#!/bin/bash

frequencies=( 32000000 16000000 8000000 4000000 2000000 1000000 500000 )
chips=( 16F1705 16LF1705 16F1709 16LF1709 )

for freq in ${frequencies[@]}; do
  for chip in ${chips[@]}; do
    echo "Building for ${chip} @ ${freq} Hertz"
    chip=$chip xtal_freq=$freq make &
  done
done

wait

make doc
