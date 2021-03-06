

#pragma once 

#include "impl\function_impl.h"
#include <memory>

namespace autoLua {
	// pimpl class (implementation is in .cpp file)

	typedef std::unique_ptr<impl::BaseFunctionWrapper> WrapperPtr;

	// class to keep track of (and in memory) all functions/objects registered from C++ in lua
	namespace impl {
		class LuaRegistry;
	}

	void registerFunction(impl::LuaRegistry*, WrapperPtr&&);
	//void registerConstructor(impl::LuaRegistry*, WrapperPtr&&);

	impl::LuaRegistry* lua_newregister(lua_State*);
	void lua_closeregister(impl::LuaRegistry*);

	lua_State* getRegisterState(impl::LuaRegistry*);

}