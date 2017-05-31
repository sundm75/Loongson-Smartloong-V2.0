"==
"Author :roc
"Website:roclinux.cn
"Version:1.0
"==
 
"按编程语言的语法，对代码进行彩色标示，术语叫做语法高亮
syntax on
 
"显示行数标示
set number
 
"打开状态栏的坐标信息
set ruler
 
"取消底部状态栏显示。1为关闭，2为开启。
set laststatus=1
 
"将输入的命令显示出来，便于查看当前输入的信息
set showcmd
 
"设置魔术匹配控制，可以通过:h magic查看更详细的帮助信息
set magic
 
"设置vim存储的历史命令记录的条数
set history=100
 
"下划线高亮显示光标所在行
set cursorline
 
"插入右括号时会短暂地跳转到匹配的左括号
set showmatch
 
"搜索时忽略大小写
set ignorecase
 
"不对匹配的括号进行高亮显示
let loaded_matchparen=1
 
"在执行宏命令时，不进行显示重绘；在宏命令执行完成后，一次性重绘，以便提高性能。
set lazyredraw
 
"设置一个tab对应4个空格
set tabstop=4
 
"在按退格键时，如果前面有4个空格，则会统一清除
set softtabstop=4
 
"cindent对c语法的缩进更加智能灵活，
"而shiftwidth则是在使用&lt;和&gt;进行缩进调整时用来控制缩进量。
"换行自动缩进，是按照shiftwidth值来缩进的
set cindent shiftwidth=4
 
"最基本的自动缩进
set autoindent shiftwidth=4
 
"比autoindent稍智能的自动缩进
set smartindent shiftwidth=4
 
"将新增的tab转换为空格。不会对已有的tab进行转换
set expandtab
 
"高亮显示搜索匹配到的字符串
set hlsearch
 
"在搜索模式下，随着搜索字符的逐个输入，实时进行字符串匹配，并对首个匹配到的字符串高亮显示
set incsearch
 
"设置自定义快捷键的前导键
let mapleader=","
 
"利用前导键加b，则可以在一个单子两边加上大括号
map b wbi{<Esc>ea}<Esc>
 
"使用前导键加w来实现加速文件保存，来代替:w!加回车
nmap w :w!<CR>
 
"匹配那些末尾有空格或TAB的行。（es：Endspace Show）
map es :/.*\s\+$<CR>
 
"删除行末尾的空格或TAB（ed：Endspace Delete）
map ed :s#\s\+$##<CR>
 
"如果所选行的行首没有#，则给所选行行首加上注释符#（#a：# add）
map #a :s/^\([^#]\s*\)/#\1/<CR>
 
"如果所选行行首有#，则将所选行行首所有的#都去掉（#d：# delete）
map #d :s/^#\+\(\s*\)/\1/<CR>
 
"如果所选行的行首没有//，则给所选行行首加上注释符//（/a：/ add）
map /a :s/^\([^\/\/]\s*\)/\/\/\1/<CR>
 
"如果所选行行首有//，则将所选行行首的//都去掉（/d：/ delete）
map /d :s/^\/\/\(\s*\)/\1/<CR>

set fileencodings=utf-8,gbk,gb2312,gb18030 
set termencoding=utf-8
set fileformats=unix
set encoding=prc
