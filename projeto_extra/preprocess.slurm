#!/bin/bash
#SBATCH --job-name=preprocess
#SBATCH --output=output_preprocess.txt
#SBATCH --nodes=2
#SBATCH --ntasks=4
#SBATCH --time=00:05:00
#SBATCH --mem=12000M

echo "[Pré-processamento]"
./preprocessing
echo -e ""
