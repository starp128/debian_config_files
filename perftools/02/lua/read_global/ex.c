//gcc -I ./lua-5.3.6/src/ -L ./lua-5.3.6/src/   m2.c  -llua -lm -ldl

#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static lua_State *L;

int opt_get_num(char *name)
{
	lua_getglobal(L, name);

	return lua_tonumber(L, -1);
}

const char* opt_get_string(char *name)
{
	lua_getglobal(L, name);

	return lua_tostring(L, -1);

}

int opt_init(char *config_file_path)
{
	L = luaL_newstate();
	if (!L) {
		return -1;
	}
	luaL_openlibs(L);

	//if(luaL_loadfile(L, config_file_path)) no work

	if (luaL_dofile(L, config_file_path)) {
		return -1;
	}

	/*
	lua_getglobal(L,"mystr");

	const char *version = lua_tostring(L, -1);


	// 获取表中的数据
	lua_getglobal(L,"myTable");
	lua_getfield(L,-1,"name");
	version = lua_tostring(L,-1);

	lua_getglobal(L,"myTable");
	lua_getfield(L,-1,"id");
	int abc =  lua_tonumber(L,-1);
	printf("%d", abc);

	*/
}

void opt_fini(void)
{
	lua_close(L);
}

#if 1
int main()
{
	int i = 10;

	while(i--) {
		opt_init("ex.lua");
		printf("%d\n", opt_get_num("count"));
		printf("%s\n", opt_get_string("mystr"));
		opt_fini();
	}
	return 0;
}
#endif
