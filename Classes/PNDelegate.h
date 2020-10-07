#ifndef __PNDELEGATE_H_
#define __PNDELEGATE_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "user.h"

class PNDelegate: public cocos2d::ui::EditBoxDelegate {
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
};

#endif
