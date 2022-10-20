#pragma once

#include <memory>

#include <soup/country_names.hpp>
#include <soup/joaat.hpp>
#include <soup/json.hpp>
#include <soup/JsonArray.hpp>
#include <soup/JsonBool.hpp>
#include <soup/JsonFloat.hpp>
#include <soup/JsonInt.hpp>
#include <soup/JsonString.hpp>
#include <soup/IpAddr.hpp>
#include <soup/netIntel.hpp>
#include <soup/Vector3.hpp>

namespace soup
{
	// If you're not using Pluto, your compiler might raise warnings because the error functions don't have the [[noreturn]] attribute in stock lua.

	struct LuaBindings
	{
#define pushNewAndBeginMt(T, ...) pushNewAndBeginMtImpl<T>(L, "soup::" #T, __VA_ARGS__);

#pragma region C++ API

#ifdef PLUTO_VERSION
		static constexpr auto PLUTO_MAJOR = (PLUTO_VERSION[6] - '0');
		static constexpr auto PLUTO_MINOR = (PLUTO_VERSION[8] - '0');
		static constexpr auto PLUTO_PATCH = (PLUTO_VERSION[10] - '0');
#endif

		static void open(lua_State* L)
		{
			lua_newtable(L);

			open_setDataFields(L);
			open_setMathFields(L);
			open_setNetFields(L);

			lua_setglobal(L, "soup");
		}
#pragma endregion C++ API

#pragma region Lua API - Data
		static void open_setDataFields(lua_State* L)
		{
			// json
			{
				const luaL_Reg functions[] = {
					{"encode", &lua_json_encode},
					{"decode", &lua_json_decode},
					{nullptr, nullptr}
				};
				luaL_newlib(L, functions);
				lua_setfield(L, -2, "json");
			}
		}

		static int lua_json_encode(lua_State* L)
		{
			// Lua Table -> JSON Tree
			auto root = checkJson(L, 1);
			// JSON Tree -> String
			if (lua_gettop(L) >= 2 && lua_toboolean(L, 2))
			{
				pushString(L, root->encodePretty());
			}
			else
			{
				pushString(L, root->encode());
			}
			return 1;
		}

		static int lua_json_decode(lua_State* L)
		{
			// String -> JSON Tree
			auto root = json::decodeForDedicatedVariable(luaL_checkstring(L, 1));
			// JSON Tree -> Lua Table
			pushFromJson(L, *root);
			return 1;
		}
#pragma endregion Lua API - Data

#pragma region Lua API - Net
		static void open_setNetFields(lua_State* L)
		{
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
		}

