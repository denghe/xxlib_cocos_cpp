/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "AppMacros.h"

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"

USING_NS_CC;
using namespace spine;

//static Cocos2dTextureLoader textureLoader;
//#define SPINE_NODE_SCALE_FACTOR 0.4
//#define SCALE_SKELETON_NODE(node) do { if(node) node->setScale(SPINE_NODE_SCALE_FACTOR); } while(false)

Scene* HelloWorld::scene()
{
     return HelloWorld::create();
}


static float pathwayR = 0.0f;
static cocos2d::Label *label = nullptr;
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() ) {
        return false;
    }

    auto bg = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE);
    bg->setContentSize(_contentSize);
    addChild(bg);

    initFish();
//    initSpine();
//    initDragon();

    label = cocos2d::Label::create();
    label->setPosition(_contentSize.width / 2, 50);
    label->setString("cnt:0");
    label->setSystemFontSize(40);
    label->setTextColor(cocos2d::Color4B::GREEN);
    addChild(label, 100);

    pathwayR = sqrt(_contentSize.width * _contentSize.width + _contentSize.height * _contentSize.height) / 2 + 75;

    scheduleUpdate();
    return true;
}

//static const char* ATLAS_PATH = "spine/spineboy.atlas";
//static const char* SKE_PATH = "spine/spineboy-pro.json";
//static const char* ATLAS_PATH1 = "spine/spine/3.8/bin/skeleton.atlas";
//static const char* SKE_PATH1 = "spine/spine/3.8/bin/skeleton.skel";
////static const char* ATLAS_PATH1 = "spine/spine/3.6.5/bin/skeleton.atlas";
////static const char* SKE_PATH1 = "spine/spine/3.6.5/bin/skeleton.skel";

//void HelloWorld::initSpine() {
//    auto atlas = new (__FILE__, __LINE__) Atlas(ATLAS_PATH, &textureLoader, true);
//    CCASSERT(atlas, "Error reading atlas file.");
//
//    // This attachment loader configures attachments with data needed for cocos2d-x rendering.
//// Do not dispose the attachment loader until the skeleton data is disposed!
//    auto attachmentLoader = new (__FILE__, __LINE__) Cocos2dAtlasAttachmentLoader(atlas);
//
//    // Load the skeleton data.
//    SkeletonJson* json = new (__FILE__, __LINE__) SkeletonJson(attachmentLoader);
//    json->setScale(0.6f); // Resizes skeleton data to 60% of the size it was in Spine.
//    SkeletonData *skeletonData = json->readSkeletonDataFile(SKE_PATH);
//    CCASSERT(skeletonData,
//             json->getError().isEmpty() ? json->getError().buffer() : "Error reading skeleton data file.");
//
//    delete atlas;
//    delete attachmentLoader;
//    delete json;
//
//    // Setup mix times.
//    auto stateData = new (__FILE__, __LINE__) AnimationStateData(skeletonData);
////    stateData->setMix("walk", "jump", 0.2f);
////    stateData->setMix("jump", "run", 0.2f);
//
//    int xMin = _contentSize.width * 0.10f, xMax = _contentSize.width * 0.90f;
//    int yMin = 0, yMax = _contentSize.height * 0.7f;
//    for (int i = 0; i < 10; i++) {
//        // Each skeleton node shares the same atlas, skeleton data, and mix times.
//        SkeletonAnimation* skeletonNode = SkeletonAnimation::createWithData(skeletonData, false);
//        auto track = skeletonNode->setAnimation(i * 30, "walk", true);
//        track->setTrackTime(i);
//        track->setTimeScale(0.1f);
////        auto tracks = skeletonNode->getState()->getTracks();
////        for (int i = 0; i < tracks.size(); ++i) {
////            tracks[i]->setAnimationStart(i);
////        }
////        skeletonNode->addAnimation(0, "jump", true, RandomHelper::random_int(0, 300) / 100.0f);
////        skeletonNode->addAnimation(0, "run", true);
//
//        // alternative setting two color tint for groups of 10 skeletons
//        // should end up with #skeletons / 10 batches
//        // if (j++ < 10)
////			skeletonNode->setTwoColorTint(true);
////		if (j == 20) j = 0;
//        // skeletonNode->setTwoColorTint(true);
//
//        skeletonNode->setPosition(Vec2(
//                RandomHelper::random_int(xMin, xMax),
//                RandomHelper::random_int(yMin, yMax)
//        ));
//        skeletonNode->setScale(0.4);
//        addChild(skeletonNode);
//    }
////    SCALE_SKELETON_NODE(skeletonData);
//
//}

