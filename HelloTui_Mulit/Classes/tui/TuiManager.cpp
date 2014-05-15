#include "TuiManager.h"
#include "tuiconsts.h"


bool TuiManager::init()
{
	m_isUseSpriteFrame = false;
	return true;
}

void TuiManager::parseScene(TuiBase* pScene ,const char* sceneName,const char* xmlPath){
	//判断是否用SpriteFrame创建控件
	m_isUseSpriteFrame = pScene->getAutoRemoveUnusedSpriteFrame();

	loadXml(xmlPath);
	string xmlContent = m_DataMap.at(xmlPath);
	char* buf = new char[ xmlContent.size()+1 ];
	memcpy( buf, xmlContent.c_str(), xmlContent.size()+1 );
	
	xml_document<> doc;
	doc.parse<0>(buf);

	for(xml_node<char> *item = doc.first_node("control");item != NULL;item = item->next_sibling()){
		
		if( strcmp(item->first_attribute("type")->value(),kTuiContainerPanel) == 0){//panel

			if(strcmp(item->first_attribute("name")->value(),sceneName) != 0) continue;//只解析当前场景

			this->parseControl(pScene,item);
		}
	}
}
////////////////////解析组件/////////////////////////////////////////////
void TuiManager::parseControl(Node* container,xml_node<char> *item)
{
	int tag = atof(item->first_attribute("tag")->value());
	int x = atof(item->first_attribute("x")->value());
	int y = atof(item->first_attribute("y")->value());
	int rotation = atof(item->first_attribute("rotation")->value());

	if(strcmp(item->first_attribute("type")->value(), kTuiContainerPanel) == 0){//panel
		CWidgetWindow* pPanel = createPanel(tag,x,y,rotation);
		container->addChild(pPanel);
		//递归
		for( xml_node<char> *iitem = item->first_node( kTuiNodeControl );iitem != NULL; iitem = iitem->next_sibling()){
			parseControl(pPanel,iitem);
		}

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlImage) == 0){//image
		const char* file = item->first_attribute("image")->value();
		CImageView *pImg = createImage(tag,file,x,y,rotation);
		container->addChild(pImg);
	
	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlImage9) == 0){//image9
		const char* file = item->first_attribute("image")->value();
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		float up = atof(item->first_attribute("up")->value());
		float down = atof(item->first_attribute("down")->value());
		float left = atof(item->first_attribute("left")->value());
		float right = atof(item->first_attribute("right")->value());
		CImageViewScale9 *pImg = createImage9(tag,file,x,y,w,h,up,down,left,right,rotation);
		container->addChild(pImg);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlButton) == 0){//button
		const char* normal = item->first_attribute("normal")->value();
		const char* select = item->first_attribute("select")->value();
		const char* disable = item->first_attribute("disable")->value();
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		CButton *pBtn = createBtn(tag,normal,select,disable,x,y,w,h,rotation);
		container->addChild(pBtn);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlToggleView) == 0){//toggleView
		const char* normal = item->first_attribute("normal")->value();
		const char* select = item->first_attribute("select")->value();
		const char* disable = item->first_attribute("disable")->value();
		float exclusion = atof(item->first_attribute("exclusion")->value());
		CToggleView* toggle = createToggleView(tag,exclusion,normal,select,disable,x,y,rotation);
		container->addChild(toggle);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlSlider) == 0){//slider
		const char* bg = item->first_attribute("bg")->value();
		const char* progress = item->first_attribute("progress")->value();
		const char* thumb = item->first_attribute("thumb")->value();
		CSlider *pSlider = createSlider(tag,bg,progress,thumb,x,y,rotation);
		container->addChild(pSlider);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlProgress) == 0){//progress
		const char* bg = item->first_attribute("bg")->value();
		const char* progress = item->first_attribute("progress")->value();
		CProgressBar *pProgress = createProgress(tag,bg,progress,x,y,rotation);
		container->addChild(pProgress);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlLabel) == 0){//label
		float size = atof(item->first_attribute("textSize")->value());
		const char* text = item->first_attribute("text")->value();
		const char* font = item->first_attribute("textFont")->value();
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		
		int r = atoi(item->first_attribute("red")->value());
		int g = atoi(item->first_attribute("green")->value());
		int b = atoi(item->first_attribute("blue")->value());
		int r2 = atoi(item->first_attribute("strokeRed")->value());
		int g2 = atoi(item->first_attribute("strokeGreen")->value());
		int b2 = atoi(item->first_attribute("strokeBlue")->value());
		int strokeSize = atoi(item->first_attribute("strokeSize")->value());
		int shadowDistance = atoi(item->first_attribute("shadowDistance")->value());
		int shadowBlur = atoi(item->first_attribute("shadowBlur")->value());
		CLabel *pLabel = createLabel(tag,text,font,size,r,g,b,x,y,w,h,
									r2,g2,b2,strokeSize,shadowDistance,shadowBlur,rotation);
		container->addChild(pLabel);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlLabelAtlas) == 0){//labelAtlas
		const char* imgPath = item->first_attribute("image")->value();
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		CLabelAtlas *pLabAtlas = createLabelAtlas(tag,imgPath,x,y,w,h,rotation);
		container->addChild(pLabAtlas);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlArmature) == 0){//armature
		const char* xml = item->first_attribute("xml")->value();
		const char* png = item->first_attribute("png")->value();
		const char* plist = item->first_attribute("plist")->value();
		const char* name = item->first_attribute("name")->value();
		Armature *pArmature = createArmature(tag,name,png,plist,xml,x,y,rotation);
		container->addChild(pArmature);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlAnim) == 0){//animation
		const char* png = item->first_attribute("png")->value();
		const char* plist = item->first_attribute("plist")->value();
		const char* name = item->first_attribute("name")->value();
		Sprite *pSprite = createAnim(tag,name,png,plist,x,y,rotation);
		container->addChild(pSprite);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlControl) == 0){//controlView
		const char* baseboard = item->first_attribute("baseboard")->value();
		const char* joystick = item->first_attribute("joystick")->value();
		CControlView *pControl = createControl(tag,baseboard,joystick,x,y,rotation);
		container->addChild(pControl);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiContainerScroll) == 0){//scrollView
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		CScrollView *pView = createScrollView(tag,x,y,w,h,rotation);
		container->addChild(pView);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlListView) == 0){//listView
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		const char* img = item->first_attribute("image")->value();
		float num = atof(item->first_attribute("num")->value());
		CListView* pList = createListView(tag,img,x,y,w,h,rotation);
		container->addChild(pList);

		for(int i=0; i<num;i++){//添加item
			xml_node<char> *iitem = item->first_node( kTuiNodeControl );
			w = atof(iitem->first_attribute("width")->value());
			h = atof(iitem->first_attribute("height")->value());

			CLayout *pLayout = createLayout(i,0,0,w,h,rotation);
			for( xml_node<char> *iiitem = iitem->first_node( kTuiNodeControl );iiitem!=NULL; iiitem = iiitem->next_sibling()){
				parseControl(pLayout,iiitem);
			}
			Vector<Node*> vet = pLayout->getChildren();
			for(Node *pChild : vet){//偏移坐标 因为CLayout的零点在左下角
				pChild->setPosition(pChild->getPosition()+Point(w/2,h/2));
			}
			pList->insertNodeAtLast(pLayout);
		}
		pList->reloadData();

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlPageView) == 0){//pageView
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		CPageView *pPageView = createPageView(tag,x,y,w,h,rotation);
		container->addChild(pPageView);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlCheckBox) == 0){//checkBox
		const char* normal1 = item->first_attribute("normal1")->value();
		const char* normal2 = item->first_attribute("normal2")->value();
		const char* select1 = item->first_attribute("select1")->value();
		const char* select2 = item->first_attribute("select2")->value();
		const char* disable1 = item->first_attribute("disable1")->value();
		const char* disable2 = item->first_attribute("disable2")->value();
		CCheckBox *pCheckBox = createCheckBox(tag,normal1,normal2,select1,select2,disable1,disable2,x,y,rotation);
		container->addChild(pCheckBox);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlArmatureBtn) == 0){//ArmatureBtn
		const char* png = item->first_attribute("png")->value();
		const char* plist = item->first_attribute("plist")->value();
		const char* name = item->first_attribute("name")->value();
		const char* xml = item->first_attribute("xml")->value();
		ArmatureBtn *pArmBtn = createArmatureBtn(tag,name,png,plist,xml,x,y,rotation);
		container->addChild(pArmBtn);

	}else if(strcmp(item->first_attribute("type")->value(),kTuicontrolNumbericStepper) == 0){//NumbericStepper
		const char* lnormal = item->first_attribute("lnormal")->value();
		const char* rnormal = item->first_attribute("rnormal")->value();
		const char* lselect = item->first_attribute("lselect")->value();
		const char* rselect = item->first_attribute("rselect")->value();
		const char* ldisable = item->first_attribute("ldisable")->value();
		const char* rdisable = item->first_attribute("rdisable")->value();
		const char* stepBg = item->first_attribute("stepBg")->value();
		NumericStepper *pNumStep = createNumStep(tag,lnormal,lselect,ldisable,rnormal,rselect,rdisable,stepBg,x,y,rotation);
		container->addChild(pNumStep);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlPaticle) == 0){//Paticle
		const char* plist = item->first_attribute("plist")->value();
		ParticleSystem *pPartical = createParticle(tag,plist,x,y);
		container->addChild(pPartical);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlTable) == 0){//TableView
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		CTableView *pView = createTableView(tag,x,y,w,h,rotation);
		container->addChild(pView);

	}else if(strcmp(item->first_attribute("type")->value(),kTuiControlEditBox) == 0){//EditBox
		float w = atof(item->first_attribute("width")->value());
		float h = atof(item->first_attribute("height")->value());
		const char* img = item->first_attribute("image")->value();
		EditBox *pEdit = createEditBox(tag,img,x,y,w,h,rotation);
		container->addChild(pEdit);

	}
}

