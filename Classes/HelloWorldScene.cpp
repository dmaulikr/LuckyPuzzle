#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "ThirdPartyHelper.h"
#include "UIButton.h"
#include "DeveloperInfoScence.h"
#include "LHLeaderBoard.h"
#include "UILayout.h"
#include "LHShareButton.h"
#include "UIText.h"
#include "LocalizedString.h"
#include "LHMacros.h"
#include "LevelScene.h"

USING_NS_CC;
USING_NS_UM_SOCIAL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->initWithColor(Color4B::WHITE);
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    ThirdPartyHelper::setAd(SET_AD_SHOW);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto appname = cocos2d::ui::Text::create(LHLocalizedCString("appname"), Common_Font, 40);
    appname->setColor(Color3B::BLACK);
    appname->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3*2+ origin.y));
    this->addChild(appname);
    
    auto play = ui::Button::create("rb.png");
    play->setTitleText("Play");
    play->setTitleFontSize(35);
    play->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    play->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            ThirdPartyHelper::setAd(SET_AD_HID);
            auto tran = TransitionSlideInR::create(0.3f, LevelScene::createScene(nullptr));
            Director::getInstance()->replaceScene(tran);
        }
    });
    this->addChild(play);
    
    auto ud = UserDefault::getInstance();
    if (ud->getIntegerForKey("level9_step", 0)!=0) {
        int total = 0;
        for (int i =0 ; i<LVCOUNT; i++) {
            int step = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level%d_step",i).c_str(),0);
            total+=step;
        }
        
        
        if (total < ud->getIntegerForKey("total_step",INT16_MAX)) {
            ud->setIntegerForKey("total_step", total);
            ThirdPartyHelper::uploadScore(total);
        }
        
        std::string lb = "rr.png";
        auto leader = LHLeaderBoard::defaultButton(lb);
        leader->setTitleText("Rank");
        leader->setTitleFontSize(40);
        leader->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y));
        this->addChild(leader);
    }else{
        auto text = ui::Text::create(LHLocalizedCString("seerank"), Common_Font, 30);
        text->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y));
        text->setColor(Color3B::GRAY);
        this->addChild(text);
    }
    
    auto debt = DeveloperInfo::DevInfoButton("devinfo.png");
    debt->setPosition(Vec2(appname->getPositionX() + appname->getContentSize().width/2 + 40, appname->getPositionY() + appname->getContentSize().height/2 + 20));
    this->addChild(debt);
    
    return true;
}
