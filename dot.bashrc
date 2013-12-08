echo helle iam from .bashrc
set +h
umask 022
LC_ALL=POSIX
PATH=/root/bin/bin:/sbin:/bin:/usr/sbin:/usr/bin
alias ls='ls --color'
alias asleep='echo standby>/sys/power/state'
alias sir_cp='cp -v arch/i386/boot/bzImage /boot/lfs_ker-2.6.22.5-ramfs'
KER=/usr/src/linux-2.6.22.5/
export PATH LC_ALL KER


alias em="emacs -nw"