///创建组件 ////////////////////////////////////////////////////////////////
CWidgetWindow *TuiManager::createPanel(float tag,float x,float y,float rotation){
	CWidgetWindow *pPanel = CWidgetWindow::create();
	pPanel->setPosition(Point(x,y));
	pPanel->setLocalZOrder(INT_MAX);
	pPanel->setRotation(rotation);
	pPanel->setTag(tag);
	return pPanel;
}

CLayout *TuiManager::createLayout(float tag,float x,float y,float w,float h,float rotation){
	CLayout *pLayout = CLayout::create(Size(w,h));
	pLayout->setPosition(Point(x,y));
	pLayout->setRotation(rotation);
	pLayout->setTag(tag);
	return pLayout;
}

CScrollView *TuiManager::createScrollView(float tag,float x,float y,float w,float h,float rotation){
	CScrollView *pView = CScrollView::create(Size(Point(w,h)));
	pView->setPosition(Point(x,-y));
	pView->setContainerSize(Size(w,h));
	pView->setDirection(eScrollViewDirectionBoth);
	pView->setRotation(rotation);
	pView->setTag(tag);
	return pView;
}

CListView *TuiManager::createListView(float tag,const char* img,float x,float y,float w,float h,float rotation){
	CListView *pList = CListView::create(Size(w,h));
	m_isUseSpriteFrame ? pList->setBackgroundSpriteFrameName(img) : pList->setBackgroundImage(img);
	pList->setDirection(eScrollViewDirectionVertical);
	pList->setPosition(Point(x,-y));
	pList->setRotation(rotation);
	pList->setTag(tag);
	return pList;
}

