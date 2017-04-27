#!/bin/bash
# author: Ernest Du 
# please change USERNAME and PASSWD to your own username and password.
# modified by xuanlangjian 2012.07.31

USERNAME=school_id
PASSWD=password

function log_in
{
	RESULT=`curl --silent -d "username=$USERNAME&password=$(echo -n $PASSWD|md5sum|cut -c 9-24)&drop=0&type=1&n=100" "http://gw.bupt.edu.cn/cgi-bin/do_login"`
	if [ "$RESULT" = "online_num_error" ]; then
		echo -e "The number of users has exceed the limit,please log out first!"
	else
		echo -e "logged in success!"
	fi

}

function log_out
{
	RESULT=`curl --silent -d "username=$USERNAME&password=$PASSWD&drop=0&type=1&n=1" "http://gw.bupt.edu.cn/cgi-bin/force_logout"`
	if [ "$RESULT" = "logout_ok" ]; then
		echo -e "logged out success!"
	elif [ "$RESULT" = "logout_error" ]; then
		echo -e "logged out error!"
	fi
}

case $1 in 
-i)
	log_in;;
-o)
	log_out;;
*)
	echo  -e "buptgw usage:\noption\n-i\n\tlogin\n-o\n\tlogout"
esac
