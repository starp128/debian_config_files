#!/bin/bash

help()
{
    echo "help"
}

while [[ $# -gt 0 ]]; do
    case "$1" in
	-I|--instence)
            instence="$2"
	    echo "instance ${instence}"
            shift
            ;;
	-D|--database)
            database="$2"
	    echo "database ${database}"
            shift
            ;;
	-h|--help)
            usage
            ;;
	--)
            shift
            break
            ;;
    esac
    shift
done 