CPageView *TuiManager::createPageView(float tag,float x,float y,float w,float h,float rotation){
	CPageView *pPage = CPageView::create(Size(w,h));
	pPage->setSizeOfCell(Size(w,h));
	pPage->setPosition(Point(x,-y));
	pPage->setRotation(rotation);
	pPage->setTag(tag);
	return pPage;
}

CImageView *TuiManager::createImage(float tag, const char* file,float x,float y,float rotation){
	CImageView *pSprite = m_isUseSpriteFrame ? CImageView::createWithSpriteFrameName(file) : CImageView::create(file);
	Size size = pSprite->getContentSize();
	pSprite->setPosition(Point(x,-y));
	pSprite->setTag(tag);
	return pSprite;
}

CImageViewScale9 *TuiManager::createImage9(float tag,const char* file,float x,float y,float w,float h,float up,float down,float left, float right,float rotation){
	CImageViewScale9* pSprite = NULL;
	if(m_isUseSpriteFrame){
		pSprite = CImageViewScale9::createWithSpriteFrameName(file,Rect(up,down,left,right));
	}else{
		CImageView *temp = CImageView::create(file);
		Size size = temp->getContentSize();
		pSprite = CImageViewScale9::create(file, Rect(0,0,size.width,size.height),Rect(up,down,left,right));
	}
	pSprite->setContentSize(Size(w,h));
	pSprite->setPosition(Point(x,-y));
	pSprite->setTag(tag);
	return pSprite;
}

