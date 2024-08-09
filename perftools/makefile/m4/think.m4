changequote([,])dnl
define(aa, hello)dnl 直接展开
define(bb, [hello])dnl 先引用展开
define(cc, [[hello]])dnl 先用用展开 [[hello]] -> [hello], cc 替换成[hello], 再次对结果尽心引用展开 -> hello

define(dd, [[[hello]]])dnl
dnl 参数展开, 引用展开, [[[hello]]] -> [[hello]] 。  翻译成 把dd 变成[[hello]].
dd
dnl dd是一个宏，宏展开为[[hello]], 再对结果展开，结果是个引用[[hello]], 对引用展开，变为[hello]。 引用的结果不继续展开

宏展开  宏展开的结果要再展开
引用展开 引用展开的结果不再继续展开
参数展开, 参数可以是宏或者引用

宏展开
  参数展开
    宏展开
    引用展开
引用展开

宏展开的意思是： 把这个替换成别的，并且给你点参数，可以利用这些参数造出来一个东西

一个特殊，$1 $2等参数，参数替换的过程发生在参数展开之后，rescanning之前, 不管有多少quota
