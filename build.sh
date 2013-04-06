#http://stackoverflow.com/questions/4778389/makefile-automatically-setting-jobs-j-flag-for-a-multicore-machine
NUMCPUS=`grep -c '^processor' /proc/cpuinfo`
time make -j$NUMCPUS