CButton* TuiManager::createBtn(float tag, const char* normal,const char* select,const char* disable,float x,float y,float w, float h,float rotation){
	CButton * pBtn = NULL;
	if(m_isUseSpriteFrame){
		pBtn = CButton::create();
		pBtn->setNormalSpriteFrameName(normal);
		pBtn->setSelectedSpriteFrameName(select);
		pBtn->setDisabledSpriteFrameName(disable);
	}else{
		pBtn = CButton::createWith9Sprite(Size(w,h),normal,select,disable);
	}
	pBtn->setPosition(Point(x,-y));
	pBtn->setTag(tag);
	return pBtn;
}

CToggleView* TuiManager::createToggleView(float tag,int exclusion,const char* normal,const char* select,const char* disable,float x,float y,float rotation){
	CToggleView *pToggle = NULL;
	if(m_isUseSpriteFrame){
		pToggle = CToggleView::create();
		pToggle->setNormalSpriteFrameName(normal);
		pToggle->setSelectedSpriteFrameName(select);
		pToggle->setDisabledSpriteFrameName(disable);
	}else{
		pToggle = CToggleView::create(normal,select,disable);
	}
	pToggle->setExclusion(exclusion);
	pToggle->setPosition(x,-y);
	pToggle->setTag(tag);
	return pToggle;
}

CSlider* TuiManager::createSlider(float tag, const char* bg,const char* progress,const char* thumb,float x,float y,float rotation){
	CSlider *pSlider = NULL;
	if(m_isUseSpriteFrame){
		pSlider = CSlider::create();
		pSlider->setBackgroundSpriteFrameName(bg);
		pSlider->setProgressSpriteFrameName(progress);
		pSlider->setSliderSpriteFrameName(thumb);
	}else{
		pSlider = CSlider::create(thumb,progress);
		pSlider->setBackgroundImage(bg);
	}
	pSlider->setPosition(Point(x,-y));
	pSlider->setMinValue(0);
	pSlider->setMaxValue(100);
	pSlider->setValue(15);
	pSlider->setTag(tag);
	return pSlider;
}

CProgressBar* TuiManager::createProgress(float tag, const char* bg,const char* progress,float x,float y,float rotation){
	CProgressBar *pProgress = NULL;
	if(m_isUseSpriteFrame){
		pProgress = CProgressBar::create();
		pProgress->setBackgroundSpriteFrameName(bg);
		pProgress->setProgressSpriteFrameName(progress);
	}else{
		pProgress = CProgressBar::create(progress);
		pProgress->setBackgroundImage(bg);
	}
	pProgress->setPosition(Point(x,-y));
	pProgress->setMaxValue(100);
	pProgress->setMinValue(0);
	pProgress->setValue(15);
	pProgress->setTag(tag);
	return pProgress;
}

