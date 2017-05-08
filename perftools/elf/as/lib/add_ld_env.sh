# use source add_ld_env.sh to change the environment variable
# instead of using ./add_ld_env.sh. 

if [[ -n $LD_LIBRARY_PATH ]]; then
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/ws/elf/as/lib/
else
    export LD_LIBRARY_PATH=/mnt/ws/elf/as/lib
fi

echo $LD_LIBRARY_PATH

