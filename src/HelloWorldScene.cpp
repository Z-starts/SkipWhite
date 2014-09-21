#include "HelloWorldScene.h"


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

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MULIC_FILE);
    SimpleAudioEngine::getInstance()->preloadEffect(CLICK_MULIC_FILE);
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::getInstance()->setEffectsVolume(0.3);
    
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(BACKGROUND_MULIC_FILE);
    SimpleAudioEngine::getInstance()->playBackgroundMusic(fullpath.c_str(),true);
    
    
    visibleSize=CCDirector::getInstance()->getVisibleSize();
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
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
    
    timerLabel= Label::create("Ready?", "Arial", 30);
    timerLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - 100));
    timerLabel->setColor(Color3B::RED);
    this->addChild(timerLabel,1);
    startGame();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* t, Event* e)
    {
        log("onTouch");
        m_nSoundId=SimpleAudioEngine::getInstance()->playEffect(std::string(FileUtils::getInstance()->fullPathForFilename(CLICK_MULIC_FILE)).c_str(), false);
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
                    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
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
	auto startLine=Block::createWithArgs(ccColor3B::YELLOW,Size(visibleSize.width,visibleSize.height/4), "",30,ccColor4B::BLACK);
    addChild(startLine);
    startLine->setLineIndex(0);
}
//添加结束的绿色栏，占满屏幕
void HelloWorld::addEndLine()
{
    auto b = Block::createWithArgs(Color3B::GREEN, visibleSize, "Game Over", 30, Color4B::BLACK);
    addChild(b);
    b->setLineIndex(4);
}

//添加普通的黑白块栏
void HelloWorld::addNormalLine(int lineIndex)
{
    Block *b;
    srand(time(NULL));
    int blackIndex = rand()%4;
    for(int i=0; i<4; i++)
    {
        b = Block::createWithArgs(blackIndex == i ? Color3B::BLACK : Color3B::WHITE, Size(visibleSize.width/4-1,visibleSize.height/4-1), "", 20, Color4B::BLACK);
        b->setPosition(i*visibleSize.width/4, lineIndex*visibleSize.height/4);
        b->setLineIndex(lineIndex);
        addChild(b);
    }
    linesCount++;
}

//开始游戏
void HelloWorld::startGame()
{
    linesCount = 0;
    showEnd = false;
    timeRunning = false;
    
    addStartLine();
    
    addNormalLine(1);
    addNormalLine(2);
    addNormalLine(3);
}

void HelloWorld::moveDown()
{
    if (linesCount<30) {
        addNormalLine(4);
    }else if(!showEnd){
        addEndLine();
        showEnd = true;
    }
    
    
    auto bs = Block::getBlocks();
    
    for (auto it = bs->begin(); it!=bs->end(); it++) {
        (*it)->moveDownBlock();
    }
}

void HelloWorld::update(float dt)
{
    long offset = clock()-startTime;
    
    timerLabel->setString(StringUtils::format("%g",((double)offset)/1000));
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


