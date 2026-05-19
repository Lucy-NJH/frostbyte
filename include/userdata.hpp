#pragma once

#include "lua.h"

namespace frostbyte {
    namespace userdata {
        #define USERDATA_TYPES         \
            X(Instance)                \
            X(Enums)                   \
            X(Enum)                    \
            X(EnumItem)                \
            X(RBXScriptConnection)     \
            X(RBXScriptSignal)         \
            X(Color3)                  \
            X(Vector2)                 \
            X(Vector3)                 \
            X(NumberRange)             \
            X(NumberSequenceKeypoint)  \
            X(NumberSequence)          \
            X(ColorSequenceKeypoint)   \
            X(ColorSequence)           \
            X(TweenInfo)               \
            X(Rect)                    \
            X(UDim)                    \
            X(UDim2)                   \
                                       \
            X(InstructionWrapper)      \
            X(DrawEntry)

        #define X(name) name,
        enum UserdataTag {
            SharedPtrObject,
            USERDATA_TYPES
        };
        #undef X

        extern const char* const userdata_names[];

        bool get(lua_State* L, void*& out, int ud, int ttag);
        bool is(lua_State* L, int ud, int ttag);
        void* check(lua_State* L, int ud, int ttag);

        int newClassMetatable(lua_State* L, int ttag);
        int getClassMetatable(lua_State* L, int ttag);
    }
}
