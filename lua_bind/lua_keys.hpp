﻿#pragma once

inline const char* const LuaKey_null = "null";

inline const char* const LuaKey_xx = "xx";
inline const char* const LuaKey_Object = "Object";
inline const char* const LuaKey_Uv = "Uv";

inline const char* const LuaKey_Callbacks = "AllCallbacks";

inline const char* const LuaKey_cc = "cc";
inline const char* const LuaKey_cca = "cca";
inline const char* const LuaKey_spine = "spine";
inline const char* const LuaKey_sys = "sys";
inline const char* const LuaKey_ext = "ext";


struct Lua_BBuffer;
struct Lua_Func;

template<typename T>
struct TypeNames;


template<>
struct TypeNames<int>
{
	inline static const char* value = "int";
};
template<>
struct TypeNames<int64_t>
{
	inline static const char* value = "int64";
};
template<>
struct TypeNames<float>
{
	inline static const char* value = "float";
};
template<>
struct TypeNames<double>
{
	inline static const char* value = "double";
};
template<>
struct TypeNames<bool>
{
	inline static const char* value = "bool";
};
template<>
struct TypeNames<const char*>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<char*>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<std::string>
{
	inline static const char* value = "string";
};
template<>
struct TypeNames<std::vector<std::string>>
{
	inline static const char* value = "strings";
};
template<>
struct TypeNames<Lua_Func>
{
	inline static const char* value = "func";
};



