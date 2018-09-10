# .bashrc

alias ll='ls -al'
alias ls='ls --color=auto -l'
alias l='ls'

alias nginx='/usr/local/nginx/sbin/nginx'
alias ngxConfigure='cdn && ./configure --add-module=src/Ctest --add-module=src/Filter && make && make install'
alias cdNginxLog='cd /usr/local/nginx/logs'
alias cdNginx='cd /usr/local/nginx'
alias cdNginxConf='cd /usr/local/nginx/conf'
alias cdSrc='cd /root/nginx-1.14.0/src'
alias cdn='cd /root/nginx-1.14.0'
alias g++='g++ -std=gnu++11'
alias rmgch='rm *.gch' 
alias vi='vim'

alias rm='rm'

alias crtTags='ctags -R --sort=yes --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++'

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
