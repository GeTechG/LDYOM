#pragma once

#include <sol/sol.hpp> // Для sol::state_view, sol::usertype и др.

#define SOL_LUA_EXPAND(...) __VA_ARGS__

// Вспомогательные макросы для работы с переменным числом аргументов

// 1. SOL_LUA_PP_ARG_N остается таким же: извлекает N-й аргумент после списка заполнителей.
#define SOL_LUA_PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

// 2. SOL_LUA_PP_RSEQ_N становится объектно-подобным макросом (без скобок).
//    Это последовательность чисел, используемая для выбора правильного N.
#define SOL_LUA_PP_RSEQ_N 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// 3. SOL_LUA_PP_NARG_IMPL - это внутренняя реализация, которая вызывает SOL_LUA_PP_ARG_N.
//    SOL_LUA_EXPAND используется для обеспечения правильного раскрытия.
#define SOL_LUA_PP_NARG_IMPL(...) SOL_LUA_EXPAND(SOL_LUA_PP_ARG_N(__VA_ARGS__))

// 4. SOL_LUA_PP_NARG - это основной макрос для подсчета аргументов.
//    Он передает __VA_ARGS__ (список членов) и SOL_LUA_PP_RSEQ_N в SOL_LUA_PP_NARG_IMPL.
//    Поскольку SOL_LUA_PP_RSEQ_N теперь объектно-подобный макрос, он должен раскрыться здесь.
#define SOL_LUA_PP_NARG(...) SOL_LUA_PP_NARG_IMPL(__VA_ARGS__, SOL_LUA_PP_RSEQ_N)

#define SOL_LUA_PASTE_INTERNAL(a, b) a##b
#define SOL_LUA_PASTE(a, b) SOL_LUA_PASTE_INTERNAL(a, b)

// Добавляем определение для SOL_LUA_FOR_EACH_0 для случая с 0 аргументов
#define SOL_LUA_FOR_EACH_0(what, usertype, ClassType) /* intentionally empty */
#define SOL_LUA_FOR_EACH_1(what, usertype, ClassType, x) what(usertype, ClassType, x)
#define SOL_LUA_FOR_EACH_2(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_1(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_3(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_2(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_4(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_3(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_5(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_4(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_6(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_5(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_7(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_6(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_8(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_7(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_9(what, usertype, ClassType, x, ...)                                                          \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_8(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_10(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_9(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_11(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_10(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_12(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_11(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_13(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_12(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_14(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_13(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_15(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_14(what, usertype, ClassType, __VA_ARGS__))
#define SOL_LUA_FOR_EACH_16(what, usertype, ClassType, x, ...)                                                         \
	what(usertype, ClassType, x);                                                                                      \
	SOL_LUA_EXPAND(SOL_LUA_FOR_EACH_15(what, usertype, ClassType, __VA_ARGS__))
// При необходимости можно добавить больше SOL_LUA_FOR_EACH_N макросов для поддержки большего количества членов.

#define SOL_LUA_FOR_EACH(what, usertype, ClassType, ...)                                                               \
	SOL_LUA_EXPAND(                                                                                                    \
		SOL_LUA_PASTE(SOL_LUA_FOR_EACH_, SOL_LUA_PP_NARG(__VA_ARGS__))(what, usertype, ClassType, __VA_ARGS__))

// Макрос для привязки одного члена класса
#define SOL_LUA_BIND_MEMBER_ACTION(usertype, ClassType, member_name) usertype[#member_name] = &ClassType::member_name;

/**
 * @brief Определяет статическую функцию sol_lua_register для класса,
 *        которая регистрирует указанные члены класса в Lua с использованием Sol2.
 *
 * @param Type Имя класса.
 * @param LuaTypeName Строковое имя типа, под которым он будет известен в Lua.
 * @param ... Список имен членов класса для регистрации.
 *
 * Пример использования внутри класса:
 * SOL_LUA_DEFINE_TYPE_INTRUSIVE(MyClass, member1, member2)
 */
#define SOL_LUA_DEFINE_TYPE_INTRUSIVE(Type, ...)                                                                       \
	static void sol_lua_register(sol::state_view lua_state) {                                                          \
		auto ut = lua_state.new_usertype<Type>(#Type);                                                                 \
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Type, __VA_ARGS__)                                            \
	}