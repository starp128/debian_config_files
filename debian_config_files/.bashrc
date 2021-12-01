#PS1="\[\e[32m\][\u@\h \w]\n\#$\[\e[m\]"
PS1="\[\e[33m\][\u@\h \[\e[33;1m\]\w\[\e[33m\]]\n\# $\[\e[m\]"
#30:黑 31:红 32:绿 33:黄 34:蓝色 35:紫色 36:深绿 37:白色
#40:黑 41:深红 42:绿 43:黄色 44:蓝色 45:紫色 46:深绿 47:白色

alias cp="cp -v"
alias mv="mv -v"
alias ls="ls --color"
alias ll="ls -l"
alias la="ls -al"
alias grep="grep --color=auto"
alias rmbk="mv -v .*\~ *\~ \#*\# ~/.terisk_laji/"

#useful functions stops you from endless typing
mcd() { mkdir -p "$1"; cd "$1";}
cls() { cd "$1"; ls;}
extract() {
    if [ -f $1 ] ; then
      case $1 in
        *.tar.bz2)   tar xjf $1     ;;
        *.tar.gz)    tar xzf $1     ;;
        *.bz2)       bunzip2 $1     ;;
        *.rar)       unrar e $1     ;;
        *.gz)        gunzip $1      ;;
        *.tar)       tar xf $1      ;;
        *.tbz2)      tar xjf $1     ;;
        *.tgz)       tar xzf $1     ;;
        *.zip)       unzip $1       ;;
        *.Z)         uncompress $1  ;;
        *.7z)        7z x $1        ;;
        *)     echo "'$1' cannot be extracted via extract()" ;;
         esac
     else
         echo "'$1' is not a valid file"
     fi
}

gen_etags() {
    bash -c 'find . -name "*.[ch]" | etags -'
}

export HISTTIMEFORMAT='%F %T '

export LESS_TERMCAP_mb=$'\E[01;31m'
export LESS_TERMCAP_md=$'\E[01;31m'
export LESS_TERMCAP_me=$'\E[0m'
export LESS_TERMCAP_se=$'\E[0m'
export LESS_TERMCAP_so=$'\E[01;44;33m'
export LESS_TERMCAP_ue=$'\E[0m'
export LESS_TERMCAP_us=$'\E[01;32m'

export DISPLAY=":0"
export LANG="en_US.UTF-8"
export LANGUAGE="en_US.UTF-8"

#xmodmap ~/.xmodmaprc
#echo "CAP now is CTRL"

HISTSIZE=1000000
HISTFILESIZE=1000000
export HISTSIZE
export HISTFILESIZE

#uncomment this to make set -x print more meaningful message
#export PS4='+(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]:+${FUNCNAME[0]}(): }'

#another one from bashdb
#PS4='(${BASH_SOURCE}:${LINENO}): ${FUNCNAME[0]} - [${SHLVL},${BASH_SUBSHELL}, $?]
#'


# README
# cp -r .cnt to /your/real/workspace/
# cp /your/real/workspace/.cnt/debian_config_files/.bashrc /home/you. and source /hoem/you/.bashrc
# ln -s /your/real/workspace /b

# the config really does is share an user name but provide your custom configuration.
# config file do not need to move out to you ~ dir.

if [ -h /b ] ; then
    # using private tmux. in order to share box with others
    # alias tm="/b/bin/tmux -f /b/.cnt/debian_config_files/.tmux.conf -S /b/bin/sock"
    alias tm="tmux -f /b/.cnt/debian_config_files/.tmux.conf"
    alias em="emacs -nw --load /b/.cnt/debian_config_files/.emacs"
    alias note="emacs -nw --load /b/.cnt/debian_config_files/.emacs /b/ws/note"
fi

#bind '"\M-\C-f": shell-forward-word'
#bind '"\M-\C-b": shell-backward-word'

#bind '"\C-f":vi-fWord'
#bind '"\C-b":vi-bWord'

bind '"\ef": vi-fWord'
bind '"\eb": vi-bWord'
