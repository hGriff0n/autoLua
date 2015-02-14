

#include "impl\LuaRegistry.h"

#include <vector>

namespace autoLua {

	class impl::LuaRegistry {
		public:
			std::vector<WrapperPtr> function_set;
			lua_State* L;

			LuaRegistry(lua_State* lua) : L(lua) { }
			~LuaRegistry() {
				L = nullptr;
			}
	};

	void registerFunction(impl::LuaRegistry* registry, WrapperPtr&& item) {
		registry->function_set.emplace_back(std::forward<WrapperPtr>(item));
	}

	impl::LuaRegistry* lua_newregister(lua_State* L) {
		return new impl::LuaRegistry(L);
	}
	void lua_closeregister(impl::LuaRegistry* registry) {
		delete registry;
	}

	lua_State* getRegisterState(impl::LuaRegistry* registry) {
		return registry->L;
	}

}