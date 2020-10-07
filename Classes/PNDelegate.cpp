#include "PNDelegate.h"
USING_NS_CC;
using namespace ui;
using std::string;

void PNDelegate::editBoxEditingDidBegin(EditBox* editBox)
{
	log("editbox %p Did begin", editBox);
}

void PNDelegate::editBoxEditingDidEnd(EditBox* editBox)
{
	log("editBox %p DidEnd !", editBox);
	strcpy_s(User::data.userName, editBox->getText());
	User::writeOut();
}

void PNDelegate::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void PNDelegate::editBoxReturn(EditBox* editBox)
{
	log("editBox %p was returned !", editBox);
}
