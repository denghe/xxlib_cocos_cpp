#include "Anim.h"
#include "FileExts_class_lite.ajson.h"
#include "Tools.h"

// todo: 以后再补合法性检测, 异常处理

int Anim_Frames::Load() {
	// 加载文件数据
	if (int r = LoadJson(file, fileName)) return r;
	// 缓存相关 plists
	auto&& sfc = cocos2d::SpriteFrameCache::getInstance();
	for (auto&& fn : file.plists) {
		sfc->addSpriteFramesWithFile(fn);
	}
	// 设置当前动作
//	action = &file.actions[0];
    SetAction(file.actions[0].name);
	return 0;
}

void Anim_Frames::SetAction(const std::string& name) {
	if (name.size()) {
	    for (auto& a: file.actions) {
	        if (a.name == name) {
	            action = &a;
	            break;
	        }
	    }
	}
	totalElapsedSeconds = 0;
	frameIndex = 0;
}
int Anim_Frames::Update(float const& elapsedSeconds) {
	totalElapsedSeconds += elapsedSeconds;
	if (totalElapsedSeconds >= action->totalSeconds) {
		if (loop) {
			totalElapsedSeconds = fmodf(totalElapsedSeconds, action->totalSeconds);
			frameIndex = 0;
		}
		else {
			totalElapsedSeconds = action->totalSeconds;
			frameIndex = action->frames.size() - 1;
			return 1;
		}
	}
	while (action->frames.size() > frameIndex + 1 && action->frames[frameIndex + 1].time <= totalElapsedSeconds) {
		++frameIndex;
	}
	return 0;
}
void Anim_Frames::Draw() {
	if (!sprite) {
		sprite = cocos2d::Sprite::create();
		if (shadowScale != 0.f) {
            spriteShadow = cocos2d::Sprite::create();
            spriteShadow->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
            spriteShadow->setPosition(shadowX, shadowY);
            spriteShadow->setColor(cocos2d::Color3B::BLACK);
            spriteShadow->setOpacity(64);
            sprite->addChild(spriteShadow, -1);
            container->addChild(sprite);
        }
	}
	sprite->setSpriteFrame(action->frames[frameIndex].picName);
	if (spriteShadow) {
        spriteShadow->setSpriteFrame(action->frames[frameIndex].picName);
    }
	sprite->setPosition(pos.x, pos.y);
	sprite->setRotation(-angle * (180.0f / float(M_PI)));
	sprite->setScale(scaleX, scaleY);
	//sprite->setZOrder(??); 
	sprite->setVisible(visible);
}
Anim_Frames::~Anim_Frames() {
	if (sprite) {
		sprite->removeFromParentAndCleanup(true);
	}
}

Anim_Atlas::~Anim_Atlas() {
    if (skeletonAnimation) {
        skeletonAnimation->removeFromParentAndCleanup(true);
    }
}
int Anim_Atlas::Load() {
    auto skeletonFilename = fileName.substr(0, fileName.rfind('.')) + ".skel";
    skeletonAnimation = spine::SkeletonAnimation::createWithBinaryFile(skeletonFilename, fileName, 1);
    skeletonAnimation->retain();
    CCASSERT(skeletonAnimation, "");
    skeletonAnimationShadow = spine::SkeletonAnimation::createWithBinaryFile(skeletonFilename, fileName, 1);
    CCASSERT(skeletonAnimationShadow, "");
    skeletonAnimationShadow->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    skeletonAnimationShadow->setPosition(-80, -80);
//    skeletonAnimationShadow->setOpacity(128);
//    skeletonAnimationShadow->setColor(cocos2d::Color3B::BLACK);
//    auto node = cocos2d::Node::create();
//    node->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    skeletonAnimationShadow->setColor(cocos2d::Color3B::BLACK);
    skeletonAnimationShadow->setCascadeColorEnabled(true);
    skeletonAnimationShadow->setOpacityModifyRGB(false);
    skeletonAnimationShadow->setOpacity(64);
//    node->setColor(cocos2d::Color3B::YELLOW);
//    skeletonAnimation->addChild(node, -1);
//    node->addChild(skeletonAnimationShadow);
    skeletonAnimation->addChild(skeletonAnimationShadow, -1);
//    container->addChild(skeletonAnimation);
//    SetAction("");
    // TODO set default anim
    return 0;
}
void Anim_Atlas::SetAction(const std::string& name) {
    CCASSERT(skeletonAnimation, "");
    CCASSERT(skeletonAnimationShadow, "");
    if (!name.empty()) {
        skeletonAnimation->setAnimation(0, name, true);
        skeletonAnimationShadow->setAnimation(0, name, true);
    }
    totalElapsedSeconds = 0;
}
int Anim_Atlas::Update(float const& elapsedSeconds) { return 0; }
void Anim_Atlas::Draw() {
    auto node = skeletonAnimation;
    if (!node->getParent()) {
        container->addChild(node);
        node->release();
    }
    node->setPosition(pos.x, pos.y);
    node->setRotation(-angle * (180.0f / float(M_PI)));
    node->setScale(scaleX, scaleY);
    //sprite->setZOrder(??);
    node->setVisible(visible);
}


