#pragma once

inline void Lua_Register_uiWidget(lua_State* const& L)
{
	Lua_NewMT(L, TypeNames<cocos2d::ui::Widget*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "setEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBright", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setBright error! need 2 args: self, bool bright");
		std::get<0>(t)->setBright(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isBright", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isBright error! need 1 args: self");
		var r = std::get<0>(t)->isBright();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTouchEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setTouchEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setTouchEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isTouchEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isTouchEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isTouchEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setBrightStyle", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, cocos2d::ui::Widget::BrightStyle>(L, "setBrightStyle error! need 2 args: self, BrightStyle style");
		std::get<0>(t)->setBrightStyle(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setHighlighted", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setHighlighted error! need 2 args: self, bool highlight");
		std::get<0>(t)->setHighlighted(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isHighlighted", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isHighlighted error! need 1 args: self");
		var r = std::get<0>(t)->isHighlighted();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getLeftBoundary", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getLeftBoundary error! need 1 args: self");
		var r = std::get<0>(t)->getLeftBoundary();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getBottomBoundary", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getBottomBoundary error! need 1 args: self");
		var r = std::get<0>(t)->getBottomBoundary();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getRightBoundary", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getRightBoundary error! need 1 args: self");
		var r = std::get<0>(t)->getRightBoundary();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getTopBoundary", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getTopBoundary error! need 1 args: self");
		var r = std::get<0>(t)->getTopBoundary();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "addTouchEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "addTouchEventListener error! need 2 args: self, func<Ref, TouchEventType>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->addTouchEventListener([f = std::move(std::get<1>(t))](cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type)
			{
				Lua_PCall(gLua, f, ref, type);
			});
		}
		else
		{
			std::get<0>(t)->addTouchEventListener(nullptr);
		}
		return 0;
	});

	Lua_NewFunc(L, "addClickEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "addClickEventListener error! need 2 args: self, func<Ref>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->addClickEventListener([f = std::move(std::get<1>(t))](cocos2d::Ref* ref)
			{
				Lua_PCall(gLua, f, ref);
			});
		}
		else
		{
			std::get<0>(t)->addClickEventListener(nullptr);
		}
		return 0;
	});

	Lua_NewFunc(L, "addCCSEventListener", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "addCCSEventListener error! need 2 args: self, func<int>/null");
		if (std::get<1>(t))
		{
			std::get<0>(t)->addCCSEventListener([f = std::move(std::get<1>(t))](cocos2d::Ref* ref, int e)
			{
				Lua_PCall(gLua, f, ref, e);
			});
		}
		else
		{
			std::get<0>(t)->addCCSEventListener(nullptr);
		}
		return 0;
	});

	Lua_NewFunc(L, "setPositionPercent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, float, float>(L, "setPositionPercent error! need 3 args: self, float percentX, float percentY");
		std::get<0>(t)->setPositionPercent({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "getPositionPercent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getPositionPercent error! need 1 args: self");
		var r = std::get<0>(t)->getPositionPercent();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setPositionType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, cocos2d::ui::Widget::PositionType>(L, "setPositionType error! need 2 args: self, PositionType type");
		std::get<0>(t)->setPositionType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getPositionType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getPositionType error! need 1 args: self");
		var r = std::get<0>(t)->getPositionType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "isFlippedX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isFlippedX error! need 1 args: self");
		var r = std::get<0>(t)->isFlippedX();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFlippedX", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setFlippedX error! need 2 args: self, bool flippedX");
		std::get<0>(t)->setFlippedX(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isFlippedY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isFlippedY error! need 1 args: self");
		var r = std::get<0>(t)->isFlippedY();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFlippedY", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setFlippedY error! need 2 args: self, bool flippedY");
		std::get<0>(t)->setFlippedY(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isClippingParentContainsPoint", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, float, float>(L, "isClippingParentContainsPoint error! need 3 args: self, float ptX, float ptY");
		var r = std::get<0>(t)->isClippingParentContainsPoint({ std::get<1>(t), std::get<2>(t) });
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getTouchBeganPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getTouchBeganPosition error! need 1 args: self");
		var r = std::get<0>(t)->getTouchBeganPosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getTouchMovePosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getTouchMovePosition error! need 1 args: self");
		var r = std::get<0>(t)->getTouchMovePosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getTouchEndPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getTouchEndPosition error! need 1 args: self");
		var r = std::get<0>(t)->getTouchEndPosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "setSizePercent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, float, float>(L, "setSizePercent error! need 2 args: self, float percentX, float percentY");
		std::get<0>(t)->setSizePercent({ std::get<1>(t), std::get<2>(t) });
		return 0;
	});

	Lua_NewFunc(L, "setSizeType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, cocos2d::ui::Widget::SizeType>(L, "setSizeType error! need 2 args: self, SizeType type");
		std::get<0>(t)->setSizeType(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getSizeType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, cocos2d::ui::Widget::SizeType>(L, "getSizeType error! need 1 args: self");
		var r = std::get<0>(t)->getSizeType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getCustomSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getCustomSize error! need 1 args: self");
		var r = std::get<0>(t)->getCustomSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getLayoutSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getLayoutSize error! need 1 args: self");
		var r = std::get<0>(t)->getLayoutSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "getSizePercent", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getSizePercent error! need 1 args: self");
		var r = std::get<0>(t)->getSizePercent();
		return Lua_Pushs(L, r.x, r.y);
	});

	// todo:
	// hitTest
	// setLayoutParameter
	// getLayoutParameter

	Lua_NewFunc(L, "ignoreContentAdaptWithSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "ignoreContentAdaptWithSize error! need 2 args: self, bool ignore");
		std::get<0>(t)->ignoreContentAdaptWithSize(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isIgnoreContentAdaptWithSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isIgnoreContentAdaptWithSize error! need 1 args: self");
		var r = std::get<0>(t)->isIgnoreContentAdaptWithSize();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getWorldPosition", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getWorldPosition error! need 1 args: self");
		var r = std::get<0>(t)->getWorldPosition();
		return Lua_Pushs(L, r.x, r.y);
	});

	Lua_NewFunc(L, "getVirtualRenderer", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getVirtualRenderer error! need 1 args: self");
		var r = std::get<0>(t)->getVirtualRenderer();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getVirtualRendererSize", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getVirtualRendererSize error! need 1 args: self");
		var r = std::get<0>(t)->getVirtualRendererSize();
		return Lua_Pushs(L, r.width, r.height);
	});

	Lua_NewFunc(L, "updateSizeAndPosition", [](lua_State* L)
	{
		var numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			var t = Lua_ToTuple<cocos2d::ui::Widget*>(L);
			std::get<0>(t)->updateSizeAndPosition();
			return 0;
		}
		case 3:
		{
			var t = Lua_ToTuple<cocos2d::ui::Widget*, float, float>(L);
			std::get<0>(t)->updateSizeAndPosition({ std::get<1>(t), std::get<2>(t) });
			return 0;
		}
		default:
			return luaL_error(L, "%s", "updateSizeAndPosition error! need 1, 3 args: self, float parentSizeX, float parentSizeY");
		}
	});

	Lua_NewFunc(L, "setActionTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, int>(L, "setActionTag error! need 2 args: self, int tag");
		std::get<0>(t)->setActionTag(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getActionTag", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getActionTag error! need 1 args: self");
		var r = std::get<0>(t)->getActionTag();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setPropagateTouchEvents", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setPropagateTouchEvents error! need 2 args: self, bool isPropagate");
		std::get<0>(t)->setPropagateTouchEvents(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isPropagateTouchEvents", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isPropagateTouchEvents error! need 1 args: self");
		var r = std::get<0>(t)->isPropagateTouchEvents();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setSwallowTouches error! need 2 args: self, bool swallow");
		std::get<0>(t)->setSwallowTouches(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isSwallowTouches", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isSwallowTouches error! need 1 args: self");
		var r = std::get<0>(t)->isSwallowTouches();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFocused", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setFocused error! need 2 args: self, bool focus");
		std::get<0>(t)->setFocused(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isFocused", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isFocused error! need 1 args: self");
		var r = std::get<0>(t)->isFocused();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setFocusEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setFocusEnabled error! need 2 args: self, bool focus");
		std::get<0>(t)->setFocusEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isFocusEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isFocusEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isFocusEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "findNextFocusedWidget", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, cocos2d::ui::Widget::FocusDirection, cocos2d::ui::Widget*>(L, "findNextFocusedWidget error! need 3 args: self, FocusDirection direction, Widget current");
		var r = std::get<0>(t)->findNextFocusedWidget(std::get<1>(t), std::get<2>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "requestFocus", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "requestFocus error! need 1 args: self");
		std::get<0>(t)->requestFocus();
		return 0;
	});

	Lua_NewFunc(L, "getCurrentFocusedWidget", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getCurrentFocusedWidget error! need 1 args: self");
		var r = std::get<0>(t)->getCurrentFocusedWidget();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "enableDpadNavigation", [](lua_State* L)
	{
		var t = Lua_ToTuple<bool>(L, "enableDpadNavigation error! need 1 args: bool enable");
		cocos2d::ui::Widget::enableDpadNavigation(std::get<0>(t));
		return 0;
	});

	Lua_NewFunc(L, "onFocusChanged", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "onFocusChanged error! need 2 args: self, function<void(Widget*,Widget*)>");
		std::get<0>(t)->onFocusChanged = [f = std::move(std::get<1>(t))](cocos2d::ui::Widget* from, cocos2d::ui::Widget* to)
		{
			Lua_PCall(gLua, f, from, to);
		};
		return 0;
	});

	Lua_NewFunc(L, "onNextFocusedWidget", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, Lua_Func>(L, "onNextFocusedWidget error! need 2 args: self, function<Widget*(FocusDirection)>");
		std::get<0>(t)->onNextFocusedWidget = [f = std::move(std::get<1>(t))](cocos2d::ui::Widget::FocusDirection fd)->cocos2d::ui::Widget*
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, fd);
			var t = Lua_ToTuple<cocos2d::ui::Widget*>(gLua, "onNextFocusedWidget return error! need return 1 result: Widget");
			lua_settop(gLua, 0);
			return std::get<0>(t);
		};
		return 0;
	});

	Lua_NewFunc(L, "setUnifySizeEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setUnifySizeEnabled error! need 2 args: self, bool enable");
		std::get<0>(t)->setUnifySizeEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isUnifySizeEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isUnifySizeEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isUnifySizeEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setCallbackName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, std::string>(L, "setCallbackName error! need 2 args: self, string callbackName");
		std::get<0>(t)->setCallbackName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getCallbackName", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getCallbackName error! need 1 args: self");
		var r = std::get<0>(t)->getCallbackName();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setCallbackType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, std::string>(L, "setCallbackType error! need 2 args: self, string callbackType");
		std::get<0>(t)->setCallbackName(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getCallbackType", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "getCallbackType error! need 1 args: self");
		var r = std::get<0>(t)->getCallbackType();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setLayoutComponentEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*, bool>(L, "setLayoutComponentEnabled error! need 2 args: self, bool enable");
		std::get<0>(t)->setLayoutComponentEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isLayoutComponentEnabled", [](lua_State* L)
	{
		var t = Lua_ToTuple<cocos2d::ui::Widget*>(L, "isLayoutComponentEnabled error! need 1 args: self");
		var r = std::get<0>(t)->isLayoutComponentEnabled();
		return Lua_Pushs(L, r);
	});


	lua_pop(L, 1);


	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::FocusDirection>::value);
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "LEFT");	lua_pushinteger(L, (int)cocos2d::ui::Widget::FocusDirection::LEFT);	lua_rawset(L, -3);
	lua_pushstring(L, "RIGHT");	lua_pushinteger(L, (int)cocos2d::ui::Widget::FocusDirection::RIGHT);	lua_rawset(L, -3);
	lua_pushstring(L, "UP");	lua_pushinteger(L, (int)cocos2d::ui::Widget::FocusDirection::UP);	lua_rawset(L, -3);
	lua_pushstring(L, "DOWN");	lua_pushinteger(L, (int)cocos2d::ui::Widget::FocusDirection::DOWN);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::PositionType>::value);
	lua_createtable(L, 2, 0);
	lua_pushstring(L, "ABSOLUTE");	lua_pushinteger(L, (int)cocos2d::ui::Widget::PositionType::ABSOLUTE);	lua_rawset(L, -3);
	lua_pushstring(L, "PERCENT");	lua_pushinteger(L, (int)cocos2d::ui::Widget::PositionType::PERCENT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::SizeType>::value);
	lua_createtable(L, 2, 0);
	lua_pushstring(L, "ABSOLUTE");	lua_pushinteger(L, (int)cocos2d::ui::Widget::SizeType::ABSOLUTE);	lua_rawset(L, -3);
	lua_pushstring(L, "PERCENT");	lua_pushinteger(L, (int)cocos2d::ui::Widget::SizeType::PERCENT);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::TouchEventType>::value);
	lua_createtable(L, 4, 0);
	lua_pushstring(L, "BEGAN");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::BEGAN);	lua_rawset(L, -3);
	lua_pushstring(L, "MOVED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::MOVED);	lua_rawset(L, -3);
	lua_pushstring(L, "ENDED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::ENDED);	lua_rawset(L, -3);
	lua_pushstring(L, "CANCELED");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TouchEventType::CANCELED);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::TextureResType>::value);
	lua_createtable(L, 2, 0);
	lua_pushstring(L, "LOCAL");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TextureResType::LOCAL);	lua_rawset(L, -3);
	lua_pushstring(L, "PLIST");	lua_pushinteger(L, (int)cocos2d::ui::Widget::TextureResType::PLIST);	lua_rawset(L, -3);
	lua_rawset(L, -3);

	lua_pushstring(L, TypeNames<cocos2d::ui::Widget::BrightStyle>::value);
	lua_createtable(L, 3, 0);
	lua_pushstring(L, "NONE");	lua_pushinteger(L, (int)cocos2d::ui::Widget::BrightStyle::NONE);	lua_rawset(L, -3);
	lua_pushstring(L, "NORMAL");	lua_pushinteger(L, (int)cocos2d::ui::Widget::BrightStyle::NORMAL);	lua_rawset(L, -3);
	lua_pushstring(L, "HIGHLIGHT");	lua_pushinteger(L, (int)cocos2d::ui::Widget::BrightStyle::HIGHLIGHT);	lua_rawset(L, -3);
	lua_rawset(L, -3);
}
