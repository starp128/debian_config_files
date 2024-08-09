#include <iostream>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

/*
  const char *lua_script = R"(
  version = '1.0.0'
  function add(a, b)
  return a + b;
  end
  )";
*/

int main() {
	lua_State *L = luaL_newstate();
	if (L == NULL) {
		std::cerr << "create state failed!" << std::endl;
		exit(-1);
	}

	/*
	if (luaL_dostring(L, lua_script) != 0) {
		std::cerr << "cannot run lua script!" << std::endl;
		exit(-1);
	}
	*/
	if (luaL_dofile(L, "./ex2.lua")) {
		std::cout << "faied" << std::endl;
	}

	lua_getglobal(L, "aa");
	int abc = lua_tonumber(L, -1);
	
// get global variable
	lua_getglobal(L, "version");
	const char *version = lua_tostring(L, -1);
	std::cout << "version: " << version << std::endl;

	version = lua_tostring(L, -2);
	version = lua_tostring(L, 0);


	
	lua_pop(L, 1);

// call global function
	lua_getglobal(L, "add");
	int a = 100;
	int b = 200;
	lua_pushnumber(L, a);
	lua_pushnumber(L, b);
	if (lua_pcall(L, 2, 1, 0) != 0) {
		std::cerr << "call func failed, got error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		exit(-1);
	}

	int sum = lua_tonumber(L, -1);
	lua_pop(L, 1);

	std::cout << a << " + " << b << " = " << sum << std::endl;
	lua_close(L);
	return 0;
}

