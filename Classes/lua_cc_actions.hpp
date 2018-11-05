#pragma once

inline void Lua_Register_Actions(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Action*>::value, TypeNames<cocos2d::Ref*>::value);

	lua_pop(L, 1);





	Lua_NewMT(L, TypeNames<cocos2d::Speed*>::value, TypeNames<cocos2d::Action*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ActionInterval*, float>(L, "create error! need 2 args: ActionInterval, speed(float)");
		var r = cocos2d::Speed::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);
	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::FiniteTimeAction*>::value, TypeNames<cocos2d::Action*>::value);

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::ActionInterval*>::value, TypeNames<cocos2d::FiniteTimeAction*>::value);

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::Sequence*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		if (!numArgs)
		{
			return luaL_error(L, "create error! need 1+ args: FiniteTimeAction...");
		}

		gActions.clear();
		for (int i = 1; i <= numArgs; ++i)
		{
			cocos2d::FiniteTimeAction* a = nullptr;
			Lua_Get(a, L, i);
			gActions.pushBack(a);
		}
		var o = cocos2d::Sequence::create(gActions);
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::Repeat*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::FiniteTimeAction*, int>(L, "create error! need 2 args: FiniteTimeAction, times(int)");
		var r = cocos2d::Repeat::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);
	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::RepeatForever*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ActionInterval*>(L, "create error! need 1 args: ActionInterval");
		var r = cocos2d::RepeatForever::create(std::get<0>(t));
		return Lua_Push(L, r);
	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::Spawn*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		if (!numArgs)
		{
			return luaL_error(L, "create error! need 1+ args: FiniteTimeAction...");
		}

		gActions.clear();
		for (int i = 1; i <= numArgs; ++i)
		{
			cocos2d::FiniteTimeAction* a = nullptr;
			Lua_Get(a, L, i);
			gActions.pushBack(a);
		}
		var o = cocos2d::Spawn::create(gActions);
		if (!o) return 0;
		return Lua_Push(L, o);

	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::RotateTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create error! need 2 args: duration, dstAngle");
		var r = cocos2d::RotateTo::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, dstAngleX, dstAngleY");
		var r = cocos2d::RotateTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, dstAngleX, dstAngleY, dstAngleZ");
		var r = cocos2d::RotateTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::RotateBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create error! need 2 args: duration, deltaAngle");
		var r = cocos2d::RotateBy::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, deltaAngleX, deltaAngleY");
		var r = cocos2d::RotateBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, deltaAngleX, deltaAngleY, deltaAngleZ");
		var r = cocos2d::RotateBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::MoveBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, deltaX, deltaY");
		var r = cocos2d::MoveBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, deltaX, deltaY, deltaZ");
		var r = cocos2d::MoveBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::MoveTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, dstX, dstY");
		var r = cocos2d::MoveTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, dstX, dstY, dstZ");
		var r = cocos2d::MoveTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::SkewTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, SX, SY");
		var r = cocos2d::SkewTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::SkewBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, deltaSX, deltaSY");
		var r = cocos2d::SkewBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);


}
