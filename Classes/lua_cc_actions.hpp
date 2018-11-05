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







	Lua_NewMT(L, TypeNames<cocos2d::ActionInstant*>::value, TypeNames<cocos2d::FiniteTimeAction*>::value);

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










	Lua_NewMT(L, TypeNames<cocos2d::ResizeTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, finalWidth, finalHeight");
		var r = cocos2d::ResizeTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::ResizeBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, deltaWidth, deltaHeight");
		var r = cocos2d::ResizeBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::JumpBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, int>(L, "create error! need 5 args: duration, x, y, height, jumps(int)");
		var r = cocos2d::JumpBy::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::JumpTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, int>(L, "create error! need 5 args: duration, x, y, height, jumps(int)");
		var r = cocos2d::JumpTo::create(std::get<0>(t), { std::get<1>(t), std::get<2>(t) }, std::get<3>(t), std::get<4>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::BezierBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, float, float, float>(L, "create error! need 7 args: duration, endX, endY, c1X, c1Y, c2X, c2Y");
		var r = cocos2d::BezierBy::create(std::get<0>(t), { { std::get<1>(t), std::get<2>(t) }, { std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) } });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::BezierTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float, float, float, float>(L, "create error! need 7 args: duration, endX, endY, c1X, c1Y, c2X, c2Y");
		var r = cocos2d::BezierTo::create(std::get<0>(t), { { std::get<1>(t), std::get<2>(t) }, { std::get<3>(t), std::get<4>(t) }, { std::get<5>(t), std::get<6>(t) } });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::ScaleTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create error! need 2 args: duration, s");
		var r = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, sX, sY");
		var r = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, sX, sY, sZ");
		var r = cocos2d::ScaleTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::ScaleBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create error! need 2 args: duration, s");
		var r = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXY", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float>(L, "create error! need 3 args: duration, sX, sY");
		var r = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return Lua_Push(L, r);

	});

	Lua_NewFunc(L, "createXYZ", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float, float, float>(L, "create error! need 4 args: duration, sX, sY, sZ");
		var r = cocos2d::ScaleBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::Blink*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int>(L, "create error! need 2 args: duration, times");
		var r = cocos2d::Blink::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::FadeTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int>(L, "create error! need 2 args: duration, opacity(0-255)");
		var r = cocos2d::FadeTo::create(std::get<0>(t), std::get<1>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::FadeIn*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create error! need 1 args: duration");
		var r = cocos2d::FadeIn::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::FadeOut*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create error! need 1 args: duration");
		var r = cocos2d::FadeOut::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::TintTo*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int, int, int>(L, "create error! need 4 args: duration, red(0-255), green(0-255), blue(0-255)");
		var r = cocos2d::TintTo::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);








	Lua_NewMT(L, TypeNames<cocos2d::TintBy*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, int, int, int>(L, "create error! need 4 args: duration, deltaRed(0-255), deltaGreen(0-255), deltaBlue(0-255)");
		var r = cocos2d::TintBy::create(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::DelayTime*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create error! need 1 args: duration");
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
		var t = Lua_ToTuple<float>(L, "create error! need 1 args: x");
		var r = cocos2d::FlipX::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::FlipY*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float>(L, "create error! need 1 args: y");
		var r = cocos2d::FlipY::create(std::get<0>(t));
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









	Lua_NewMT(L, TypeNames<cocos2d::Place*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<float, float>(L, "create error! need 1 args: x, y");
		var r = cocos2d::Place::create({ std::get<0>(t), std::get<1>(t) });
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);










	Lua_NewMT(L, TypeNames<cocos2d::CallFunc*>::value, TypeNames<cocos2d::ActionInterval*>::value);

	Lua_NewFunc(L, "create", [](lua_State* L)
	{
		var t = Lua_ToTuple<Lua_Func>(L, "create error! need 1 args: func");
		if (!std::get<0>(t))
		{
			return luaL_error(L, "create error! need 1 args: func");
		}
		var r = cocos2d::CallFunc::create([f = std::move(std::get<0>(t))]
		{
			Lua_PCall(gLua, f);
		});
		return Lua_Push(L, r);

	});

	lua_pop(L, 1);









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

}
