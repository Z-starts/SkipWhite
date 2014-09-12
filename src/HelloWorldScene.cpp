#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
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
    
    visibleSize=CCDirector::getInstance()->getVisibleSize();
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    // auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    //    label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    //    this->addChild(label, 1);
    
    // add "HelloWorld" splash screen"
    //    auto sprite = Sprite::create("HelloWorld.png");
    //
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //
    //    auto roting=CCRepeatForever::create(CCRotateBy::create(1.0f, 360));
    //
    //    sprite->runAction(roting);
    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    timerLabel=LabelTTF::create("", "Arial", 24);
    timerLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - 100));
    timerLabel->setColor(Color3B::RED);
    this->addChild(timerLabel,1);
    startGame();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* t, Event* e)
    {
        log("onTouch");
        auto bs = Block::getBlocks();
        Block *b;
        
        for(auto it = bs->begin(); it != bs->end(); it++)
        {
            b = *it;
            
            if(b->getLineIndex()==1&&b->getBoundingBox().containsPoint(t->getLocation()))
            {
                if(b->getColor()==Color3B::BLACK)
                {
                    b->setColor(Color3B::GRAY);
                    this->moveDown();
                    this->startTimer();
                }
                else if(b->getColor()==Color3B::GREEN)
                {
                    this->moveDown();
                    this->stopTimer();
                }
                else
                {
                    MessageBox("GameOver","失败");
                    Block::blocks->clear();
                }
                break;
            }
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//添加黄色开始栏
void HelloWorld::addStartLine()
{
    auto startLine=Block::createWithArgs(ccColor3B::YELLOW, Size(visibleSize.width, visibleSize.height/4), "开始", 30,Color4B::BLACK);
    addChild(startLine);
    linesCount=0;
}
//添加结束的绿色栏，占满屏幕
void HelloWorld::addEndLine()
{
    auto b = Block::createWithArgs(Color3B::GREEN, visibleSize, "Game Over", 30, Color4B::BLACK);
    addChild(b);
    linesCount=0;
}

//添加普通的黑白块栏
void HelloWorld::addNormalLine(int lineIndex)
{
    Block *b;
    int blackIndex = rand()%4;
    for(int i=0; i<4; i++)
    {
        b = Block::createWithArgs(blackIndex == i ? Color3B::BLACK : Color3B::WHITE, Size(visibleSize.width/4-1,visibleSize.height/4-1), "", 20, Color4B::BLACK);
        b->setPosition(i*visibleSize.width/4, lineIndex*visibleSize.height/4);
        b->setLineIndex(lineIndex);
        addChild(b);
    }
    
}

//开始游戏
void HelloWorld::startGame()
{
    addStartLine();
    addNormalLine(1);
    addNormalLine(2);
    addNormalLine(3);
}

void HelloWorld::moveDown()
{
    linesCount+=1;
    if(linesCount<10)
    {
        addNormalLine(4);
        auto bs = Block::getBlocks();
        for(auto it=bs->begin(); it!=bs->end(); it++)
        {
            (*it)->moveDownBlock();
        }
    }
    else if(!showEnd)
    {
        addEndLine();
        showEnd = true;
    }
}

void HelloWorld::update(float dt)
{
    long offset = clock()-startTime;
    
    timerLabel->setString(StringUtils::format("%g",((double)offset)/1000000));
}

//开始计时
void HelloWorld::startTimer()
{
    if(!timeRunning)
    {
        scheduleUpdate();
        startTime = clock();
        timeRunning = true;
    }
}

//结束计时
void HelloWorld::stopTimer()
{
    if(timeRunning)
    {
        unscheduleUpdate();
        timeRunning = false;
    }
}