CLabel* TuiManager::createLabel(float tag, const char* text,const char* font,float fontSize,int r,int g,int b,float x,float y,float w,float h,
							int r2,int g2,int b2,float strokeSize,int shadowDistance,float shadowBlur,float rotation)
{
	CLabel *pLabel = CLabel::createWithTTF(text,font,fontSize);

	if(shadowDistance != 0){
		pLabel->setTextColor(Color4B(r,g,b,255));
		pLabel->enableShadow(Color4B(r2,g2,b2,255),Size(shadowDistance,shadowDistance),shadowBlur);
	}
	if(strokeSize != 0){
		pLabel->setTextColor(Color4B(r,g,b,255));
		pLabel->enableOutline(Color4B(r2,g2,b2,255),strokeSize);
	}
	if(shadowDistance == 0 && strokeSize == 0){
		pLabel->setColor(Color3B(r,g,b));
	}
	pLabel->setDimensions(w,h);
	pLabel->setPosition(Point(x+w/2,-(y + h/2)));
	pLabel->setTag(tag);
	
	return pLabel;
}

CLabelAtlas* TuiManager::createLabelAtlas(float tag,const char* imgPath,float x,float y,float w,float h,float rotation){
	CLabelAtlas *pLabAtlas = CLabelAtlas::create("123456",imgPath,w/12,h,48);
	pLabAtlas->setPosition(Point(x,-y));
	pLabAtlas->setTag(tag);
	return pLabAtlas;
}

Armature* TuiManager::createArmature(float tag,const char* name,const char* png,const char* plist,const char* xml,float x,float y,float rotation){
	if(m_isUseSpriteFrame){
		ArmatureDataManager::getInstance()->addSpriteFrameFromFile(plist,png,xml);
	}else{
		ArmatureDataManager::getInstance()->addArmatureFileInfo(png,plist,xml);
	}
	Armature *pArmature = Armature::create(name);
	pArmature->setPosition(Point(x,-y));
	pArmature->setTag(tag);
	pArmature->setRotation(rotation);
	return pArmature;
}

Sprite* TuiManager::createAnim(float tag,const char* name,const char* png,const char* plist,float x,float y,float rotation){
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist,png);
	Animation* pAnim = AnimationUtil::createAnimWithName(name,0.05f,-1);
	Sprite* pSprite = Sprite::create();
	pSprite->runAction(Animate::create(pAnim));
	pSprite->setPosition(Point(x,-y));
	pSprite->setTag(tag);
	return pSprite;
}

CControlView* TuiManager::createControl(float tag,const char* baseboard,const char* joystick,float x,float y,float rotation){
	CControlView* pView = NULL;
	if(m_isUseSpriteFrame){
		pView = CControlView::create();
		pView->setBaseBoardSpriteFrameName(baseboard);
		pView->setJoystickSpriteFrameName(joystick);
	}else{
		pView = CControlView::create(baseboard,joystick);
	}
	pView->setPosition(Point(x, -y));
	pView->setRadius(pView->getContentSize().width / 2);
	pView->setRotation(rotation);
	pView->setTag(tag);
	return pView;
}

CCheckBox* TuiManager::createCheckBox(float tag,const char* normal1,const char* normal2,const char* select1,
	const char* select2,const char* disable1,const char* disable2,float x,float y,float rotation){
	CCheckBox* pCheckBox = NULL;
	if(m_isUseSpriteFrame){
		pCheckBox = CCheckBox::create();
		pCheckBox->setNormalSpriteFrameName(normal1);
		pCheckBox->setNormalPressSpriteFrameName(normal2);
		pCheckBox->setCheckedSpriteFrameName(select1);
		pCheckBox->setCheckedPressSpriteFrameName(select2);
		pCheckBox->setDisabledNormalSpriteFrameName(disable1);
		pCheckBox->setDisabledCheckedSpriteFrameName(disable2);
	}else{
		pCheckBox = CCheckBox::create(); 
		pCheckBox->setNormalImage(normal1); 
		pCheckBox->setNormalPressImage(normal2); 
		pCheckBox->setCheckedImage(select1); 
		pCheckBox->setCheckedPressImage(select2); 
		pCheckBox->setDisabledNormalImage(disable1); 
		pCheckBox->setDisabledCheckedImage(disable2); 
	}
	pCheckBox->setRotation(rotation);
	pCheckBox->setPosition(Point(x,-y));
	pCheckBox->setTag(tag);
	return pCheckBox;
}

