//
//  WT_Block.cpp
//  SkipWhite
//
//  Created by 钟星星 on 14-9-9.
//
//

#include "WT_Block.h"

USING_NS_CC;
WT_Block::WT_Block() {
    
}

WT_Block::~WT_Block()
{
}

//初始化blocks
Vector<WT_Block *> *WT_Block::blocks = new Vector<WT_Block *>();

WT_Block *WT_Block::createWithArgs(Color3B color, Size size, std::string label, float fontSize, Color4B textColor)
{
    auto b = new WT_Block();
    b->initWithArgs(color, size, label, fontSize, textColor);
    b->autorelease();
    
    //将b添加到blocks中
    blocks->pushBack(b);
    
    return b;
}

//初始化参数：方块颜色、方块大小、字、字体大小、字体颜色
bool WT_Block::initWithArgs(Color3B color, Size size, std::string label, float fontSize, Color4B textColor)
{
    
    Sprite::init();
    
    
    //设置大小
    setContentSize(size);
    //设置锚点为左下角
    setAnchorPoint(Point::ZERO);
    //设置区域
    setTextureRect(Rect(0, 0, size.width, size.height));
    //设置颜色
    setColor(color);
    
    //设置文字
    auto myLabel = Label::create();
    myLabel->setString(label);
    myLabel->setSystemFontSize(fontSize);
    myLabel->setTextColor(textColor);
    addChild(myLabel);
    myLabel->setPosition(size.width/2, size.height/2);
    
    return true;
}

//从Vector中移除blocks
void WT_Block::removeBlock()
{
    removeFromParent();
    blocks->eraseObject(this);
}

void WT_Block::setLineIndex(int index){
    this->lineIndex=index;
    
}

int WT_Block::getLineIndex() const{
    return this->lineIndex;
}

Vector<WT_Block *>*WT_Block::getBlocks(){
    return WT_Block::blocks;
}
void WT_Block::moveDownBlock()
{
    this->lineIndex--;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    runAction(Sequence::create(MoveTo::create(0.1f, Point(getPositionX(), lineIndex*visibleSize.height/4)),
                               CallFunc::create([this]()
                                                {
                                                    if(lineIndex<0)
                                                        this->removeBlock();
                                                }), NULL));
}