void HelloWorld::initFish() {
    auto pathway = xx::Pathway::Make(true, {
            {800.0f,  200.0f, 0.4f, 500},
            {1500.0f, 500.0f, 0.4f, 500},
            {800.0f,  800.0f, 0.4f, 500},
            {100.0f,  500.0f, 0.4f, 500},
    });

    {
        auto anim = CreateAnimExt("skeleton.atlas.ext", this);
        if (anim) {
            anim->pathways.emplace_back(pathway);
            anim->timeScale = 2.5;
            anim->speedScale = -1;
            anim->anim->scaleX = 1;
            anim->anim->scaleY = 1;
            anim->SetPathway(0);
            anims.push_back(anim);
        }
    }
    {
        auto anim = CreateAnimExt("1.frames.ext", this);
        if (anim) {
            anim->pathways.emplace_back(pathway);
            anim->timeScale = 1;
            anim->speedScale = 1;
            anim->anim->scaleX = 1;
            anim->anim->scaleY = 1;
            anim->SetPathway(0);
            anims.push_back(anim);
        }
    }
}

//static TrackEntry* track = NULL;

//void HelloWorld::initDragon()
//{
//    auto node = SkeletonAnimation::createWithBinaryFile(SKE_PATH1, ATLAS_PATH1, 1);
//    track = node->setAnimation(0, "animation", false);
//    track->setTrackTime(0);
//    track->setTimeScale(0);
//    node->setPosition(_contentSize.width / 2, _contentSize.height / 2);
//    node->setScale(1.0);
//    addChild(node);
//}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();
}

static float time1 = 0;
static int frame = 0;
void HelloWorld::update(float delta)
{
    for (int i = anims.size() - 1; i >= 0; --i) {
        auto& anim = anims[i];
        if (anim->Update(delta)) {
//            anim.reset();
            anims[i] = anims[anims.size() - 1];
            anims.pop_back();
        } else {
            anim->Draw();
        }
    } 

    char tmp[64];
    snprintf(tmp, sizeof(tmp), "cnt:%d", anims.size());
    label->setString(tmp);

    if (frame++ % 1 == 0 && anims.size() < 1000) {
        //for (int i = 0; i < 8; ++i) 
        {
            xx::Point center(_contentSize.width / 2, _contentSize.height / 2);
            xx::Point pos(pathwayR, 0);
            int ran = RandomHelper::random_int(135, 225);
            if (ran & 1) {
                ran += 180;
            }
            float srcAngle = ran / 180.0f * M_PI;
            auto srcPos = xx::Rotate(pos, srcAngle) + center;

            float destAngle = srcAngle + M_PI + RandomHelper::random_int(-30, 30) / 180.0f * M_PI;
            auto destPos = xx::Rotate(pos, destAngle) + center;
            auto pathway = xx::PathwayMaker(srcPos).To(destPos).End();
            int type = RandomHelper::random_int(0, 20) == 1 ? 0 : 1;
            auto path = type ? "1.frames.ext" : "skeleton.atlas.ext";
            auto anim = CreateAnimExt(path, this);
            if (anim) {
                anim->pathways.emplace_back(pathway);
                anim->timeScale = 1;
                anim->speedScale = 1;
                if (type) {
                    anim->anim->scaleX = 1;
                    anim->anim->scaleY = 1;
                } else {
                    anim->anim->scaleX = 0.25;
                    anim->anim->scaleY = 0.25;
                }
                anim->SetPathway(0);
                anims.push_back(anim);
            }
        }
    }

//    time1 += delta;
//    float animPeriod = track->getAnimationEnd();
//    if (animPeriod > 0)
//        while (time1 > animPeriod) time1 -= animPeriod;
//    if (track) track->setTrackTime(time1);
}