Anim_C3b::~Anim_C3b() {}
int Anim_C3b::Load() { return 0; }
void Anim_C3b::SetAction(const std::string& name) {}
int Anim_C3b::Update(float const& elapsedSeconds) { return 0; }
void Anim_C3b::Draw() {}


// 根据文件名加载指定动画. 根据扩展名路由创建 Anim_Frames / Anim_Atlas / Anim_C3b
std::shared_ptr<Anim> CreateAnim(std::string const& fn, cocos2d::Node* const& container) {
	std::shared_ptr<Anim> rtv;
	const auto&& exts = xx::GetFileNameExts(fn);
	//if (exts.first != ".ext") return rtv;
	if (exts.first == ".frames") {
		rtv = xx::Make<Anim_Frames>();
	}
	else if (exts.first == ".atlas") {
		rtv = xx::Make<Anim_Atlas>();
	}
	else if (exts.first == ".c3b") {
		rtv = xx::Make<Anim_C3b>();
	}
	else return rtv;
	rtv->fileName = fn;
	rtv->container = container;
	if (rtv->Load()) return nullptr;
	return rtv;
}



int AnimExt_Anim::Load() {
	// 加载资源. 从文件名截取处资源名
	const auto&& exts = xx::GetFileNameExts(fileName);
	if (exts.first != ".ext") return -1;
	anim = CreateAnim(fileName.substr(0, fileName.size() - exts.first.size()), container);
	if (!anim) return -2;
	// 加载 .ext
	if (int r = LoadJson(file, fileName)) return r;
    anim->shadowScale = file.shadowScale;
    anim->shadowX = file.shadowX;
    anim->shadowY = file.shadowY;
	action = &file.actions[0];
	anim->SetAction(action->name);
	speed = action->ss[0].speed;
	return 0;
}
void AnimExt_Anim::SetPathway(size_t const& idx) {
	pathway = &*pathways[idx];
	if (speedScale >= 0) {
		pathway->Begin(pathwayI, pathwayD, anim->pos, anim->angle);
	}
	else {
		pathway->End(pathwayI, pathwayD, anim->pos, anim->angle);
	}
}
// 可选算法：帧逻辑，基于时间池，按一个固定时长从池减去，每减一次算一次. 有助于降低编写复杂度
int AnimExt_Anim::Update(float const& elapsedSeconds_) {
	if (!pathway || timeScale == 0 || speedScale == 0) return 0;
	auto elapsedSeconds = elapsedSeconds_ * timeScale;
	auto sumSeconds = 0;

	// 在 pathway 上移动 d 距离
	const auto&& Move = [&](float const& d) {
		if (speedScale > 0) {
			return pathway->Forward(d * speedScale, pathwayI, pathwayD, anim->pos, anim->angle);
		}
		else {
			return pathway->Backward(d * -speedScale, pathwayI, pathwayD, anim->pos, anim->angle);
		}
	};

	auto&& ss = action->ss;
	// 计算不超出当前动作总时长的移动距离
	const auto&& Calc = [&](float es) {
	LabBegin:
		auto next = ss.data() + ssCursor + 1;
		if (ss.size() > ssCursor + 1 && next->time <= totalElapsedSeconds + es) {
			++ssCursor;
			auto es2 = next->time - totalElapsedSeconds;
			if (Move(speed * es2)) {
			    return true;
			}
			es -= es2;
			sumSeconds += es2;
			totalElapsedSeconds = next->time;
			if (next->speed >= 0) {
				speed = next->speed;
			}
			else {
				Event(-(int)next->speed, elapsedSeconds_ - sumSeconds);
			}
			goto LabBegin;
		}
		else {
			totalElapsedSeconds += es;
			if (Move(speed * es)) {
			    return true;
			}
			return false;
		}
	};
LabBegin:
	// 计算距离
	// 判断传入时长是否会超出当前 timeLine 的范围. 如果有超出则切割计算
	auto left = action->totalSeconds - totalElapsedSeconds;
	if (elapsedSeconds > left) {
		elapsedSeconds -= left;
		if (Calc(left)) {
		    return -1;
		}
		anim->SetAction("");
		totalElapsedSeconds = 0;
		lpsCursor = 0;
		cdsCursor = 0;
		ssCursor = 0;
		speed = ss[0].speed;
		goto LabBegin;
	}
	else {
        if (Calc(elapsedSeconds)) return -1;
		anim->Update(elapsedSeconds);
		// 同步 锁定，碰撞 游标( 此时 totalElapsedSeconds 已经 + 了 elapsedSeconds )
		while (action->lps.size() > lpsCursor + 1 && action->lps[lpsCursor + 1].time <= totalElapsedSeconds) {
			++lpsCursor;
		}
		while (action->cds.size() > cdsCursor + 1 && action->cds[cdsCursor + 1].time <= totalElapsedSeconds) {
			++cdsCursor;
		}
	}
	return 0;
}
void AnimExt_Anim::Event(int const& eventId, float const& elapsedSeconds) {
	//xx::CoutN("eventId = ",eventId, " elapsedSeconds = ", elapsedSeconds, " pos = " ,anim->pos);
//	cocos2d::AudioEngine::play2d("effect2.ogg");
//    cocos2d::AudioEngine::play2d("music/accelerate.mp3");
}
bool AnimExt_Anim::IsIntersect(float const& x, float const& y, float const& r) const {
	if (action->cds.empty()) return false;
	auto&& Intersect = [](FileExts::CDCircle const& c, float const& x, float const& y, float const& r) {
		return (c.x - x) * (c.x - x) + (c.y - y) * (c.y - y) <= (c.r + r) * (c.r + r);
	};
	auto&& cd = action->cds[cdsCursor];
	if (!Intersect(cd.maxCDCircle, x, y, r)) return false;
	for (auto&& c : cd.cdCircles) {
		if (Intersect(c, x, y, r)) return true;
	}
	return false;
}
bool AnimExt_Anim::Lockable() const {
	if (action->lps.empty()) return false;
	// todo: 判断所有点都在可锁定区域/显示区域内
	return true;
}
xx::Point AnimExt_Anim::GetLockPoint() const {
	xx::Point rtv;
	if (action->lps.empty()) return rtv;
	// todo: 需结合坐标以及屏幕裁剪范围来算
	rtv = action->lps[lpsCursor].mainLockPoint;
	return rtv;
}
bool AnimExt_Anim::Attackable() const { return true; }
void AnimExt_Anim::Lock() {}
void AnimExt_Anim::Unlock() {}
void AnimExt_Anim::Attack() {}
void AnimExt_Anim::Death() {}
void AnimExt_Anim::Draw() {
	anim->Draw();
#ifdef DEBUG_DRAW_PATHWAY
    if (!debug_path) {
        debug_path = cocos2d::DrawNode::create();
        for (auto pathway: this->pathways) {
            debug_path = cocos2d::DrawNode::create();
            auto len = pathway->points.size();
            if (!len) continue;
            std::vector<cocos2d::Vec2> points(len);
            for (int i = 0; i < len; ++i) {
                auto& pos = pathway->points[i].pos;
                points[i] = cocos2d::Vec2(pos.x, pos.y);
            }
            if (len == 2) {
                debug_path->drawLine(points[0], points[1], cocos2d::Color4F::RED);
            } else {
                debug_path->drawPolygon(points.data(), len, cocos2d::Color4F(), 1, cocos2d::Color4F::RED);
            }
            container->addChild(debug_path);
        }
    }

#ifdef DEBUG_DRAW_ANIM_INFO
    if (!debug_info) {
        debug_info = cocos2d::DrawNode::create();
        container->addChild(debug_info);
    }
    auto pos = anim->pos;
    debug_info->setPosition(pos.x, pos.y);
    debug_info->setRotation(-anim->angle * (180.0f / float(M_PI)));
    debug_info->setScale(anim->scaleX, anim->scaleY);
    debug_info->clear();

    auto drawCircle = [&] (auto& cd) {
        debug_info->drawCircle(cocos2d::Vec2(cd.x, cd.y), cd.r,
                               0, 360, false, cocos2d::Color4F::WHITE);
    };

    auto&& cd = action->cds[cdsCursor];
    drawCircle(cd.maxCDCircle);
    for (auto&& cd1: cd.cdCircles) drawCircle(cd1);
//    debug_info->drawCircle(cocos2d::Vec2(cd.maxCDCircle.x, cd.maxCDCircle.y), cd.maxCDCircle.r,
//                           0, 360, false, cocos2d::Color4F::WHITE);

//    for (auto&& cd1: cd.cdCircles) {
//        debug_info->drawCircle(cocos2d::Vec2(cd1.x, cd1.y), cd1.r,
//                               0, 360, false, cocos2d::Color4F::WHITE);
//    }
//    debug_info->drawCircle()
#endif

#endif //DEBUG_DRAW_PATHWAY
}
AnimExt_Anim::~AnimExt_Anim() {
#ifdef DEBUG_DRAW_PATHWAY
    if (debug_path) {
        debug_path->removeFromParent();
    }
#endif
#ifdef DEBUG_DRAW_ANIM_INFO
    if (debug_info) {
        debug_info->removeFromParent();
    }
#endif
}




