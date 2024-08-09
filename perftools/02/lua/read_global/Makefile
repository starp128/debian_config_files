
LUA_TOP=../lua-5.3.6/src/

ex:
	gcc -I$(LUA_TOP) -L$(LUA_TOP) ex.c -llua -lm -ldl -o ex -g -O0
	g++ -I$(LUA_TOP) -L$(LUA_TOP) ex2.cpp -llua -lm -ldl -o ex2 -g -O0

clean:
	rm -f ex ex2
