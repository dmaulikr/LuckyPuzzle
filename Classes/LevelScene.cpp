//------.cpp
#include "LevelScene.h"
#include "LHMacros.h"
#include "UIListView.h"
#include "UIButton.h"
#include "PlayScene.h"
#include "UIText.h"

USING_NS_CC;
Scene* LevelScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = LevelScene::create();
	layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

LevelScene::~LevelScene(){
}
bool LevelScene::initDict(cocos2d::CCDictionary *dic)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    auto list = cocos2d::ui::ListView::create();
    //    list->setBackGroundColor(Color3B::YELLOW);
    //    list->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    list->setSize(Size(visibleSize.width,visibleSize.height));
    Size ls = list->getSize();
    list->setPosition(Vec2(visibleSize.width/2 - ls.width/2, visibleSize.height/2 - ls.height/2 + vo.y));
    list->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    list->setItemsMargin(5);
    this->addChild(list);
    
    for (int i = 0 ; i<LVCOUNT; i++) {
        auto ui = getUI(i);
        list->pushBackCustomItem(ui);
    }
    
	return true;
}

ui::Layout* LevelScene::getUI(int levelindex){
    int i = levelindex;
    
    Size vs = Director::getInstance()->getVisibleSize();
    
    auto lo = ui::Layout::create();
    lo->setSize(Size( vs.width , vs.height / 6));
    Size los = lo->getSize();
    
    const char *img = NULL;
    if (levelindex%2==0) {
        img = "rr.png";
    }else{
        img = "rb.png";
    }
    auto lbt = ui::Button::create(img);
    lbt->ignoreContentAdaptWithSize(false);
    lbt->setSize(los);
    lbt->setScale9Enabled(true);
    lbt->addTouchEventListener([i](Ref*,ui::Widget::TouchEventType type){
        if (ui::Widget::TouchEventType::ENDED == type) {
            auto dict = CCDictionary::create();
            dict->setObject(CCInteger::create(i), "level");
            auto tran = TransitionSlideInR::create(0.3f, PlayScene::createScene(dict));
            Director::getInstance()->replaceScene(tran);
        }
    });
    lbt->setPosition(Vec2(los.width/2, los.height/2));
    lo->addChild(lbt);
    
    auto leveltext = ui::Text::create(StringUtils::format("Level %d",i+1), Common_Font, 40);
    leveltext->setTouchEnabled(false);
//    leveltext->setColor(Color3B::BLACK);
    leveltext->setPosition(Vec2(los.width/3, los.height/2));
    lo->addChild(leveltext);
    
    int step = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level%d_step",levelindex).c_str(),0);
//    int score = UserDefault::getInstance()->getIntegerForKey(StringUtils::format("level%d_score",levelindex).c_str(),0);
    bool lock = UserDefault::getInstance()->getBoolForKey(StringUtils::format("level%d_lock",levelindex).c_str(), true);
    if (lock) {
        auto sp = Sprite::create("q.png");
        sp->setPosition(Vec2(los.width/3*2, los.height/2));
        lo->addChild(sp);
        lbt->setEnabled(false);
    }else{
        auto text = ui::Text::create(StringUtils::format("step:%d",step), Common_Font, 40);
        text->setTouchEnabled(false);
//        text->setColor(Color3B::BLACK);
        text->setPosition(Vec2(los.width/3*2, los.height/2));
        lo->addChild(text);
//        auto scoretext = ui::Text::create(StringUtils::format("score:%d",score), Common_Font, 30);
//        scoretext->setPosition(Vec2(los.width/2, los.height/4));
//        lo->addChild(scoretext);
    }
    
    return lo;
}