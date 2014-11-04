//------.cpp
#include "GameOverScene.h"
#include "UIText.h"
#include "LHMacros.h"
#include "LHShareButton.h"
#include "LHLeaderBoard.h"
#include "PlayScene.h"
#include "HelloWorldScene.h"
#include "LocalizedString.h"
#include "ThirdPartyHelper.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
Scene* GameOverScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

GameOverScene::~GameOverScene(){
    ThirdPartyHelper::setAd(SET_AD_HID);
}

static float godelay = 0;

static void show(Node *node){
    node->setScale(0.01);
    DelayTime *d = DelayTime::create(godelay);
    ScaleTo *b = ScaleTo::create(0.1, 1.2);
    ScaleTo *s = ScaleTo::create(0.03, 1);
    Sequence *ac = Sequence::create(d,b,s, NULL);
    node->runAction(ac);
    godelay += 0.05f;
}

bool GameOverScene::initDict(cocos2d::CCDictionary *dic)
{
    ThirdPartyHelper::setAd(SET_AD_SHOW);
    godelay = 0;
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    int step = ((CCInteger*)dic->objectForKey("step"))->getValue();
    int level = ((CCInteger*)dic->objectForKey("level"))->getValue();
    
    UserDefault::getInstance()->setBoolForKey(StringUtils::format("level%d_lock",level+1).c_str(), false);
    UserDefault::getInstance()->flush();
    
    auto gameover = ui::Text::create(StringUtils::format("Level %d Pass!",level+1), Common_Font, 70);
    gameover->setPosition(Vec2(vs.width/2, vs.height/3*2));
    gameover->setColor(Color3B::BLACK);
    this->addChild(gameover);
    show(gameover);
    
    int best = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level%d_step",level).c_str(), INT16_MAX);
    if (step<best) {
        best = step;
        UserDefault::getInstance()->setIntegerForKey(StringUtils::format("level%d_step",level).c_str(), best);
    }
    
    auto scorelabel = ui::Text::create(StringUtils::format("Step:%d",step), Common_Font, 50);
    scorelabel->setPosition(Vec2(vs.width/2, vs.height/2+scorelabel->getContentSize().height/2));
    scorelabel->setColor(Color3B::BLACK);
    this->addChild(scorelabel);
    show(scorelabel);
    
    auto bestlabel = ui::Text::create(StringUtils::format("Best:%d",best), Common_Font, 50);
    bestlabel->setPosition(Vec2(vs.width/2, vs.height/2-bestlabel->getContentSize().height/2));
    bestlabel->setColor(Color3B::BLACK);
    this->addChild(bestlabel);
    show(bestlabel);
    
    float by = vs.height/6;
    float fs = 40;
    
    auto replay = ui::Button::create("rb.png");
    replay->setTitleText("Next");
    replay->setTitleFontSize(fs);
    replay->setPosition(Vec2(vs.width/2, vs.height/3));
    replay->addTouchEventListener([level](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Dictionary *dict = Dictionary::create();
            dict->setObject(CCInteger::create(level+1),"level");
            Director::getInstance()->replaceScene(PlayScene::createScene(dict));
        }
    });
    this->addChild(replay);
    show(replay);
    
    auto back = ui::Button::create("rr.png");
    back->setPosition(Vec2(vs.width/3, by));
    back->setTitleFontSize(fs);
    back->setTitleText("Back");
    back->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(HelloWorld::createScene());
        }
    });
    this->addChild(back);
    show(back);
    
    auto share = LHShareButton::defaultButton("rr.png","Lucky Puzzle!");
    share->setTitleFontSize(fs);
    share->setTitleText("Share");
    share->setPosition(Vec2(vs.width/3*2, by));
    this->addChild(share);
    show(share);
    
	return true;
}