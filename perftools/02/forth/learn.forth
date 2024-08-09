( This is a comment in forth. notice the there is a space after left parenthese and also before right parenthese )


( hello world )
." Firsh example is classical hello world program " CR
." hello world "
CR

CR
." #################################### " CR
." ####### expression           ####### " CR
." #################################### " CR

( expresstion )
CR ." test expression +-*/ " CR
1 2 + . CR
2 1 - . CR
1 3 * . CR
3 3 / . CR

CR ." test >/</= " CR
." 2 > 1 ? should be true " CR
2 1 > . CR   \ -1 is true in forth. 0 is false
." 2 < 1 ? should be true " CR
2 1 < . CR
." equal " CR
1 1 = . CR

CR ." seem no '|| &&' in forth? " CR

CR ." test bit and/or/not " CR
7 3 AND . CR
1 0 AND . CR
2 0 OR . CR
0 0 OR . CR

CR ." test >>/<< " CR
1 3 LSHIFT . CR
1 32 LSHIFT . CR   \ ? what is max shift size?


CR
." #################################### " CR
." ####### function abstraction ####### " CR
." #################################### " CR

: TIMES  \ n1 n2 --
 * .
;

2 3 TIMES

CR
." dump out function definition "
SEE TIMES

CR
." call a subfunction in another source code "
INCLUDE learn_sub.forth
3 4 MULTIPLE



CR CR
." #################################### " CR
." ####### variable abstraction ####### " CR
." ####### declation statment   ####### " CR
." #################################### " CR

variable foo
123 foo !        \ store it


." use 2 variable in an add expresstion " CR
variable bar
variable baz
2 bar !
3 baz !
.s
bar @
baz @
.s
CR
+ .
CR
.s CR

( constand usage )
1 CONSTANT MAX_NUM
2 MAX_NUM + . CR

8 VALUE A_MOST_READ_VAR
A_MOST_READ_VAR . CR


CR CR
." #################################### " CR
." #######  if condition        ####### " CR
." #################################### " CR

( IF is only allow in compile time. so must bt used in a function
IF
." it is true "
ELSE
." it is false "
THEN )

: TEST_IT
IF
." it is true "
ELSE
." it is false "
THEN
;

2 3 > TEST_IT CR


( special in forth. what is on the stack. without touch it )
\ .s
