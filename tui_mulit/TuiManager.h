#ifndef __TUIMANAGER_H__
#define __TUIMANAGER_H__

#include "../../rapidXml/rapidxml.hpp"  
#include "../../rapidXml/rapidxml_utils.hpp"  
#include "../../rapidXml/rapidxml_print.hpp"

#include "tui/utils/AnimationUtil.h"
#include "tui/TuiBase.h"
#include "tui/TuiMacros.h"

#include "GUI/CCEditBox/CCEditBox.h"

using namespace extension;
using namespace rapidxml;
using namespace std;

class TuiManager : public Ref{
public:
	SINGLE_INSTANCE_FUNC(TuiManager);
	bool virtual init();

	void parseScene(TuiBase* pScene ,const char* sceneName,const char* xmlPath);//解析场景

	void setUseSpriteFrame(bool b);//是否使用SpriteFrame

public:

	CWidgetWindow *createPanel(float tag,float x,float y,float rotation);
	CLayout *createLayout(float tag,float x,float y,float w,float h,float rotation);
	CScrollView *createScrollView(float tag,float x,float y,float w,float h,float rotation);
	CListView *createListView(float tag,const char* img,float x,float y,float w,float h,float rotation);
	CPageView *createPageView(float tag,float x,float y,float w,float h,float rotation);
	CImageView *createImage(float tag,const char* file,float x,float y,float rotation);
	CImageViewScale9 *createImage9(float tag,const char* file,float x,float y,float w,float h,float up,float down,float left, float right,float rotation);
	CButton *createBtn(float tag,const char* normal,const char* select,const char* disable,float x,float y, float w,float h,float rotation);
	CToggleView *createToggleView(float tag,int exclusion,const char* normal,const char* select,const char* disable,float x,float y,float rotation);
	CSlider *createSlider(float tag, const char* bg,const char* progress,const char* thumb,float x,float y,float rotation);
	CProgressBar *createProgress(float tag, const char* bg,const char* progress,float x,float y,float rotation);
	CLabel *createLabel(float tag, const char* text,const char* font,float fontSize,int r,int g,int b,float x,float y,float w,float h,int r2,int g2,int b2,float strokeSize,int shadowDistance,float shadowBlur,float rotation);
	CLabelAtlas *createLabelAtlas(float tag,const char* file,float x,float y,float w,float h,float rotation);
	Armature *createArmature(float tag,const char* name,const char* png,const char* plist,const char* xml,float x,float y,float rotation);
	Sprite *createAnim(float tag,const char* name,const char* png,const char* plist,float x,float y,float rotation);
	CControlView *createControl(float tag,const char* baseboard,const char* joystick,float x,float y,float rotation);
	CCheckBox *createCheckBox(float tag,const char* normal1,const char* normal2,const char* select1,const char* select2,const char* disable1,const char* disable2,float x,float y,float rotation);
	ArmatureBtn *createArmatureBtn(float tag,const char* name,const char* png,const char* plist,const char* xml,float x,float y,float rotation);
	NumericStepper *createNumStep(float tag,const char* lnormal,const char* lselect,const char* ldisable,const char* rnormal,const char* rselect,const char* rdisable,const char* stepBg,float x,float y,float rotation);
	ParticleSystemQuad *createParticle(float tag,const char* plist,float x,float y);
	CTableView *createTableView(float tag,float x,float y,float w,float h,float rotation);
	EditBox *createEditBox(float tag,const char* file,float x,float y,float w,float h,float rotation);

protected:
	void loadXml(const string& pathXML);	//加载xml
	void parseControl(Node* container,xml_node<char>*);//解析组件
private:
	//string m_fileContent;//xml解析后的字符数组
	map<string,string> m_DataMap;//xml映射内容
	bool m_isUseSpriteFrame;
};
#endif

