#pragma once

inline void Lua_Register_spine(lua_State* const& L)
{
	lua_createtable(L, 0, 100);
	lua_pushvalue(L, -1);
	lua_setglobal(L, LuaKey_spine);


	/*

	sample:
	
	skeletonNode = SkeletonAnimation::createWithJsonFile("spine/spineboy-ess.json", "spine/spineboy.atlas", 0.6f);
    skeletonNode->setStartListener( [] (spTrackEntry* entry) {
        log("%d start: %s", entry->trackIndex, entry->animation->name);
    });
    skeletonNode->setInterruptListener( [] (spTrackEntry* entry) {
        log("%d interrupt", entry->trackIndex);
    });
    skeletonNode->setEndListener( [] (spTrackEntry* entry) {
        log("%d end", entry->trackIndex);
    });
    skeletonNode->setCompleteListener( [] (spTrackEntry* entry) {
        log("%d complete", entry->trackIndex);
    });
    skeletonNode->setDisposeListener( [] (spTrackEntry* entry) {
        log("%d dispose", entry->trackIndex);
    });
    skeletonNode->setEventListener( [] (spTrackEntry* entry, spEvent* event) {
        log("%d event: %s, %d, %f, %s", entry->trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
    });
    
    skeletonNode->setMix("walk", "jump", 0.4);
    skeletonNode->setMix("jump", "run", 0.4);
    skeletonNode->setAnimation(0, "walk", true);
    spTrackEntry* jumpEntry = skeletonNode->addAnimation(0, "jump", false, 1);
    skeletonNode->addAnimation(0, "run", true);
    
    skeletonNode->setTrackStartListener(jumpEntry, [] (spTrackEntry* entry) {
        log("jumped!");
    });

	*/

	// todo


	Lua_NewMT(L, TypeNames<spSkeleton*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spBone*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spSlot*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spAttachment*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spSkeletonData*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spAnimationStateData*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spTrackEntry*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spEvent*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spAtlas*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spAnimation*>::value);
	// ...
	lua_pop(L, 1);

	Lua_NewMT(L, TypeNames<spAnimationState*>::value);
	// ...
	lua_pop(L, 1);





	Lua_NewMT(L, TypeNames<spine::SkeletonRenderer*>::value, TypeNames<cocos2d::Node*>::value);

	Lua_NewFunc(L, "getSkeleton", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "getSkeleton error! need 1 args: self");
		auto&& r = std::get<0>(t)->getSkeleton();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTimeScale", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, float>(L, "setTimeScale error! need 2 args: self, float scale");
		std::get<0>(t)->setTimeScale(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getTimeScale", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "getTimeScale error! need 1 args: self");
		auto&& r = std::get<0>(t)->getTimeScale();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDebugSlotsEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, bool>(L, "setDebugSlotsEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setDebugSlotsEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDebugSlotsEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "getDebugSlotsEnabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->getDebugSlotsEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDebugBonesEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, bool>(L, "setDebugBonesEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setDebugBonesEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDebugBonesEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "getDebugBonesEnabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->getDebugBonesEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setDebugMeshesEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, bool>(L, "setDebugMeshesEnabled error! need 2 args: self, bool enabled");
		std::get<0>(t)->setDebugMeshesEnabled(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "getDebugMeshesEnabled", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "getDebugMeshesEnabled error! need 1 args: self");
		auto&& r = std::get<0>(t)->getDebugMeshesEnabled();
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "updateWorldTransform", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "updateWorldTransform error! need 1 args: self");
		std::get<0>(t)->updateWorldTransform();
		return 0;
	});

	Lua_NewFunc(L, "setToSetupPose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "setToSetupPose error! need 1 args: self");
		std::get<0>(t)->setToSetupPose();
		return 0;
	});

	Lua_NewFunc(L, "setBonesToSetupPose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "setBonesToSetupPose error! need 1 args: self");
		std::get<0>(t)->setBonesToSetupPose();
		return 0;
	});

	Lua_NewFunc(L, "setSlotsToSetupPose", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "setSlotsToSetupPose error! need 1 args: self");
		std::get<0>(t)->setSlotsToSetupPose();
		return 0;
	});

	Lua_NewFunc(L, "findBone", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, std::string>(L, "findBone error! need 2 args: self, string boneName");
		auto&& r = std::get<0>(t)->findBone(std::get<1>(t));
		if (!r) return 0;
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "findSlot", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, std::string>(L, "findSlot error! need 2 args: self, string slotName");
		auto&& r = std::get<0>(t)->findSlot(std::get<1>(t));
		if (!r) return 0;
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setSkin", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, char const*>(L, "setSkin error! need 2 args: self, string skinName");
		auto&& r = std::get<0>(t)->setSkin(std::get<1>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "getAttachment", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, std::string, std::string>(L, "getAttachment error! need 3 args: self, string slotName, attachmentName");
		auto&& r = std::get<0>(t)->getAttachment(std::get<1>(t), std::get<2>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setAttachment", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, std::string, char const*>(L, "setAttachment error! need 3 args: self, string slotName, attachmentName");
		auto&& r = std::get<0>(t)->setAttachment(std::get<1>(t), std::get<2>(t));
		return Lua_Pushs(L, r);
	});

	Lua_NewFunc(L, "setTwoColorTint", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, bool>(L, "setTwoColorTint error! need 2 args: self, bool enabled");
		std::get<0>(t)->setTwoColorTint(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "isTwoColorTint", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonRenderer*>(L, "isTwoColorTint error! need 1 args: self");
		auto&& r = std::get<0>(t)->isTwoColorTint();
		return Lua_Pushs(L, r);
	});

	//Lua_NewFunc(L, "setVertexEffect", [](lua_State* L)
	//{
	//	auto&& t = Lua_ToTuple<spine::SkeletonRenderer*, spVertexEffect*>(L, "setVertexEffect error! need 2 args: self, spVertexEffect effect");
	//	std::get<0>(t)->setVertexEffect(std::get<1>(t));
	//	return 0;
	//});

	lua_pop(L, 1);




	Lua_NewMT(L, TypeNames<spine::SkeletonAnimation*>::value, TypeNames<spine::SkeletonRenderer*>::value);

	Lua_NewFunc(L, "createWithData", [](lua_State* L)
	{
		spine::SkeletonAnimation* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<spSkeletonData*>(L);
			auto&& o = spine::SkeletonAnimation::createWithData(std::get<0>(t));
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<spSkeletonData*, bool>(L);
			auto&& o = spine::SkeletonAnimation::createWithData(std::get<0>(t), std::get<1>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithData SkeletonAnimation error! need 1 ~ 2 args: spSkeletonData skeletonData, bool ownsSkeletonData = false");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithJsonFile", [](lua_State* L)
	{
		spine::SkeletonAnimation* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			if (lua_isstring(L, 2))
			{
				auto&& t = Lua_ToTuple<std::string, std::string>(L);
				auto&& o = spine::SkeletonAnimation::createWithJsonFile(std::get<0>(t), std::get<1>(t));
			}
			else
			{
				auto&& t = Lua_ToTuple<std::string, spAtlas*>(L);
				auto&& o = spine::SkeletonAnimation::createWithJsonFile(std::get<0>(t), std::get<1>(t));
			}
			break;
		}
		case 3:
		{
			if (lua_isstring(L, 2))
			{
				auto&& t = Lua_ToTuple<std::string, std::string, float>(L);
				auto&& o = spine::SkeletonAnimation::createWithJsonFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			}
			else
			{
				auto&& t = Lua_ToTuple<std::string, spAtlas*, float>(L);
				auto&& o = spine::SkeletonAnimation::createWithJsonFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithJsonFile SkeletonAnimation error! need 2 ~ 3 args: string skeletonJsonFile, spAtlas* atlas / string atlasFile, float scale = 1");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "createWithBinaryFile", [](lua_State* L)
	{
		spine::SkeletonAnimation* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 2:
		{
			if (lua_isstring(L, 2))
			{
				auto&& t = Lua_ToTuple<std::string, std::string>(L);
				o = spine::SkeletonAnimation::createWithBinaryFile(std::get<0>(t), std::get<1>(t));
			}
			else
			{
				auto&& t = Lua_ToTuple<std::string, spAtlas*>(L);
				o = spine::SkeletonAnimation::createWithBinaryFile(std::get<0>(t), std::get<1>(t));
			}
			break;
		}
		case 3:
		{
			if (lua_isstring(L, 2))
			{
				auto&& t = Lua_ToTuple<std::string, std::string, float>(L);
				o = spine::SkeletonAnimation::createWithBinaryFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			}
			else
			{
				auto&& t = Lua_ToTuple<std::string, spAtlas*, float>(L);
				o = spine::SkeletonAnimation::createWithBinaryFile(std::get<0>(t), std::get<1>(t), std::get<2>(t));
			}
			break;
		}
		default:
			return luaL_error(L, "%s", "createWithBinaryFile SkeletonAnimation error! need 2 ~ 3 args: string skeletonBinaryFile, spAtlas* atlas / string atlasFile, float scale = 1");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "setAnimationStateData", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spAnimationStateData*>(L, "setAnimationStateData error! need 2 args: self, spAnimationStateData stateData");
		std::get<0>(t)->setAnimationStateData(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "setMix", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, std::string, std::string, float>(L, "setMix error! need 2 args: self, string fromAnimation, toAnimation, float duration");
		std::get<0>(t)->setMix(std::get<1>(t), std::get<2>(t), std::get<3>(t));
		return 0;
	});

	Lua_NewFunc(L, "setAnimation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, std::string, bool>(L, "setAnimation error! need 4 args: self, int trackIndex, string name, bool loop");
		auto&& o = std::get<0>(t)->addAnimation(std::get<1>(t), std::get<2>(t), std::get<3>(t));
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "addAnimation", [](lua_State* L)
	{
		spTrackEntry* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, std::string, bool>(L);
			o = std::get<0>(t)->addAnimation(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		case 4:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, std::string, bool, float>(L);
			o = std::get<0>(t)->addAnimation(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "addAnimation error! need 4 ~ 5 args: self, int trackIndex, string name, bool loop, float delay = 0");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "setEmptyAnimation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, float>(L, "setEmptyAnimation error! need 4 args: self, int trackIndex, float mixDuration");
		auto&& o = std::get<0>(t)->setEmptyAnimation(std::get<1>(t), std::get<2>(t));
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "setEmptyAnimations", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, float>(L, "setEmptyAnimations error! need 2 args: self, float mixDuration");
		std::get<0>(t)->setEmptyAnimations(std::get<1>(t));
		return 0;
	});

	Lua_NewFunc(L, "addEmptyAnimation", [](lua_State* L)
	{
		spTrackEntry* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 3:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, float>(L);
			o = std::get<0>(t)->addEmptyAnimation(std::get<1>(t), std::get<2>(t));
			break;
		}
		case 4:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int, float, float>(L);
			o = std::get<0>(t)->addEmptyAnimation(std::get<1>(t), std::get<2>(t), std::get<3>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "addEmptyAnimation error! need 3 ~ 4 args: self, int trackIndex, float mixDuration, float delay = 0");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "findAnimation", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, std::string>(L, "findAnimation error! need 2 args: self, string name");
		auto&& o = std::get<0>(t)->findAnimation(std::get<1>(t));
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "getCurrent", [](lua_State* L)
	{
		spTrackEntry* o = nullptr;
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*>(L);
			o = std::get<0>(t)->getCurrent();
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int>(L);
			o = std::get<0>(t)->getCurrent(std::get<1>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "getCurrent error! need 1 ~ 2 args: self, int trackIndex = 0");
		}
		if (!o) return 0;
		return Lua_Pushs(L, o);
	});

	Lua_NewFunc(L, "clearTracks", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*>(L, "clearTracks error! need 1 args: self");
		std::get<0>(t)->clearTracks();
		return 0;
	});

	Lua_NewFunc(L, "clearTrack", [](lua_State* L)
	{
		auto&& numArgs = lua_gettop(L);
		switch (numArgs)
		{
		case 1:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*>(L);
			std::get<0>(t)->clearTrack();
			break;
		}
		case 2:
		{
			auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, int>(L);
			std::get<0>(t)->clearTrack(std::get<1>(t));
			break;
		}
		default:
			return luaL_error(L, "%s", "clearTrack error! need 1 ~ 2 args: self, int trackIndex = 0");
		}
		return 0;
	});

	Lua_NewFunc(L, "setStartListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setStartListener error! need 2 args: self, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setStartListener([f = std::move(std::get<1>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setInterruptListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setInterruptListener error! need 2 args: self, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setInterruptListener([f = std::move(std::get<1>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setEndListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setEndListener error! need 2 args: self, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setEndListener([f = std::move(std::get<1>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setDisposeListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setDisposeListener error! need 2 args: self, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setDisposeListener([f = std::move(std::get<1>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setCompleteListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setCompleteListener error! need 2 args: self, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setCompleteListener([f = std::move(std::get<1>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setEventListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, Lua_Func>(L, "setEventListener error! need 2 args: self, function<void(spTrackEntry entry, spEvent event)>");
		std::get<0>(t)->setEventListener([f = std::move(std::get<1>(t))](spTrackEntry* entry, spEvent* event)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry, event);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackStartListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackStartListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackStartListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackInterruptListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackInterruptListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackInterruptListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackEndListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackEndListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackEndListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackDisposeListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackDisposeListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackDisposeListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackCompleteListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackCompleteListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackCompleteListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	Lua_NewFunc(L, "setTrackEventListener", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*, spTrackEntry*, Lua_Func>(L, "setTrackEventListener error! need 3 args: self, spTrackEntry entry, function<void(spTrackEntry entry)>");
		std::get<0>(t)->setTrackEventListener(std::get<1>(t), [f = std::move(std::get<2>(t))](spTrackEntry* entry, spEvent* event)
		{
			assert(!lua_gettop(gLua));
			Lua_PCall(gLua, f, entry, event);
			lua_settop(gLua, 0);
		});
		return 0;
	});

	// 不实现 onAnimationStateEvent, onTrackEntryEvent

	Lua_NewFunc(L, "getState", [](lua_State* L)
	{
		auto&& t = Lua_ToTuple<spine::SkeletonAnimation*>(L, "getState error! need 1 args: self");
		auto&& o = std::get<0>(t)->getState();
		return Lua_Pushs(L, o);
	});

	lua_pop(L, 1);




	lua_pop(L, 1);
	assert(lua_gettop(L) == 0);
}
