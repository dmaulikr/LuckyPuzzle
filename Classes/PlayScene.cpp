//------.cpp
#include "PlayScene.h"
#include "PHSprite.h"
#include "UILayout.h"
#include "LHMacros.h"
#include "UIButton.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "GameOverScene.h"

USING_NS_CC;
Scene* PlayScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = PlayScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

PlayScene::~PlayScene(){
    log("~PlayScene");
}

void PlayScene::update(float delta){
    
}

#define WHITE 3
#define RED 2
#define BLUE 1

static int target[10][10];
static int from[10][10];

bool PlayScene::initDict(cocos2d::CCDictionary *dic)
{
    //level
    CCInteger *il = (CCInteger*)dic->objectForKey("level");
    levelValue = il->getValue();
    initDataFromLevel(levelValue);
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();

    bannerHei = vs.height / 12;
    playViewHei = (vs.height - bannerHei -20 ) /2;
    playViewWID = vs.width - 10;
    
    stepValue = 0;
    auto steptext = ui::Text::create(StringUtils::format("%d",stepValue), Common_Font, 35);
    steptext -> setPosition(Vec2(vs.width + vo.x - steptext->getContentSize().width/2 - 50, vs.height - bannerHei/2 + vo.y));
    steptext -> setColor(Color3B::BLACK);
    this->addChild(steptext);
    step = steptext;
    
    auto leveltext = ui::Text::create(StringUtils::format("Level-%d",levelValue), Common_Font, 35);
    leveltext -> setPosition(Vec2(vs.width/2 + vo.x, vs.height - bannerHei/2 + vo.y));
    leveltext -> setColor(Color3B::BLACK);
    this->addChild(leveltext);
    
    auto play = ui::Button::create("rb.png");
    play->setTitleText("Back");
    play->setScale(0.5);
    play->setTitleFontSize(36);
    play->setPosition(Vec2(10 + play->getContentSize().width/2, vs.height - bannerHei/2 + vo.y));
    play->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(HelloWorld::createScene());
        }
    });
    this->addChild(play);
    
    for (int i = 0 ; i<HEI; i++) {
        for (int j = 0 ; j<WID; j++) {
            int tag = from[i][j];
            if(tag == WHITE){
                wi = i;
                wj = j;
                auto sp = Sprite::create("rw.png");
                sp->setTag(WHITE);
                this->addRectAt(sp,i,j,false);
            }else if(tag == RED){
                auto sp = Sprite::create("rr.png");
                sp->setTag(RED);
                this->addRectAt(sp,i,j,false);
            }else{
                auto sp = Sprite::create("rb.png");
                sp->setTag(BLUE);
                this->addRectAt(sp,i,j,false);
            }
        }
    }
    
    //target
    for (int i = 0 ; i<HEI; i++) {
        for (int j = 0 ; j<WID; j++) {
            int tag = target[i][j];
            if(tag == WHITE){
                auto sp = Sprite::create("rw.png");
                sp->setTag(WHITE);
                this->addRectAt(sp,i,j,true);
            }else if(tag == RED){
                auto sp = Sprite::create("rr.png");
                sp->setTag(RED);
                this->addRectAt(sp,i,j,true);
            }else{
                auto sp = Sprite::create("rb.png");
                sp->setTag(BLUE);
                this->addRectAt(sp,i,j,true);
            }
        }
    }
    
    auto lis = EventListenerTouchOneByOne::create();
    lis->setSwallowTouches(true);
    lis->onTouchBegan = [this](Touch*, Event*){
        canMove = true;
        return true;
    };
    lis->onTouchMoved =[this](Touch* touch, Event*){
        if (canMove == false) {
            return;
        }
        
        Vec2 last = touch->getPreviousLocation();
        Vec2 cur = touch->getLocation();
        if (cur.x - last.x > 20) {
            canMove = false;
            this->moveWhiteTo(wi , wj + 1);
            return;
        }
        if(cur.x - last.x < -20){
            canMove = false;
            this->moveWhiteTo(wi , wj - 1);
            return;
        }
        if (cur.y - last.y > 20) {
            canMove = false;
            this->moveWhiteTo(wi +1 , wj );
            return;
        }
        if(cur.y - last.y < -20){
            canMove = false;
            this->moveWhiteTo(wi -1, wj );
            return;
        }
    };
    lis->onTouchEnded = [](Touch* touch, Event*){
        
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, this);
    
	return true;
}

