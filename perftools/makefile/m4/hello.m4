define(`nargs', `$#')dnl
nargs
nargs()
nargs(arg1, arg2, arg3)


define(`push3', `move aaa , bbb
move ccc ,ddd')
push3


################

len(`aaaa')

define(`my var', `a strange one')

my var is defn(`my var')



#
  define(`_set', `define(`$1[$2]', `$3')')
  define(`_get', `defn(`$1[$2]')')
  _set(`myarray', 1, `alpha')
  _get(`myarray', 1)                  # -> alpha
  _set(`myarray', `alpha', `omega')
  _get(`myarray', _get(`myarray',1))  # -> omega
  defn(`myarray[alpha]')              # -> omega
  



