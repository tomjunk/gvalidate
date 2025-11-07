#!/bin/bash

# script takes one argument, the dunesw version.  It cleans out and makes a new subdirectory called validate_${GENIE_VERSION}

# run in an SL7 container.  You may need to adjust the mounted volumes
# to match what's available on your system.  I ran it on a system with
# a /data volume.

if [[ `grep PRETTY /etc/os-release | grep "Scientific Linux 7"`x = x ]]; then
    echo "Need SL7 -- starting a container with apptainer"
    /cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer run -B /cvmfs,/data /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest $0 $@
    exit $?
fi
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup dunesw $1 -q e26:prof

# runs 100k genie events 
dirname=validate_${GENIE_VERSION}
touch ${dirname}
rm -rf ${dirname}
mkdir ${dirname}
cp rootracker.* ${dirname}
cd ${dirname}

# first argument:  nue or numu or nuebar or numubar
# second argument:  pdg for nue (12) or numu (14) nuebar (-12) or numubar (-14)

run_single_sample() {
tmpfile=`mktemp genie_100k_$1.root.XXXXXX`
gevgen -n 100000 -p $2 -t 1000180400 -e ${energy} --tune AR23_20i_00_000 --cross-sections $GENIEXSECFILE -o ${tmpfile} >& /dev/null
gntpc -i ${tmpfile} -o ${tmpfile}_rootracker.root -f rootracker >& /dev/null
root -b -l <<EOF
.L rootracker.C
rootracker t("${tmpfile}_rootracker.root");
t.Loop(${energy},"genie_$1_${energy}_${GENIE_VERSION}.root");
.q
EOF
rm ${tmpfile}
rm ${tmpfile}_rootracker.root
}

for energy in 0.4 0.6 0.8 1.0 1.4 1.6 1.8 2.0 2.5 3.0 3.5 4.0; do
    run_single_sample numu 14 &
    run_single_sample numubar -14 &
    run_single_sample nue 12 &
    run_single_sample nuebar -12 &
    wait
done