void PlayScene::initDataFromLevel(int level){
    std::string s = cocos2d::FileUtils::getInstance()->getStringFromFile(StringUtils::format("puzzle%d.json",level));
    
    rapidjson::Document jdoc;
    jdoc.Parse<0>(s.c_str());
    if (jdoc.HasParseError()) {
        log("parse json error.");
        return;
    }
    
    int ac = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(jdoc, "layers");
    for (int i =0 ;i<ac; i++) {
        const rapidjson::Value &ly = cocostudio::DictionaryHelper::getInstance()->getDictionaryFromArray_json(jdoc, "layers", i);
        const char *name = cocostudio::DictionaryHelper::getInstance()->getStringValue_json(ly, "name");
        int ph = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(ly, "height");
        int pw = cocostudio::DictionaryHelper::getInstance()->getIntValue_json(ly, "width");
        
        WID = pw;
        HEI = ph;
        
        int datacount = cocostudio::DictionaryHelper::getInstance()->getArrayCount_json(ly, "data");
        for (int j = 0 ; j<datacount; j++) {
            int v = cocostudio::DictionaryHelper::getInstance()->getIntValueFromArray_json(ly, "data", j);
            int ai = ph - j/pw - 1;
            int aj = j%pw;
            if (strcmp(name, "from")==0) {
                from[ai][aj] = v;
            }else{
                target[ai][aj] = v;
            }
        }
    }
}

bool PlayScene::checkWin(){
    
    for (int i = 0 ; i<HEI; i++) {
        for (int j = 0 ; j<WID; j++){
            int tmp = rects[i][j]->getTag();
            if (tmp!=target[i][j]) {
                return false;
            }
        }
    }
    
    return true;
}

void PlayScene::moveWhiteTo(int i, int j){
    log("move white to %d,%d",i,j);
    if (i<0||i>=HEI||j<0||j>=WID) {
        return;
    }
    
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    Sprite *tmp = rects[i][j];
    Sprite *white = rects[wi][wj];
    
    Vec2 tmppos = tmp->getPosition();
    float mvtime = 0.15f;
    auto mvt = MoveTo::create(mvtime, white->getPosition());
    tmp->runAction(mvt);
    auto mvt2 = MoveTo::create(mvtime, tmppos);
    auto call = CallFunc::create([this](){
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
        
        stepValue ++;
        step->setString(StringUtils::format("%d",stepValue));
        
        if (this->checkWin()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("seegod.wav");
            
            Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
            Size vs = Director::getInstance()->getVisibleSize();
            Vec2 vo = Director::getInstance()->getVisibleOrigin();
            
            Vec2 topos = Vec2(vs.width/2 + vo.x,vs.height/2 + vo.y);
            
            auto winlay = ui::Layout::create();
            winlay->setSize(vs);
            auto text = ui::Text::create("You Win!", Common_Font, 55);
            winlay->addChild(text);
            text->setPosition(topos);
            text->setColor(Color3B::GREEN);
            
            this->addChild(winlay);
            
            auto dl = DelayTime::create(1);
            auto call = CallFunc::create([this](){
                Dictionary *dict = Dictionary::create();
                dict->setObject(CCInteger::create(stepValue), "step");
                dict->setObject(CCInteger::create(levelValue),"level");
                auto tran = TransitionSlideInR::create(0.5, GameOverScene::createScene(dict));
                Director::getInstance()->replaceScene(tran);
            });
            auto sq = Sequence::create(dl,call, NULL);
            this->runAction(sq);
            
            return;
        }else{
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("move.wav");
        }
    });
    auto sq = Sequence::create(mvt2,call, NULL);
    white->runAction(sq);
    
    rects[wi][wj] = tmp;
    rects[i][j] = white;
    
    wi = i;
    wj = j;
}

void PlayScene::addRectAt(Sprite *sp,int i,int j,bool targ){
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    float base = playViewWID>playViewHei?playViewHei:playViewWID;
    
    float basew = base/WID;
    float sy = (vs.height - bannerHei)/4 - basew*HEI/2 + vo.y + 5;
    if (targ) {
        sy = (vs.height - bannerHei)/4 - basew*HEI/2 + vo.y + 15 + playViewHei;
    }
    
    float x = playViewWID/2 - basew*WID/2 + basew/2 + basew*j;
    float y = sy + basew/2 + basew*i;
    
    sp->setPosition(Vec2(x, y));
    sp->setScale(basew/sp->getContentSize().width);
    this->addChild(sp);
    
    if (!targ) {
        rects[i][j]=sp;
    }
}