#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "WT_Block.h"


#define BACKGROUND_MULIC_FILE "music/music_taohuayuanBG.mp3"
#define CLICK_MULIC_FILE "sounds/sound_click.mp3"

USING_NS_CC;
//using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
public:
	//其它函数省略
	virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags); 
    //virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated) override;
	void onDraw();


    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    /*
     *黄色栏大小
     */
     
    Size visibleSize;
    //添加开始的黄色栏
    void addStartLine();
    //添加结束的绿色栏，占满屏幕
    void addEndLine();
    
    void addNormalLine(int lineIndex);
    //开始游戏
    void startGame();
    
    void moveDown();
    
    int linesCount;
    bool showEnd;
    
    long startTime;
    bool timeRunning;
    
    //开始计时
    void startTimer();
    
    //结束计时
    void stopTimer();
    
    virtual void update(float dt);
    //点击音效id
    int    m_nSoundId;
private:
	CustomCommand _command;

    Label* timerLabel;
};

#endif // __HELLOWORLD_SCENE_H__
