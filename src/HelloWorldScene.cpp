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
void HelloWorld::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) 
{ 
	Layer::visit(renderer, parentTransform, parentFlags); //...其他代码 

	_command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void HelloWorld::onDraw()
{
    //获得当前HelloWorld的shader
    auto glProgram = getGLProgram();
   //使用此shader
    glProgram->use();
   //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
    glProgram->setUniformsForBuiltins();
  
    auto size = Director::getInstance()->getWinSize();
    //指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
    float vertercies[] = { 0,0,   //第一个点的坐标
                            size.width, 0,   //第二个点的坐标
                           size.width / 2, size.height};  //第三个点的坐标
    //指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
    float color[] = { 0, 1,0, 1,    //第一个点的颜色，绿色
                        1,0,0, 1,  //第二个点的颜色, 红色
                         0, 0, 1, 1};  //第三个点的颜色， 蓝色
    //激活名字为position和color的vertex attribute
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    //分别给position和color指定数据源
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
    //绘制三角形，所谓的draw call就是指这个函数调用
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();
}

bool HelloWorld::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	//TODO OpenGL eslearn
	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));



	/*SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MULIC_FILE);
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

	create menu, it's an autorelease object
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
	auto bs = WT_Block::getBlocks();
	WT_Block *b;

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

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);*/
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
	auto startLine= WT_Block::createWithArgs(ccColor3B::YELLOW,Size(visibleSize.width,visibleSize.height/4), "",30,ccColor4B::BLACK);
	addChild(startLine);
	startLine->setLineIndex(0);
}
//添加结束的绿色栏，占满屏幕
void HelloWorld::addEndLine()
{
	auto b = WT_Block::createWithArgs(Color3B::GREEN, visibleSize, "Game Over", 30, Color4B::BLACK);
	addChild(b);
	b->setLineIndex(4);
}

//添加普通的黑白块栏
void HelloWorld::addNormalLine(int lineIndex)
{
	WT_Block *b;
	srand(time(NULL));
	int blackIndex = rand()%4;
	for(int i=0; i<4; i++)
	{
		b = WT_Block::createWithArgs(blackIndex == i ? Color3B::BLACK : Color3B::WHITE, Size(visibleSize.width/4-1,visibleSize.height/4-1), "", 20, Color4B::BLACK);
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


	auto bs = WT_Block::getBlocks();

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


