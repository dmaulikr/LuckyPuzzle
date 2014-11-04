//------.h
#ifndef __LEVELSCENE__
#define __LEVELSCENE__
#include "cocos2d.h"
#include "UILayout.h"

#define LVCOUNT 10

class LevelScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~LevelScene();
	CREATE_FUNC(LevelScene);
private:
    cocos2d::ui::Layout* getUI(int levelindex);
};
#endif