template<>
struct TypeNames<cocos2d::Data*>
{
	inline static const char* value = "Data";
};
template<>
struct TypeNames<cocos2d::Ref*>
{
	inline static const char* value = "Ref";
};
template<>
struct TypeNames<cocos2d::Animation*>
{
	inline static const char* value = "Animation";
};
template<>
struct TypeNames<cocos2d::AnimationFrame*>
{
	inline static const char* value = "AnimationFrame";
};
template<>
struct TypeNames<cocos2d::AnimationCache*>
{
	inline static const char* value = "AnimationCache";
};
template<>
struct TypeNames<cocos2d::extension::AssetsManagerEx*>
{
	inline static const char* value = "AssetsManagerEx";
};
template<>
struct TypeNames<cocos2d::Action*>
{
	inline static const char* value = "Action";
};
template<>
struct TypeNames<cocos2d::Speed*>
{
	inline static const char* value = "Speed";
};
template<>
struct TypeNames<cocos2d::FiniteTimeAction*>
{
	inline static const char* value = "FiniteTimeAction";
};
template<>
struct TypeNames<cocos2d::ActionInstant*>
{
	inline static const char* value = "ActionInstant";
};
template<>
struct TypeNames<cocos2d::ActionInterval*>
{
	inline static const char* value = "ActionInterval";
};
template<>
struct TypeNames<cocos2d::Animate*>
{
	inline static const char* value = "Animate";
};
template<>
struct TypeNames<cocos2d::Spawn*>
{
	inline static const char* value = "Spawn";
};
template<>
struct TypeNames<cocos2d::Repeat*>
{
	inline static const char* value = "Repeat";
};
template<>
struct TypeNames<cocos2d::RepeatForever*>
{
	inline static const char* value = "RepeatForever";
};
template<>
struct TypeNames<cocos2d::Sequence*>
{
	inline static const char* value = "Sequence";
};
template<>
struct TypeNames<cocos2d::RotateTo*>
{
	inline static const char* value = "RotateTo";
};
template<>
struct TypeNames<cocos2d::RotateBy*>
{
	inline static const char* value = "RotateBy";
};
template<>
struct TypeNames<cocos2d::MoveBy*>
{
	inline static const char* value = "MoveBy";
};
template<>
struct TypeNames<cocos2d::MoveTo*>
{
	inline static const char* value = "MoveTo";
};
template<>
struct TypeNames<cocos2d::SkewTo*>
{
	inline static const char* value = "SkewTo";
};
template<>
struct TypeNames<cocos2d::SkewBy*>
{
	inline static const char* value = "SkewBy";
};
template<>
struct TypeNames<cocos2d::ResizeTo*>
{
	inline static const char* value = "ResizeTo";
};
template<>
struct TypeNames<cocos2d::ResizeBy*>
{
	inline static const char* value = "ResizeBy";
};
template<>
struct TypeNames<cocos2d::JumpBy*>
{
	inline static const char* value = "JumpBy";
};
template<>
struct TypeNames<cocos2d::JumpTo*>
{
	inline static const char* value = "JumpTo";
};
template<>
struct TypeNames<cocos2d::BezierBy*>
{
	inline static const char* value = "BezierBy";
};
template<>
struct TypeNames<cocos2d::BezierTo*>
{
	inline static const char* value = "BezierTo";
};
template<>
struct TypeNames<cocos2d::ScaleBy*>
{
	inline static const char* value = "ScaleBy";
};
template<>
struct TypeNames<cocos2d::ScaleTo*>
{
	inline static const char* value = "ScaleTo";
};
template<>
struct TypeNames<cocos2d::Blink*>
{
	inline static const char* value = "Blink";
};
template<>
struct TypeNames<cocos2d::FadeTo*>
{
	inline static const char* value = "FadeTo";
};
template<>
struct TypeNames<cocos2d::FadeIn*>
{
	inline static const char* value = "FadeIn";
};
template<>
struct TypeNames<cocos2d::FadeOut*>
{
	inline static const char* value = "FadeOut";
};
template<>
struct TypeNames<cocos2d::TintTo*>
{
	inline static const char* value = "TintTo";
};
template<>
struct TypeNames<cocos2d::TintBy*>
{
	inline static const char* value = "TintBy";
};
template<>
struct TypeNames<cocos2d::DelayTime*>
{
	inline static const char* value = "DelayTime";
};
template<>
struct TypeNames<cocos2d::Show*>
{
	inline static const char* value = "Show";
};
template<>
struct TypeNames<cocos2d::Hide*>
{
	inline static const char* value = "Hide";
};
template<>
struct TypeNames<cocos2d::ToggleVisibility*>
{
	inline static const char* value = "ToggleVisibility";
};
template<>
struct TypeNames<cocos2d::RemoveSelf*>
{
	inline static const char* value = "RemoveSelf";
};
template<>
struct TypeNames<cocos2d::FlipX*>
{
	inline static const char* value = "FlipX";
};
template<>
struct TypeNames<cocos2d::FlipY*>
{
	inline static const char* value = "FlipY";
};
template<>
struct TypeNames<cocos2d::Place*>
{
	inline static const char* value = "Place";
};
template<>
struct TypeNames<cocos2d::CallFunc*>
{
	inline static const char* value = "CallFunc";
};
template<>
struct TypeNames<cocos2d::Touch*>
{
	inline static const char* value = "Touch";
};
template<>
struct TypeNames<cocos2d::Event*>
{
	inline static const char* value = "Event";
};
template<>
struct TypeNames<cocos2d::EventListener*>
{
	inline static const char* value = "EventListener";
};
template<>
struct TypeNames<cocos2d::EventListenerTouchOneByOne*>
{
	inline static const char* value = "EventListenerTouchOneByOne";
};
template<>
struct TypeNames<cocos2d::EventListenerTouchAllAtOnce*>
{
	inline static const char* value = "EventListenerTouchAllAtOnce";
};
template<>
struct TypeNames<cocos2d::EventListenerKeyboard*>
{
	inline static const char* value = "EventListenerKeyboard";
};
template<>
struct TypeNames<cocos2d::extension::EventListenerAssetsManagerEx*>
{
	inline static const char* value = "EventListenerAssetsManagerEx";
};
template<>
struct TypeNames<cocos2d::Node*>
{
	inline static const char* value = "Node";
};
template<>
struct TypeNames<cocos2d::ClippingNode*>
{
	inline static const char* value = "ClippingNode";
};
template<>
struct TypeNames<cocos2d::experimental::TMXTiledMap*>
{
	inline static const char* value = "TMXTiledMap";
};
template<>
struct TypeNames<cocos2d::Scene*>
{
	inline static const char* value = "Scene";
};
template<>
struct TypeNames<cocos2d::Layer*>
{
	inline static const char* value = "Layer";
};
template<>
struct TypeNames<cocos2d::extension::ScrollView*>
{
	inline static const char* value = "ScrollView";
};
template<>
struct TypeNames<cocos2d::extension::TableView*>
{
	inline static const char* value = "ScrollView";
};
template<>
struct TypeNames<cocos2d::Sprite*>
{
	inline static const char* value = "Sprite";
};
template<>
struct TypeNames<cocos2d::SpriteFrame*>
{
	inline static const char* value = "SpriteFrame";
};
template<>
struct TypeNames<cocos2d::Texture2D*>
{
	inline static const char* value = "Texture2D";
};
template<>
struct TypeNames<cocos2d::TextureCache*>
{
	inline static const char* value = "TextureCache";
};
template<>
struct TypeNames<cocos2d::Label*>
{
	inline static const char* value = "Label";
};
template<>
struct TypeNames<GLubyte>
{
	inline static const char* value = "GLubyte";
};
template<>
struct TypeNames<GLenum>
{
	inline static const char* value = "GLenum";
};
template<>
struct TypeNames<cocos2d::TextHAlignment>
{
	inline static const char* value = "TextHAlignment";
};
template<>
struct TypeNames<cocos2d::TextVAlignment>
{
	inline static const char* value = "TextVAlignment";
};
template<>
struct TypeNames<cocos2d::LabelEffect>
{
	inline static const char* value = "LabelEffect";
};
template<>
struct TypeNames<cocos2d::Label::Overflow>
{
	inline static const char* value = "Overflow";
};
template<>
struct TypeNames<cocos2d::Label::LabelType>
{
	inline static const char* value = "LabelType";
};
template<>
struct TypeNames<cocos2d::Director::Projection>
{
	inline static const char* value = "Projection";
};
template<>
struct TypeNames<cocos2d::EventListener::Type>
{
	inline static const char* value = "EventListenerType";
};
template<>
struct TypeNames<cocos2d::ApplicationProtocol::Platform>
{
	inline static const char* value = "Platform";
};
template<>
struct TypeNames<ResolutionPolicy>
{
	inline static const char* value = "ResolutionPolicy";
};
template<>
struct TypeNames<cocos2d::experimental::AudioEngine::AudioState>
{
	inline static const char* value = "AudioState";
};
template<>
struct TypeNames<cocos2d::Event::Type>
{
	inline static const char* value = "EventType";
};
template<>
struct TypeNames<cocos2d::EventKeyboard::KeyCode>
{
	inline static const char* value = "KeyCode";
};
template<>
struct TypeNames<cocos2d::extension::AssetsManagerEx::State>
{
	inline static const char* value = "State";
};
template<>
struct TypeNames<cocos2d::extension::Manifest*>
{
	inline static const char* value = "Manifest";
};
template<>
struct TypeNames<cocos2d::extension::Manifest const*>
{
	inline static const char* value = "Manifest";
};
template<>
struct TypeNames<cocos2d::extension::EventAssetsManagerEx::EventCode>
{
	inline static const char* value = "EventCode";
};
template<>
struct TypeNames<cocos2d::extension::EventAssetsManagerEx*>
{
	inline static const char* value = "EventAssetsManagerEx";
};
template<>
struct TypeNames<cocos2d::Vector<cocos2d::SpriteFrame*>*>
{
	inline static const char* value = "SpriteFrames";
};

