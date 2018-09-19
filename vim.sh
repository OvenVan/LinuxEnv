#! /bin/bash

git clone https://github.com/tpope/vim-pathogen.git ~/xvim
mkdir -p ~/xvim ~/xvim/bundle 
git clone git://github.com/jiangmiao/auto-pairs.git ~/xvim/bundle/auto-pairs
git clone https://github.com/scrooloose/nerdtree.git ~/xvim/bundle/nerdtree
cd ~/xvim/bundle && wget "http://www.vim.org/scripts/download_script.php?src_id=19574" -O taglist.zip && unzip taglist.zip -d taglist
rm taglist.zip
mkdir -p ~/xvim/tags && cd ~/xvim/tags && wget "http://www.vim.org/scripts/download_script.php?src_id=9178" -O - | tar jxvf -
cd ./cpp_src && ctags -R --sort=yes --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++
cd ~/xvim/bundle && git clone https://github.com/ervandew/supertab.git
cd ~/xvim/bundle && wget "http://www.vim.org/scripts/download_script.php?src_id=7722" -O omnicppcomplete.zip && unzip omnicppcomplete.zip -d omnicppcomplete
rm omnicppcomplete.zip
cd ~/xvim && rm *.markdown && rm .git -rf

