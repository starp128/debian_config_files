#PS1="\[\e[32m\][\u@\h \w]\n\#$\[\e[m\]"
PS1="\[\e[32m\][\u@\h \[\e[32;1m\]\w\[\e[32m\]]\n\# $\[\e[m\]"

alias em="emacs -nw"
alias cp="cp -v"
alias mv="mv -v"
alias ls="ls --color"
alias ll="ls -l"
alias grep="grep --color=auto"
alias rmbk="mv -v *~ \#*\# ~/.terisk_laji/"

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

xmodmap ~/.xmodmaprc
echo "CAP now is CTRL"
