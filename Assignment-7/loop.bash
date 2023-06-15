#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p classmpitest
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o autocorr.out
#SBATCH -e autocorr.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=beemires@oregonstate.edu
module load openmpi


for n in 1 2 4 8
do
   echo "Running with $n processors"
   mpic++ proj07.cpp -o proj07 -lm
   mpiexec -mca btl self,tcp -np $n ./proj07
done