// todo: more



template<>
struct TypeNames<cocos2d::ui::Widget*>
{
	inline static const char* value = "Widget";
};
template<>
struct TypeNames<cocos2d::ui::EditBox*>
{
	inline static const char* value = "EditBox";
};
template<>
struct TypeNames<cocos2d::ui::Button*>
{
	inline static const char* value = "Button";
};
template<>
struct TypeNames<cocos2d::ui::ImageView*>
{
	inline static const char* value = "ImageView";
};
template<>
struct TypeNames<cocos2d::ui::Layout*>
{
	inline static const char* value = "Layout";
};
template<>
struct TypeNames<cocos2d::ui::LoadingBar*>
{
	inline static const char* value = "LoadingBar";
};
template<>
struct TypeNames<cocos2d::ui::Scale9Sprite*>
{
	inline static const char* value = "Scale9Sprite";
};
template<>
struct TypeNames<cocos2d::ui::Widget::FocusDirection>
{
	inline static const char* value = "FocusDirection";
};
template<>
struct TypeNames<cocos2d::ui::Widget::PositionType>
{
	inline static const char* value = "PositionType";
};
template<>
struct TypeNames<cocos2d::ui::Widget::SizeType>
{
	inline static const char* value = "SizeType";
};
template<>
struct TypeNames<cocos2d::ui::Widget::TouchEventType>
{
	inline static const char* value = "TouchEventType";
};
template<>
struct TypeNames<cocos2d::ui::Widget::TextureResType>
{
	inline static const char* value = "TextureResType";
};
template<>
struct TypeNames<cocos2d::ui::Widget::BrightStyle>
{
	inline static const char* value = "BrightStyle";
};
template<>
struct TypeNames<cocos2d::ui::EditBox::InputMode>
{
	inline static const char* value = "InputMode";
};
template<>
struct TypeNames<cocos2d::ui::EditBox::InputFlag>
{
	inline static const char* value = "InputFlag";
};
template<>
struct TypeNames<cocos2d::ui::EditBox::KeyboardReturnType>
{
	inline static const char* value = "KeyboardReturnType";
};
template<>
struct TypeNames<cocos2d::ui::LoadingBar::Direction>
{
	inline static const char* value = "LoadingBarDirection";
};
template<>
struct TypeNames<cocos2d::ui::Layout::ClippingType>
{
	inline static const char* value = "ClippingType";
};
template<>
struct TypeNames<cocos2d::ui::Layout::Type>
{
	inline static const char* value = "LayoutType";
};
template<>
struct TypeNames<cocos2d::ui::Layout::BackGroundColorType>
{
	inline static const char* value = "BackGroundColorType";
};

