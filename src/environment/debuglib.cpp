#include "environment/debuglib.hpp"
#include "common.hpp"
#include "environment.hpp"

#include "lapi.h"
#include "lgc.h"
#include "lobject.h"
#include "lstate.h"
#include "lua.h"
#include "lualib.h"

namespace frostbyte {

// from ldebug.cpp's  lua_getinfo
Closure* levelToClosure(lua_State* L, int level) {
    Closure* f = nullptr;
    CallInfo* ci = nullptr;

    if (level < 0) {
        // element has to be within stack
        if (-level > L->top - L->base)
            return 0;

        StkId func = L->top + level;

        // and it has to be a function
        if (!ttisfunction(func))
            return 0;

        f = clvalue(func);
    } else if (unsigned(level) < unsigned(L->ci - L->base_ci)) {
        ci = L->ci - level;
        LUAU_ASSERT(ttisfunction(ci->func));
        f = clvalue(ci->func);
    }

    return f;
}

int checkStackLevel(lua_State* L, int level) {
    if (level < 1) {
        lua_pushstring(L, "stack level cannot be less than 1");
        lua_error(L);
    } else if (level >= lua_stackdepth(L)) {
        lua_pushstring(L, "stack level is too high");
        lua_error(L);
    }
    return 1;
}

Closure* getClosure(lua_State* L, int index) {
    Closure* f = nullptr;

    int type = lua_type(L, index);

    switch (type) {
        case LUA_TNUMBER: {
            int level = lua_tointeger(L, 1);
            checkStackLevel(L, level);

            f = levelToClosure(L, level);
            break;
        } case LUA_TFUNCTION:
            f = clvalue(luaA_toobject(L, index));
            break;
        default:
            luaL_error(L, "invalid argument #%d to %s (expected number or function, got %s)", index, currfuncname(L), lua_typename(L, type));
            break;
    }

    return f;
}

void checkLClosure(lua_State* L, int narg, Closure* closure) {
    if (closure->isC)
        luaL_error(L, "invalid argument #%d to %s (expected Lua closure, got C closure)", narg, currfuncname(L));
}

int fr_debug_getconstant(lua_State* L) {
    Closure* closure = getClosure(L, 1);
    checkLClosure(L, 1, closure);
    Proto* p = closure->l.p;

    int index = luaL_checknumberrange(L, 2, 1, p->sizek, "index");

    luaA_pushobject(L, &p->k[index - 1]);

    return 1;
}
int fr_debug_getconstants(lua_State* L) {
    Closure* closure = getClosure(L, 1);
    checkLClosure(L, 1, closure);
    Proto* p = closure->l.p;

    lua_createtable(L, p->sizek, 0);
    int table = lua_absindex(L, -1);

    for (int i = 0; i < p->sizek; i++) {
        lua_pushnumber(L, i + 1);
        luaA_pushobject(L, &p->k[i]);
        lua_settable(L, table);
    }

    return 1;
}
int fr_debug_setconstant(lua_State* L) {
    Closure* closure = getClosure(L, 1);
    checkLClosure(L, 1, closure);
    Proto* p = closure->l.p;

    int index = luaL_checknumberrange(L, 2, 1, p->sizek, "index");
    luaL_checkany(L, 3);

    int valuet = lua_type(L, 3);
    if (valuet == LUA_TNIL || valuet == LUA_TBOOLEAN || valuet == LUA_TNUMBER ||
        valuet == LUA_TVECTOR || valuet == LUA_TSTRING || valuet == LUA_TTABLE ||
        valuet == LUA_TFUNCTION)
    {
        setobj(L, &p->k[index - 1], luaA_toobject(L, 3));
    } else
        luaL_error(L, "invalid argument #3 to getconstants (expected nil, boolean, number, vector, string, table, or function, got %s", lua_typename(L, valuet));

    return 0;
}

void open_debuglib(lua_State* L) {
    lua_getglobal(L, "debug");

    setfunctionfield(L, fr_getreg, "getregistry");

    setfunctionfield(L, fr_debug_getconstant, "getconstant");
    setfunctionfield(L, fr_debug_getconstants, "getconstants");
    setfunctionfield(L, fr_debug_setconstant, "setconstant");

    lua_pop(L, 1);
}

}
