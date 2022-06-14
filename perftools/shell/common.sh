#!/bin/bash

# include this in user
# bin_abs=$(realpath $0)
# bin_dir=$(dirname $bin_abs)
# . ${bin_dir}/common.sh

# if [ $# -ne 1 ] ; then echo "cmd ip" ; exit -1 ; fi
# if [ $? -ne 0 ] ; then echo "error"; exit -1 ; fi
# if [ $? -ne 0 ] ; then error "error"; fi
echo_date()
{
    echo "`date +%F_%H:%M:%S`# "
}

echo_green()
{
    red_color="\033[31m"
    green_color="\033[32m"
    reset_color="\033[0m"
    echo -e "${green_color}$1${reset_color}"
}

echo_red()
{
    red_color="\033[31m"
    green_color="\033[32m"
    reset_color="\033[0m"
    echo -e "${red_color}$1${reset_color}"
}

debug() {
    if [ "$VERBOSE" != "" ] ; then
	echo $@
    fi
}

warn() {
    echo $@
}

error() {
    echo $@
    exit -1
}

function wait_interrupt() {
    # $1: a message. $2: timout value
    echo -n  "`date +'%Y-%m-%d %H:%M:%S'`: "
    read -t $2 -a input_value -p "$1: wait $2 second...[cont/other_to_stop]: "
    if [ $? -eq 0 ] ; then
	while true ; do
	    if [ "$input_value" == "cont" ] ; then
		break
	    elif [ "$input_value" == "" ] ; then
		break
	    else
		read -a input_value -p "please input [cont]: "
	    fi
	done
    else
	echo "<<< timed out for input. continue"
    fi
}
