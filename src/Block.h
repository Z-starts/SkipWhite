//
//  Block.h
//  SkipWhite
//
//  Created by 钟星星 on 14-9-9.
//
//

#ifndef SkipWhite_Block_h
#define SkipWhite_Block_h

#include "CCSprite.h"
#include "cocos2d.h"


USING_NS_CC;

class Block : public cocos2d::Sprite
{
public:
    Block();
    virtual ~Block();
    int lineIndex;
    
    static Block* createWithArgs(Color3B color, Size size, std::string label, float fontSize, Color4B textColor);
    
    //初始化参数：方块颜色、方块大小、字、字体大小、字体颜色
    virtual bool initWithArgs(Color3B color, Size size, std::string label, float fontSize, Color4B textColor);
    
    //用来存放block
    static Vector<Block*> * blocks;
    void removeBlock();
    
    
    virtual void setLineIndex(int index);
    virtual int getLineIndex() const;
    
    static Vector<Block*> * getBlocks();
    
    void moveDownBlock();
};
#endif
