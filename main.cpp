

#include "LuaState.h"
#include "FunctionWrapper.h"

#include "LuaTypes\Tuple.h"
#include "LuaTypes\LuaNil.h"
#include "LuaTypes\LuaTable.h"

#include "impl\LuaStackGuard.h"

#include "debugTesting.h"

#include <iostream>

using namespace autoLua;
using namespace autoLua::impl;
using namespace std;

// possible to bind variadic functions ???

// TODO:
// implement LuaStackGuard within the various Lua_ classes
// Register functions with lua using the indexing syntax
// Pass values between lua variables using the indexing syntax
// LuaThreadState class
// Move non-template function definitions to source files
// Move commonly used 'function_impl' functions to seperate file ???
// Correct autoLua::impl::_getLuaTable implementation as mentioned in the function

#define showLine(x) cout << x << endl
#define showPause(x) showLine(x); cin.get()


int main(int argc, const char* argv[]) {
	LuaState L;
	double x, y, z;

	// demonstrating current function wrapping syntax
	function<tuple<double, double, double>(double, double)> combiner = [] (double x, double y) -> tuple<double, double, double> {
		return make_tuple(x, y, x + y);
	};
	auto someCombo = makeWrapper(L, combiner);
	lua_setglobal(L, "combo");

	// luaTie(x,y,z) = L["combo"](5.3, 4.2);
	// top throws error C4716 'LuaTypeTraits<LuaTuple<double,double,double>>::popValue
	// must return a value', yet the bottom doesn't !!!???
	{ auto tmp = L["combo"](5.3, 4.2); luaTie(x, y, z) = tmp; }

	showLine(x);
	showLine(y);
	showLine(z);
	showLine("");

	{
		impl::LuaStackGuard guard(L);

		*L = 3.5;
		*L = true;
		*L = "Hello";
		debug::debugStack(L, cout);
	}
	debug::debugStack(L, cout);
	showLine("The stack has been cleared!!!");


	// demonstrating recent addition
	L["test"]["randVal"]["randKey"] = 5.5;
	luaL_remove(L, -2);	// needs to go into LuaVarHelper destructor

	showLine((double)L["test"]["randVal"]["randKey"]);

	// should go in destructor
	luaL_remove(L, -2);
	luaL_remove(L, -2);

	// cout << LuaStackPos<-1>::getValue(lua) << endl;

	cin.get();
}