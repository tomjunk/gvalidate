#!/bin/bash

# this has hardcoded versions in it -- may want to put these in as arguments

gv1=v3_04_02a
gv2=v3_06_02a
dir1=validate_${gv1}
dir2=validate_${gv2}

dirname=compare_${gv1}_${gv2}
touch ${dirname}
rm -rf ${dirname}
mkdir ${dirname}

compare_single_sample() {
  fn1=${dir1}/genie_$1_${energy}_${gv1}.root
  fn2=${dir2}/genie_$1_${energy}_${gv2}.root
  fnc=${dirname}/genie_$1_${energy}_${gv1}_${gv2}_compare.pdf
root -b -l <<EOF
.x compareHists.C("${fn1}","${fn2}","${fnc}");
.q
EOF
}

for energy in 0.4 0.6 0.8 1.0 1.4 1.6 1.8 2.0 2.5 3.0 3.5 4.0; do
    compare_single_sample numu
    compare_single_sample numubar 
    compare_single_sample nue 
    compare_single_sample nuebar 
done