ArmatureBtn* TuiManager::createArmatureBtn(float tag,const char* name,const char* png,const char* plist,const char* xml,float x,float y,float rotation){
	if(m_isUseSpriteFrame){
		ArmatureDataManager::getInstance()->addSpriteFrameFromFile(plist,png,xml);
	}else{
		ArmatureDataManager::getInstance()->addArmatureFileInfo(png,plist,xml);
	}
	ArmatureBtn *pArmBtn = ArmatureBtn::create(name);
	Size size = pArmBtn->getContentSize();
	pArmBtn->setRotation(rotation);
	pArmBtn->setPosition(Point(x - size.width/2,-y - size.height/2));
	pArmBtn->setTag(tag);
	return pArmBtn;
}

NumericStepper* TuiManager::createNumStep(float tag,const char* lnormal,const char* lselect,const char* ldisable,const char* rnormal,const char* rselect,const char* rdisable,const char* stepBg,float x,float y,float rotation){
	NumericStepper* pNumStep = NULL;
	if(m_isUseSpriteFrame){
		pNumStep = NumericStepper::create();
		pNumStep->setlNormalSpriteFrameName(lnormal);
		pNumStep->setlSelectedSpriteFrameName(lselect);
		pNumStep->setlDisabledSpriteFrameName(ldisable);
		pNumStep->setrNormalSpriteFrameName(rnormal);
		pNumStep->setrSelectedSpriteFrameName(rselect);
		pNumStep->setrDisabledSpriteFrameName(rdisable);
		pNumStep->setStepBgSpriteFrameName(stepBg);
	}else{
		pNumStep = NumericStepper::create(lnormal,lselect,ldisable,rnormal,rselect,rdisable,stepBg);
	}
	Size size = pNumStep->getContentSize();
	pNumStep->setRotation(rotation);
	pNumStep->setPosition(Point(x - size.width/2,-y - size.height/2));
	pNumStep->setTag(tag);
	return pNumStep;
}

ParticleSystemQuad* TuiManager::createParticle(float tag,const char* plist,float x,float y){
	ParticleSystemQuad *pPartical = ParticleSystemQuad::create(plist);
	pPartical->setPosition(x,-y);
	pPartical->setTag(tag);
	return pPartical;
}

CTableView* TuiManager::createTableView(float tag,float x,float y,float w,float h,float rotation){
	CTableView *pView = CTableView::create(Size(w,h));
	pView->setRotation(rotation);
	pView->setPosition(x,-y);
	pView->setTag(tag);
	return pView;
}

EditBox* TuiManager::createEditBox(float tag,const char* file,float x,float y,float w,float h,float rotation){
	EditBox *pEditBox = NULL;
	if(m_isUseSpriteFrame){
		pEditBox = EditBox::create(Size(w,h),Scale9Sprite::createWithSpriteFrameName(file));
	}else{
		pEditBox = EditBox::create(Size(w,h),Scale9Sprite::create(file));
	}
	pEditBox->setRotation(rotation);
	pEditBox->setPosition(Point(x,-y));
	pEditBox->setTag(tag);
	return pEditBox;
}

void TuiManager::loadXml(const string& path)
{
	if(m_DataMap.find(path) == m_DataMap.end())//简单地缓存下xml
	{
		string pathAbs = FileUtils::getInstance()->fullPathForFilename(path);
		m_DataMap[path] = FileUtils::getInstance()->getStringFromFile(pathAbs);
	}
}

/************************************************************************/
//	GET/SET/IS
/************************************************************************/
void TuiManager::setUseSpriteFrame(bool b){
	m_isUseSpriteFrame = b;
}
