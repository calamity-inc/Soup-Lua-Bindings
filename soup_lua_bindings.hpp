#pragma once

#include <soup/country_names.hpp>
#include <soup/joaat.hpp>
#include <soup/IpAddr.hpp>
#include <soup/netIntel.hpp>

namespace soup
{
	struct LuaBindings
	{
		// C++ API

		static void open(lua_State* L)
		{
			lua_newtable(L);

			// netIntel
			{
				const luaL_Reg functions[] = {
					{"getAsByIp", &lua_netIntel_getAsByIp},
					{"getLocationByIp", &lua_netIntel_getLocationByIp},
					{nullptr, nullptr}
				};
				luaL_newlib(L, functions);
				lua_setfield(L, -2, "netIntel");
			}

			lua_pushcfunction(L, &lua_getCountryName);
			lua_setfield(L, -2, "getCountryName");

			lua_pushcfunction(L, &lua_IpAddr);
			lua_setfield(L, -2, "IpAddr");

			lua_setglobal(L, "soup");
		}

		// Lua API

		static int lua_netIntel_getAsByIp(lua_State* L)
		{
			auto* as = netIntel::getAsByIp(checkIp(L, 1));
			pushMediumUserdata(L, as);
			lua_newtable(L);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("isValid"):
						lua_pushcfunction(L, &lua_mm_isValid);
						return 1;

					case joaat::hash("number"):
						lua_pushinteger(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->number);
						return 1;

					case joaat::hash("handle"):
						lua_pushstring(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->handle);
						return 1;

					case joaat::hash("name"):
						lua_pushstring(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->name);
						return 1;

					case joaat::hash("isHosting"):
						lua_pushcfunction(L, [](lua_State* L) -> int
						{
							lua_pushboolean(L, reinterpret_cast<netAs*>(checkMediumUserdata(L, 1))->isHosting());
							return 1;
						});
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_netIntel_getLocationByIp(lua_State* L)
		{
			auto* location = netIntel::getLocationByIp(checkIp(L, 1));
			pushMediumUserdata(L, location);
			lua_newtable(L);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("isValid"):
						lua_pushcfunction(L, &lua_mm_isValid);
						return 1;

					case joaat::hash("city"):
						lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->city);
						return 1;

					case joaat::hash("state"):
						lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->state);
						return 1;

					case joaat::hash("country_code"):
						lua_pushstring(L, reinterpret_cast<netIntelLocationData*>(checkMediumUserdata(L, 1))->country_code.c_str());
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_getCountryName(lua_State* L)
		{
			if (lua_gettop(L) >= 2)
			{
				lua_pushstring(L, getCountryName(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
			}
			else
			{
				lua_pushstring(L, getCountryName(luaL_checkstring(L, 1)));
			}
			return 1;
		}

		// Lua Helpers

		[[nodiscard]] static IpAddr checkIp(lua_State* L, int i)
		{
			if (lua_type(L, i) == LUA_TSTRING)
			{
				return IpAddr(lua_tostring(L, i));
			}
			return IpAddr((uint32_t)luaL_checkinteger(L, i));
		}

		static void pushMediumUserdata(lua_State* L, void* ud)
		{
			*(void**)lua_newuserdata(L, sizeof(void*)) = ud;
		}

		static void checkType(lua_State* L, int i, int tp)
		{
			if (lua_type(L, i) != tp)
			{
				luaL_typeerror(L, i, lua_typename(L, tp));
			}
		}

		[[nodiscard]] static void* checkMediumUserdataNullable(lua_State* L, int i)
		{
			checkType(L, i, LUA_TUSERDATA);
			return *(void**)lua_touserdata(L, i);
		}

		[[nodiscard]] static void* checkMediumUserdata(lua_State* L, int i)
		{
			auto ud = checkMediumUserdataNullable(L, i);
			if (ud == nullptr)
			{
				luaL_error(L, "nullptr access");
			}
			return ud;
		}

		static int lua_mm_isValid(lua_State* L)
		{
			lua_pushboolean(L, checkMediumUserdataNullable(L, 1) != nullptr);
			return 1;
		}
	};
}
