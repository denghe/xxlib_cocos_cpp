#pragma once

inline void Lua_Register_Actions(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::Action*>::value, TypeNames<cocos2d::Ref*>::value);

	lua_pop(L, 1);





	Lua_NewMT(L, TypeNames<cocos2d::Speed*>::value, TypeNames<cocos2d::Action*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ActionInterval*, float>(L, "create Speed error! need 2 args: ActionInterval* action, float speed");
		var r = cocos2d::Speed::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);
	});

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::FiniteTimeAction*>::value, TypeNames<cocos2d::Action*>::value);

	lua_pop(L, 1);






	Lua_NewMT(L, TypeNames<cocos2d::ActionInterval*>::value, TypeNames<cocos2d::FiniteTimeAction*>::value);

	lua_pop(L, 1);







	Lua_NewMT(L, TypeNames<cocos2d::ActionInstant*>::value, TypeNames<cocos2d::FiniteTimeAction*>::value);

	lua_pop(L, 1);







	Lua_NewMT(L, TypeNames<cocos2d::Sequence*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		if (!numArgs)
		{
			return luaL_error(L, "create Sequence error! need 1+ args: FiniteTimeAction*...");
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
		var t = Lua_ToTuple<cocos2d::FiniteTimeAction*, int>(L, "create Repeat error! need 2 args: FiniteTimeAction*, int times");
		var r = cocos2d::Repeat::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);
	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::RepeatForever*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ActionInterval*>(L, "create RepeatForever error! need 1 args: ActionInterval*");
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
			return luaL_error(L, "create Spawn error! need 1+ args: FiniteTimeAction*...");
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
		cocos2d::RotateTo* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<float, float>(L);
			o = cocos2d::RotateTo::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::RotateTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::RotateTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		default:
			return luaL_error(L, "create RotateTo error! need 2 ~ 4 args: float duration, float dstAngleX, float dstAngleY, float dstAngleZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::RotateBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::RotateBy* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<float, float>(L);
			o = cocos2d::RotateBy::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::RotateBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::RotateBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		default:
			return luaL_error(L, "create RotateBy error! need 2 ~ 4 args: float duration, float deltaAngleX, float deltaAngleY, float deltaAngleZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::MoveBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::MoveBy* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::MoveBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::MoveBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		default:
			return luaL_error(L, "create MoveBy error! need 3 ~ 4 args: float duration, float deltaX, float deltaY, float deltaZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::MoveTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::MoveTo* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::MoveTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::MoveTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t), std::get<3>(t) });
			break;
		}
		default:
			return luaL_error(L, "create MoveTo error! need 3 ~ 4 args: float duration, float dstX, float dstY, float dstZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::SkewTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create SkewTo error! need 3 args: float duration, float SX, float SY");
		var r = cocos2d::SkewTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::SkewBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create SkewBy error! need 3 args: float duration, float deltaSX, float deltaSY");
		var r = cocos2d::SkewBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::ResizeTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create ResizeTo error! need 3 args: float duration, float finalWidth, float finalHeight");
		var r = cocos2d::ResizeTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::ResizeBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create ResizeBy error! need 3 args: float duration, float deltaWidth, float deltaHeight");
		var r = cocos2d::ResizeBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::JumpBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, int>(L, "create JumpBy error! need 5 args: float duration, float x, float y, float height, int times");
		var r = cocos2d::JumpBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::JumpTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, int>(L, "create JumpTo error! need 5 args: float duration, float x, float y, float height, int times");
		var r = cocos2d::JumpTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::BezierBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, float, float, float>(L, "create BezierBy error! need 7 args: float duration, float endX, float endY, float c1X, float c1Y, float c2X, float c2Y");
		var r = cocos2d::BezierBy::create(std::get<0>(t), { { std::get<1>(t), std::get<2>(t) }, { std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) } });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::BezierTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, float, float, float>(L, "create BezierTo error! need 7 args: float duration, float endX, float endY, float c1X, float c1Y, float c2X, float c2Y");
		var r = cocos2d::BezierTo::create(std::get<0>(t), { { std::get<1>(t), std::get<2>(t) }, { std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) } });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::ScaleTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ScaleTo* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<float, float>(L);
			o = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "create ScaleTo error! need 2 ~ 4 args: float duration, float sXY/sX, float sY, float sZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::ScaleBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		cocos2d::ScaleBy* o = nullptr;
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			var t = Lua_ToTuple<float, float>(L);
			o = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t));
			break;
		}
		case 3:
		{
			var t = Lua_ToTuple<float, float, float>(L);
			o = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			var t = Lua_ToTuple<float, float, float, float>(L);
			o = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "create ScaleBy error! need 2 ~ 4 args: float duration, float sXY/sX, float sY, float sZ");
		}
		if (!o) return 0;
		return Lua_Push(L, o);
	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::Blink*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int>(L, "create Blink error! need 2 args: float duration, int times");
		var r = cocos2d::Blink::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::FadeTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int>(L, "create FadeTo error! need 2 args: float duration, byte opacity");
		var r = cocos2d::FadeTo::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::FadeIn*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create FadeIn error! need 1 args: float duration");
		var r = cocos2d::FadeIn::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::FadeOut*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create FadeOut error! need 1 args: float duration");
		var r = cocos2d::FadeOut::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::TintTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int, int, int>(L, "create TintTo error! need 4 args: float duration, byte red, byte green, byte blue");
		var r = cocos2d::TintTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::TintBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int, int, int>(L, "create TintBy error! need 4 args: float duration, byte deltaRed, byte deltaGreen, byte deltaBlue");
		var r = cocos2d::TintBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::DelayTime*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create DelayTime error! need 1 args: float duration");
		var r = cocos2d::DelayTime::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::Show*>::value, TypeNames<cocos2d::ActionInstant*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var r = cocos2d::Show::create();
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::Hide*>::value, TypeNames<cocos2d::ActionInstant*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var r = cocos2d::Hide::create();
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::ToggleVisibility*>::value, TypeNames<cocos2d::ActionInstant*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var r = cocos2d::ToggleVisibility::create();
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::RemoveSelf*>::value, TypeNames<cocos2d::ActionInstant*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var r = cocos2d::RemoveSelf::create();
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::FlipX*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "create FlipX error! need 1 args: bool");
		var r = cocos2d::FlipX::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::FlipY*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "create FlipY error! need 1 args: bool");
		var r = cocos2d::FlipY::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::Place*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create Place error! need 2 args: x, y");
		var r = cocos2d::Place::create({ std::get<0>(t), std::get<1>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::CallFunc*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "create CallFunc error! need 1 args: func");
		if (!std::get<0>(t))
		{
			return luaL_error(L, "create CallFunc error! need 1 args: func");
		}
		var r = cocos2d::CallFunc::create([f = std::move(std::get<0>(t))]
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f);
			lua_settop(gLua, 0);
		});
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);







	// todo

	//Lua_NewMT(L, TypeNames<cocos2d::Animation*>::value, TypeNames<cocos2d::Ref*>::value);

	//Lua_NewFunc(L, "create", [](lua_State* L)
	//{
	//	var r = cocos2d::Animation::create();
	//	return Lua_Push(L, r);

	//});

	// addSpriteFrame
	// addSpriteFrameWithFile
	// setDelayPerUnit

	//lua_pop(L, 1);









	//Lua_NewMT(L, TypeNames<cocos2d::Animate*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	//Lua_NewFunc(L, "create", [](lua_State* L)
	//{
	//	var t = Lua_ToTuple<cocos2d::Animation*>(L, "create error! need 1 args: Animation");
	//	var r = cocos2d::Animate::create(std::get<0>(t));
	//	return Lua_Push(L, r);

	//});

	//lua_pop(L, 1);







	// EaseXXXXXXXXXXXXXXXXXXXXX

	// .......
}