// todo: more

template<>
struct TypeNames<spine::SkeletonAnimation*>
{
	inline static const char* value = "SkeletonAnimation";
};
template<>
struct TypeNames<spine::SkeletonRenderer*>
{
	inline static const char* value = "SkeletonRenderer";
};
//template<>
//struct TypeNames<spVertexEffect*>
//{
//	inline static const char* value = "spVertexEffect";
//};
template<>
struct TypeNames<spSkeleton*>
{
	inline static const char* value = "spSkeleton";
};
template<>
struct TypeNames<spBone*>
{
	inline static const char* value = "spBone";
};
template<>
struct TypeNames<spSlot*>
{
	inline static const char* value = "spSlot";
};
template<>
struct TypeNames<spAttachment*>
{
	inline static const char* value = "spAttachment";
};
template<>
struct TypeNames<spSkeletonData*>
{
	inline static const char* value = "spSkeletonData";
};
template<>
struct TypeNames<spAnimationStateData*>
{
	inline static const char* value = "spAnimationStateData";
};
template<>
struct TypeNames<spTrackEntry*>
{
	inline static const char* value = "spTrackEntry";
};
template<>
struct TypeNames<spEvent*>
{
	inline static const char* value = "spEvent";
};
template<>
struct TypeNames<spAtlas*>
{
	inline static const char* value = "spAtlas";
};
template<>
struct TypeNames<spAnimation*>
{
	inline static const char* value = "spAnimation";
};
template<>
struct TypeNames<spAnimationState*>
{
	inline static const char* value = "spAnimationState";
};

// todo: more


template<>
struct TypeNames<xx::BBuffer*>
{
	inline static const char* value = "BBuffer";
};
template<>
struct TypeNames<Lua_BBuffer*>
{
	inline static const char* value = "BBuffer";
};
template<>
struct TypeNames<xx::UvItem_s*>
{
	inline static const char* value = "UvItem";
};
template<>
struct TypeNames<xx::UvResolver_s*>
{
	inline static const char* value = "UvResolver";
};
template<>
struct TypeNames<xx::UvDialer_s*>
{
	inline static const char* value = "UvDialer";
};
template<>
struct TypeNames<xx::UvSerialBBufferPeer_s*>
{
	inline static const char* value = "UvPeer";
};

// todo: more





template<>
struct TypeNames<CatchFish_s*>
{
	inline static const char* value = "CatchFish";
};