std::shared_ptr<AnimExt> CreateAnimExt(std::string const& fn, cocos2d::Node* const& container) {
	std::shared_ptr<AnimExt> rtv;
	const auto&& exts = xx::GetFileNameExts(fn);
	if (exts.first != ".ext") return rtv;
	rtv = xx::Make<AnimExt_Anim>();
	rtv->fileName = fn;
	rtv->container = container;
	if (rtv->Load()) return nullptr;
	return rtv;
}



//// Lua 类动画基类, 虚函数调用到和 lua 函数绑定的 std::function
//struct AnimLua : AnimBase {
//    lua_State *L;
//    std::string scriptName;
//    // AnimXxxx 容器? lua 可能创建 n 个 并控制它们. 通常创建 1 个
//
//    // 加载脚本并映射函数
//    AnimLua(lua_State *const &L, std::string scriptName) : L(L), scriptName(std::move(scriptName)) {
//        // todo: load script, bind func
//    };
//
//    std::function<void(std::string const &actionName)> onSetAction;
//
//    void SetAction(std::string const &actionName) override {
//        onSetAction(actionName);
//    }
//
//    // more
//};


//
//
//int main() {
//    xx::Lua::State L;
//    if (auto r = XL::Try(L, [&] {
//        int x = 0;
//        XL::SetGlobal(L, "beep", [&x] { return ++x; });
//
//        auto t = std::chrono::steady_clock::now();
//        auto &&Show = [&] {
//            std::cout << "x = " << x << " ms = " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count() << std::endl;
//            t = std::chrono::steady_clock::now();
//        };
//
//        luaL_dostring(L, R"(
//for i = 1, 1000000 do
//    beep()
//end
//)");
//        Show();
//
//        std::function<int()> f;
//        XL::GetGlobal(L, "beep", f);
//        for (int i = 0; i < 1000000; ++i) {
//            f();
//        }
//        Show();
//
//    })) {
//        std::cout << "error! " << r.m << std::endl;
//    }
//}
//