		static int lua_netIntel_getAsByIp(lua_State* L)
		{
			auto* as = netIntel::getAsByIp(checkIpAddr(L, 1));
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
			auto* location = netIntel::getLocationByIp(checkIpAddr(L, 1));
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

		static int lua_IpAddr(lua_State* L)
		{
			pushNewAndBeginMt(IpAddr, checkIpAddr(L, 1));
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("getReverseDns"):
						lua_pushcfunction(L, [](lua_State* L) -> int
						{
							pushString(L, reinterpret_cast<IpAddr*>(lua_touserdata(L, 1))->getReverseDns());
							return 1;
						});
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__tostring");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					pushString(L, reinterpret_cast<IpAddr*>(lua_touserdata(L, 1))->toString());
					return 1;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}
#pragma endregion Lua API - Net

#pragma region Lua API - Math
		static void open_setMathFields(lua_State* L)
		{
			lua_pushcfunction(L, &lua_Matrix);
			lua_setfield(L, -2, "Matrix");

			lua_pushcfunction(L, &lua_Vector3);
			lua_setfield(L, -2, "Vector3");
		}

		static int lua_Matrix(lua_State* L)
		{
			pushNewAndBeginMt(Matrix);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("setPosRotXYZ"):
						lua_pushcfunction(L, &lua_Matrix_setPosRotXYZ);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__mul");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					checkTypename(L, 2, "soup::Vector3");
					Matrix& m = *reinterpret_cast<Matrix*>(lua_touserdata(L, 1));
					*pushNewVector3(L) = (m * *reinterpret_cast<Vector3*>(lua_touserdata(L, 2)));
					return 1;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return 1;
		}

		static int lua_Matrix_setPosRotXYZ(lua_State* L)
		{
			Vector3 pos, rot;
			if (lua_gettop(L) == 3)
			{
				checkTypename(L, 2, "soup::Vector3"); pos = *reinterpret_cast<Vector3*>(lua_touserdata(L, 2));
				checkTypename(L, 3, "soup::Vector3"); rot = *reinterpret_cast<Vector3*>(lua_touserdata(L, 3));
			}
			else
			{
				pos.x = (float)luaL_checknumber(L, 2);
				pos.y = (float)luaL_checknumber(L, 3);
				pos.z = (float)luaL_checknumber(L, 4);
				rot.x = (float)luaL_checknumber(L, 5);
				rot.y = (float)luaL_checknumber(L, 6);
				rot.z = (float)luaL_checknumber(L, 7);
			}
			reinterpret_cast<Matrix*>(lua_touserdata(L, 1))->setPosRotXYZ(pos, rot);
			return 0;
		}

		static int lua_Vector3(lua_State* L)
		{
			const auto args = lua_gettop(L);
			auto v = pushNewVector3(L);
			if (args >= 3)
			{
				v->x = (float)luaL_checknumber(L, 1);
				v->y = (float)luaL_checknumber(L, 2);
				v->z = (float)luaL_checknumber(L, 3);
			}
			return 1;
		}
#pragma endregion Lua API - Math

#pragma region Lua Helpers
		[[nodiscard]] static IpAddr checkIpAddr(lua_State* L, int i)
		{
			if (lua_type(L, i) == LUA_TSTRING)
			{
				return IpAddr(lua_tostring(L, i));
			}
			else if (lua_type(L, i) == LUA_TUSERDATA)
			{
				return *(IpAddr*)lua_touserdata(L, 1);
			}
			return IpAddr(native_u32_t((uint32_t)luaL_checkinteger(L, i)));
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

		template <typename T, typename...Args>
		static T* pushNew(lua_State* L, Args&&...args)
		{
			return std::construct_at((T*)lua_newuserdata(L, sizeof(T)), std::forward<Args>(args)...);
		}

		template <typename T, typename...Args>
		static T* pushNewAndBeginMtImpl(lua_State* L, const char* tn, Args&&...args)
		{
			auto inst = pushNew<T>(L, std::forward<Args>(args)...);
			lua_newtable(L);
			addDtorToMt<T>(L);
			{
				lua_pushstring(L, "__name");
				lua_pushstring(L, tn);
				lua_settable(L, -3);
			}
			return inst;
		}

		template <typename T>
		static void addDtorToMt(lua_State* L)
		{
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, [](lua_State* L) -> int
			{
				std::destroy_at((T*)lua_touserdata(L, 1));
				return 0;
			});
			lua_settable(L, -3);
		}

		[[nodiscard]] static const char* getTypename(lua_State* L, int i)
		{
			const char* ret = nullptr;
			if (lua_getmetatable(L, i))
			{
				if (lua_getfield(L, -1, "__name"))
				{
					ret = lua_tostring(L, -1);
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
			}
			return ret;
		}

		static Vector3* pushNewVector3(lua_State* L)
		{
			auto v = pushNewAndBeginMt(Vector3);
			{
				lua_pushstring(L, "__index");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("x"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->x);
						return 1;

					case joaat::hash("y"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->y);
						return 1;

					case joaat::hash("z"):
						lua_pushnumber(L, reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->z);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			{
				lua_pushstring(L, "__newindex");
				lua_pushcfunction(L, [](lua_State* L) -> int
				{
					switch (joaat::hash(luaL_checkstring(L, 2)))
					{
					case joaat::hash("x"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->x = (float)luaL_checknumber(L, 3);
						return 1;

					case joaat::hash("y"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->y = (float)luaL_checknumber(L, 3);
						return 1;

					case joaat::hash("z"):
						reinterpret_cast<Vector3*>(lua_touserdata(L, 1))->z = (float)luaL_checknumber(L, 3);
						return 1;
					}
					return 0;
				});
				lua_settable(L, -3);
			}
			lua_setmetatable(L, -2);
			return v;
		}

		[[nodiscard]] static bool isTypename(lua_State* L, int i, const char* tn)
		{
			auto val_tn = getTypename(L, i);
			return val_tn != nullptr
				&& strcmp(val_tn, tn) == 0
				;
		}

		static void checkTypename(lua_State* L, int i, const char* tn)
		{
			if (!isTypename(L, i, tn))
			{
				luaL_typeerror(L, i, tn);
			}
		}

		static void pushString(lua_State* L, const char* str)
		{
			lua_pushstring(L, str);
		}

		static void pushString(lua_State* L, const std::string& str)
		{
			lua_pushlstring(L, str.data(), str.size());
		}

		static bool isIndexBasedTable(lua_State* L, int i)
		{
			lua_pushvalue(L, i);
			lua_pushnil(L);
			size_t k = 1;
			for (; lua_next(L, -2); ++k)
			{
				lua_pushvalue(L, -2);
				if (lua_type(L, -1) != LUA_TNUMBER)
				{
					lua_pop(L, 4);
					return false;
				}
				if (!lua_isinteger(L, -1))
				{
					lua_pop(L, 4);
					return false;
				}
				if (lua_tointeger(L, -1) != k)
				{
					lua_pop(L, 4);
					return false;
				}
				lua_pop(L, 2);
			}
			lua_pop(L, 1);
			return k != 1; // say it's not an index based table if it's empty
		}

		static UniquePtr<JsonNode> checkJson(lua_State* L, int i)
		{
			auto type = lua_type(L, i);
			if (type == LUA_TBOOLEAN)
			{
				return soup::make_unique<JsonBool>(lua_toboolean(L, i));
			}
			else if (type == LUA_TNUMBER)
			{
				if (lua_isinteger(L, i))
				{
					return soup::make_unique<JsonInt>(lua_tointeger(L, i));
				}
				else
				{
					return soup::make_unique<JsonFloat>(lua_tonumber(L, i));
				}
			}
			else if (type == LUA_TSTRING)
			{
				return soup::make_unique<JsonString>(lua_tostring(L, i));
			}
			else if (type == LUA_TTABLE)
			{
				if (isIndexBasedTable(L, i))
				{
					auto arr = soup::make_unique<JsonArray>();
					lua_pushvalue(L, i);
					lua_pushnil(L);
					while (lua_next(L, -2))
					{
						lua_pushvalue(L, -2);
						arr->children.emplace_back(checkJson(L, -2));
						lua_pop(L, 2);
					}
					lua_pop(L, 1);
					return arr;
				}
				else
				{
					auto obj = soup::make_unique<JsonObject>();
					lua_pushvalue(L, i);
					lua_pushnil(L);
					while (lua_next(L, -2))
					{
						lua_pushvalue(L, -2);
						obj->children.emplace_back(checkJson(L, -1), checkJson(L, -2));
						lua_pop(L, 2);
					}
					lua_pop(L, 1);
					return obj;
				}
			}
			luaL_typeerror(L, i, "JSON-castable type");
		}

		static void pushFromJson(lua_State* L, const JsonNode& node)
		{
			if (node.isBool())
			{
				lua_pushboolean(L, node.asBool().value);
			}
			else if (node.isInt())
			{
				lua_pushinteger(L, node.asInt().value);
			}
			else if (node.isFloat())
			{
				lua_pushnumber(L, node.asFloat().value);
			}
			else if (node.isStr())
			{
				pushString(L, node.asStr().value);
			}
			else if (node.isArr())
			{
				lua_newtable(L);
				lua_Integer i = 1;
				for (const auto& child : node.asArr().children)
				{
					lua_pushinteger(L, i++);
					pushFromJson(L, *child);
					lua_settable(L, -3);
				}
			}
			else if (node.isObj())
			{
				lua_newtable(L);
				for (const auto& e : node.asObj().children)
				{
					pushFromJson(L, *e.first);
					pushFromJson(L, *e.second);
					lua_settable(L, -3);
				}
			}
			else
			{
				lua_pushnil(L);
			}
		}

#pragma endregion Lua Helpers

#undef pushNewAndBeginMt
	};
}
