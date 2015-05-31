

#include "LuaState.h"
#include "FunctionWrapper.h"

#include "LuaTypes\LuaTuple.h"
#include "LuaTypes\LuaNil.h"
#include "LuaTypes\LuaTable.h"

#include "debugTesting.h"
#include "Examples.h"

#include <iostream>

using namespace autoLua;
using namespace autoLua::impl;
using namespace std;


// TODO:
// Improve LuaTable
// Find a way to directly assign lambdas to lua variables
    // I don't believe it would be possible to do so for generic lambdas however
// Can't do implicit conversion using std::string from LuaStack ???
// Consider removing LuaNil, except as a typedef over nullptr_t
    // What should the behavior for pointers be if LuaTypeTraits is defined for T* (particularly returns) ???
    // How do I go about returning and converting nil values from Lua (Here's where the typedef fails) ???
// Beef Up Exception classes and Improve Error Handling
    // Figure out how to handle and throw a LuaPanicError so the program does not exit (remove exceptions from LuaTypeTraits to test)
        // This might involve me having to change the "atPanic" function
        // http://www.knightsgame.org.uk/blog/2012/09/03/notes-on-luac-error-handling/
    // Find a way to include type and index information with error messages
        // Also way to include function stack for Lua calls ???
    // Add LuaStackErrors to LuaTypeTraits (bad indices)
// Provide some form of permanence for LuaVariable (or merge it with LuaStack)
    // It's basically a glorified stack when you think about it
    // Also need to provide a way to change the throw behavior when creating new tables
// C++/lua object binding/wrapping/etc.
	// std::mem_fn to bind member functions ???
// Start filling in github readme with information about the interface and project
// Implement LuaStackGuard within the various Lua_ classes (maybe)
// LuaThreadState class
// Correct autoLua::impl::_getLuaTable implementation as mentioned in the function
// Use std pointers instead of raw ones in Lua_ classes (I'm already including <memory>)
// Can I make the stack iterable like (for (auto elem : LuaStack(L)) cout << elem.idx() << ": " << (T)elem << endl;)

// possible to bind variadic functions ???
// mark issue in github for converting LuaStack to a tuple (fixed???)
// how are type errors handled in lua ie. what happens when "tmp.combo("Hello", nil)" is run ???

// LuaExceptions
    // Type errors
    // Stack errors (indexing)
    // Panic errors (something goes wrong in lua_call/lua_pcall. causes program to exit)

#define showLine(x) (cout << x << endl)
#define showPause(x) (showLine(x); cin.get())
#define pause() (cin.get())

int main(int argc, const char* argv[]) {
	LuaState L;

	double x, y, z;
	function<tuple<double, double, double>(double, double)> combiner = [](auto x, auto y) {
		return make_tuple(x, y, x + y);
	};

	try {
		LuaStackGuard guard(L);

		L["combo"] = combiner;
		//luaTie(x, y, z) = L["combo"](5.3, 4.2);
		luaTie(x, y, z) = L["combo"]("Hello", 4);    // This should throw a LuaPanicError. Throws LuaTypeError

		debug::debugStack(L, cout);

		showLine(x);
		showLine(y);
		showLine(z);
	} catch ( LuaException e ) {
		cout << e.error() << endl;
	}

	/* try {
		LuaStackGuard guard(L);

		L["foo"]["bar"]["foo"] = 5;

		lua_settop(L, 0);

		L["tmp"] = L["foo"]["bar"]["foo"];

		lua_settop(L, 0);

		//showLine((int)L["tmp"]);
	} catch ( LuaException e ) {
		showLine(e.error());
	}

	try {
		LuaStackGuard guard(L);

		L["combo"] = combiner;
		luaTie(x, y, z) = L["combo"](5.3, 4.2);
		//luaTie(x, y, z) = L["combo"]("Hello", 4);    // This should throw a LuaPanicError. Throws LuaTypeError

		debug::debugStack(L, cout);

		showLine(x);
		showLine(y);
		showLine(z);
	} catch ( LuaException e ) {
		cout << e.error() << endl;
	}

	try {
		LuaStackGuard guard(L);

		int* p = nullptr;

		cout << "Pushing pointer" << endl;

		//*L << p;    // "<<" will take raw pointers if LuaTypeTraits<T*> is defined

		impl::LuaTypeTraits<int*>::pushValue(L, p);    // Throws an UndefinedBehavior unless LuaTypeTraits<T*> is uncommented

		debug::debugStack(L, cout);
	} catch ( LuaException e ) {
		cout << e.error() << endl;
	}

	try {
		LuaStackGuard guard(L);

		*L << "Hello";

		// Why does this not compile (C2440: No constructor could take the source type / overload was ambiguous) ???
		    // LuaStack has a templated implicit casting operator
		    // Creating an explicit overload for std::string does not change the error
		//string err = *L;

		debug::debugStack(L, cout);
	} catch ( LuaException e ) {
		cout << e.error() << endl;
	} */

	// demonstrating Examples.h
	try {
		*L << "Goodbye" << 3.00 << "World" << 4.00 << "Hello" << 5.00 << "World";
		cout << after<int>(L) << endl;
		cout << before<int>(L, -2) << endl;
		showLine(( get<string>(L, 1) ));
	} catch ( LuaException e ) {
		cout << e.error() << endl;
	}

	cin.get();
}