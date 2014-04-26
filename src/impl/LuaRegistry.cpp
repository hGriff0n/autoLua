

#include "impl\LuaRegistry.h"

#include <vector>

namespace autoLua {

	class impl::LuaRegistry {
		private:
			std::vector<WrapperPtr> function_set;

		public:
			LuaRegistry(lua_State* lua) { }
			~LuaRegistry() { }

			std::vector<WrapperPtr>& functions() {
				return function_set;
			}
	};

	void registerFunction(impl::LuaRegistry* registry, WrapperPtr&& item) {
		registry->functions().emplace_back(std::forward<WrapperPtr>(item));
	}

	impl::LuaRegistry* lua_newregister(lua_State* L) {
		return new impl::LuaRegistry(L);
	}
	void lua_closeregister(impl::LuaRegistry* registry) {
		delete registry;
	}

}