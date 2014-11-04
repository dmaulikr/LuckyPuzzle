//------.h
#ifndef __PLAYSCENE__
#define __PLAYSCENE__
#include "cocos2d.h"
#include "UIText.h"

#define MAXA 10

class PlayScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~PlayScene();
    virtual void update(float delta);
	CREATE_FUNC(PlayScene);
private:
    float bannerHei;
    float playViewHei;
    float playViewWID;
    
    cocos2d::ui::Text *step;
    int stepValue;
    int levelValue;
    
    int WID;
    int HEI;
    
    void addRectAt(cocos2d::Sprite *sp,int i,int j,bool targ);
    void moveWhiteTo(int i,int j);
    int wi;
    int wj;
    cocos2d::Sprite *rects[MAXA][MAXA];
    
    bool canMove;
    
    bool checkWin();
    
    void initDataFromLevel(int level);
};
#endif