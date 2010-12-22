#include "HiKernel/HiFramework.h"
#include "HiKernel/HiOSGRenderer.h"
#include "HiKernel/HiEventManager.h"
#include "HiModules/mHiGameWorld/mHiGameWorld.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiTelegram.h"
//#include "HiModules/MessageState.h"

using namespace HiKernel;
using namespace HiModules;


mHiGameWorld::mHiGameWorld(HiXmlNode *xml)
{
	if(xml != NULL)
		ParseXml(xml);	
	
	_origin.set(0.0f,0.0f,0.0f);
    _width.set(768.0f,0.0f,0.0f);
    _height.set(0.0f,0.0f,1024.0f);
    _widthBaseLine = _width*0.9f;
    _originBaseLine = _origin+_width*0.5-_widthBaseLine*0.5f;
    _characterSize = _width.length()*0.2f;
	
    _numberOfPlayers = 0;
    _level = 0;
	
    _chanceOfExplodingAtStart = 0.1f;
    _initialNumDropsPerSecond = 1.0f;
	
    _leftKeyPressed=false;
    _rightKeyPressed=false;
	
    _backgroundFiles.push_back("sky1.tga");
    _backgroundFiles.push_back("sky3.tga");
    _backgroundFiles.push_back("sky2.tga");
    _backgroundFiles.push_back("farm.tga");
	
	_currentScore = 0;
    
    setFOVY(osg::DegreesToRadians(60.0));
	
	_touched = false;
	
	
	//_px = 0;
	//_pxField = new HiVec3f(this, 0, "pt");//, &_px);
	//_pxField->Set(_px);
	_vx = 100;
	_vxfield = new HiVec3f(this, "pt");
}

mHiGameWorld::~mHiGameWorld()
{
}

bool mHiGameWorld::HandleMessage(const HiTelegram &msg)
{
	if(msg.Msg == GAMEGROUP_ADDCHILD_OBJECT)
	{
		osg::PositionAttitudeTransform* object_get = static_cast<osg::PositionAttitudeTransform*>(msg.ExtraInfo);
		
		_gameGroup->addChild(object_get);
	}
	
	else if(msg.Msg == GAMEGROUP_ADDCHILD_SWITCH)
	{
		osg::Switch* switch_get = static_cast<osg::Switch*>(msg.ExtraInfo);
		
		_gameGroup->addChild(switch_get);
	}
	else if(msg.Msg == SEND_MESSAGE_LOSTINDEX)
	{
		_currentIndex = _lostIndex;
		_gameSwitch->setSingleChildOn(_currentIndex);
	}
	else if(msg.Msg == SEND_MESSAGE_UPDATESCOREWITHCATCH)
	{
		updateScoreWithCatch();
	}
	else if(msg.Msg == SEND_MESSAGE_UPDATESCOREWITHLEVEL)
	{
		updateScoreWithLevel();
		nextLevel();
		if (gameComplete())
		{
			_currentIndex = _wonIndex;
			_gameSwitch->setSingleChildOn(_currentIndex);
		}
	}
	else if(msg.Msg == IPHONE_TOUCHED)
	{
		_touched = !_touched;
		CGPoint* _touchPoint = static_cast<CGPoint*>(msg.ExtraInfo);
		float px = (2.0f * (_touchPoint->x/768.0f)) * 0.5f;
		_vx = px;
		float vx[3] = {_vx, 0, 0};
		_vxfield->Set(vx);
		this->SendEvent(_vxfield);
	}
	
	return true;
}

void mHiGameWorld::PostConfig()
{
	//_numberOfPlayersField = new HiVec3f(this, 0, "numberofplayer");
	
	osg::ref_ptr<osg::Node> rootNode = createScene();
	
	rootNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	
	//	HiOSGRenderer::GetOSGViewer()->setSceneData(rootNode.get());
	OSGRenderer->getViewer()->setSceneData(rootNode.get());
	
#if defined (HI_WIN32)
	float fovy, aspectRatio, zNear, zFar;
#else
	double fovy, aspectRatio, zNear, zFar;
#endif
    OSGRenderer->getViewer()->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	setFOVY(osg::DegreesToRadians(fovy));
	
	//	OSGRenderer->getViewer()->setUpViewAcrossAllScreens();
	
    OSGRenderer->getViewer()->realize();
	
    // switch off the cursor
	
	//OSGRenderer->getViewer()->getCamera()->setClearColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 0.0f));
}
void mHiGameWorld::Update()
{
	OSGRenderer->getViewer()->getCamera()->setViewMatrix(getCameraPosition());
	
	if (_currentIndex==_welcomeIndex)
    {
		if(_touched)
		{
			_currentIndex = _gameIndex;
			_gameSwitch->setSingleChildOn(_currentIndex);
			resetGame();
			_touched = false;
		}
    }
    else if (_currentIndex==_lostIndex)
    {
        // lost screen
		if(_touched)
		{
			_currentIndex = _gameIndex;
			_gameSwitch->setSingleChildOn(_currentIndex);
			resetGame();
			_touched = false;
		}
    }
    else if (_currentIndex==_wonIndex)
    {
        // won screen
		if(_touched)
		{
			_currentIndex = _gameIndex;
			_gameSwitch->setSingleChildOn(_currentIndex);
			resetGame();
			_touched = false;
		}
    }
    else if (_currentIndex==_gameIndex)
    {
        // in game.
		//		if(GetAsyncKeyState(VK_LEFT))
		//		{
		//			_leftKeyPressed=true;
		//		}
		//		else if (GetAsyncKeyState(VK_RIGHT))
		//		{
		//			_rightKeyPressed=true;
		//		}
		
		// move characters
		if (_leftKeyPressed)
		{
			if (_numberOfPlayers>=2) 
				Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", SEND_MESSAGE_PLAYER1LEFT, 0);
			
		}
		
		if (_rightKeyPressed)
		{
			if (_numberOfPlayers>=2)
				Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", SEND_MESSAGE_PLAYER1RIGHT, 0);
		}
		
		
		
		_leftKeyPressed=false;
		
		_rightKeyPressed=false;
		_touched = false;
		
		NSDate*	eventstart = [NSDate date];
		static NSTimeInterval timer = [eventstart timeIntervalSinceReferenceDate];
		NSTimeInterval dt = [eventstart timeIntervalSinceReferenceDate] - timer;
		timer = [eventstart timeIntervalSinceReferenceDate];
		
		float numDropsPerSecond = _initialNumDropsPerSecond * (_level+1);
		float r = (float)rand()/(float)RAND_MAX;
		if (r < numDropsPerSecond*dt)
		{ 
			Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", SEND_MESSAGE_CREATECATCHABLE, 0);
		}
	}
	
	
}
void mHiGameWorld::Realize()
{
	
}

int mHiGameWorld::ParseXml(HiXmlNode *xml)
{
	
	return 0;
}


#ifndef HI_IOS4
extern "C" 
HI_DLLEXPORT mHiGameWorld* HiLoadModule_mHiGameWorld(HiXmlNode* xml)
{
	mHiGameWorld *m = new mHiGameWorld(xml);
	return m;
}
#endif

//--------------------------------------------------------------------

osg::Matrix mHiGameWorld::getCameraPosition()
{
    osg::Matrix cameraPosition;
    osg::Vec3 center = _origin+(_width+_height)*0.5f;
    
    float distance = _height.length()/(2.0f*tanf(_fovy*0.5f));
    
    cameraPosition.makeLookAt(center-osg::Vec3(0.0f,distance,0.0f),center,osg::Vec3(0.0f,0.0f,1.0f));
    return cameraPosition;
}


osg::Node* mHiGameWorld::createScene()
{
    _gameSwitch = new osg::Switch;
    
    // create a dummy catchable to load all the particule textures to reduce 
    // latency later on..
	/*
	 _dummyCatchable = new CatchableObject;
	 _dummyCatchable->setObject("Catch/a.png","a",osg::Vec3(0.0f,0.0,0.0f),1.0f,osg::Vec3(0.0f,0.0,0.0f));
	 _dummyCatchable->explode();
	 */
	
	
    // set up welcome subgraph
    {
        osg::Geode* geode = new osg::Geode;
		
        // set up the background
		osg::Image* image = osgDB::readImageFile("Welcome.tga");
        if (image)
        {
			osg::Geometry*	geometry = osg::createTexturedQuadGeometry(_origin,_width,_height);
			osg::StateSet* stateset = geometry->getOrCreateStateSet();
            stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
			/*
			 osg::StateSet*	stateset = geometry->getOrCreateStateSet();
			 osg::Texture2D*	_texture = new osg::Texture2D;
			 _texture->setDataVariance( osg::Object::STATIC );
			 _texture->setImage(image);
			 _texture->setUnRefImageDataAfterApply(false);
			 _texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
			 _texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			 stateset->setTextureAttributeAndModes(0,_texture,osg::StateAttribute::ON);
			 */
			
            geode->addDrawable(geometry);
        }
        /*
		 // set up the text
		 osg::Vec3 textPosition = _origin+_width*0.5f+_height*0.8f -osg::Vec3(0.0f,0.1f,0.0f);
		 {
		 osgText::Text* text = new osgText::Text;
		 text->setText("osgcatch is a childrens catching game\nMove your character using the mouse to\ncatch falling objects in your net\nbut avoid burning objects - they kill!!");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Move mouse left and right to move character\nCatch ten objects to advance to next level\nComplete four levels to win.");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Game concept and artwork - Caitlin Osfield, aged 5!\nSoftware development - Robert Osfield");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Press any key to start game.\nPress Escape to exit game at any time.");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 */
		
        _welcomeIndex = _gameSwitch->getNumChildren();
        _gameSwitch->addChild(geode);
    }
	
	
	
    // set up you've lost subgraph
    {
        osg::Geode* geode = new osg::Geode;
		
        osg::Image* image = osgDB::readImageFile("YouLost.tga");
        if (image)
        {
            osg::Geometry* geometry = osg::createTexturedQuadGeometry(_origin,_width,_height);
            osg::StateSet* stateset = geometry->getOrCreateStateSet();
            stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
			
            geode->addDrawable(geometry);
        }
        /*
		 // set up the text
		 osg::Vec3 textPosition = _origin+_width*0.5f+_height*0.75f -osg::Vec3(0.0f,0.1f,0.0f);
		 {
		 osgText::Text* text = new osgText::Text;
		 text->setText("Game Over\nYou lost all three lives");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.04f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Score: 0");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.04f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 text->setDataVariance(osg::Object::DYNAMIC);
		 
		 geode->addDrawable(text);
		 _scoreTextList.push_back(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Press any key to have another game.\nPress Escape to exit game.");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 */ 
        _lostIndex = _gameSwitch->getNumChildren();
        _gameSwitch->addChild(geode);
    }
	
    // set up you've won subgraph
    {
        osg::Geode* geode = new osg::Geode;
		
        osg::Image* image = osgDB::readImageFile("YouWon.tga");
        if (image)
        {
            osg::Geometry* geometry = osg::createTexturedQuadGeometry(_origin,_width,_height);
            osg::StateSet* stateset = geometry->getOrCreateStateSet();
            stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
			
            geode->addDrawable(geometry);
        }
        /*
		 // set up the text
		 osg::Vec3 textPosition = _origin+_width*0.5f+_height*0.75f -osg::Vec3(0.0f,0.1f,0.0f);
		 {
		 osgText::Text* text = new osgText::Text;
		 text->setText("Well done!!!\nYou completed all levels!");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.04f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Score: 0");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.04f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 _scoreTextList.push_back(text);
		 }
		 
		 {
		 textPosition -= _height*0.25f;
		 osgText::Text* text = new osgText::Text;
		 text->setText("Press any key to have another game.\nPress Escape to exit game.");
		 text->setFont("fonts/dirtydoz.ttf");
		 text->setPosition(textPosition);
		 text->setCharacterSize(_width.length()*0.025f);
		 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
		 text->setAlignment(osgText::Text::CENTER_CENTER);
		 text->setAxisAlignment(osgText::Text::XZ_PLANE);
		 
		 geode->addDrawable(text);
		 }
		 */
        _wonIndex = _gameSwitch->getNumChildren();
        _gameSwitch->addChild(geode);
    }
	
    // set up game subgraph.
    {
        _gameGroup = new osg::Group;
		
        if (_numberOfPlayers==0)
        {
			//            addPlayer(PLAYER_GIRL);
			_numberOfPlayers++;
			Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", ADD_CHARACTER, 0);
        }
		
        for(unsigned int i=0;i<_numberOfPlayers;++i)
        {
			/*
			 _gameGroup->addChild(_players[i]._character.get());
			 _gameGroup->addChild(_players[i]._livesSwitch.get());
			 _gameGroup->addChild(_players[i]._catchSwitch.get());
			 */
			Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", SEND_MESSAGE_GIVECHARACTER, &i);
			//			Dispatch->HiDispatchMessage(SEND_MSG_IMMEDIATELY, "dummy", "imageobject", SEND_MESSAGE_GIVELIVESWITCH, &i);
			//			Dispatch->HiDispatchMessage(SEND_MSG_IMMEDIATELY, "dummy", "imageobject", SEND_MESSAGE_GIVECATCHSWITCH, &i);
        }    
		
        // background
        {
            _levelSwitch = new osg::Switch;
			
            for(FileList::const_iterator itr = _backgroundFiles.begin();
                itr != _backgroundFiles.end();
                ++itr)
            {
				
                osg::Image* image = osgDB::readImageFile(*itr);
                if (image)
                {
                    osg::Geometry* geometry = osg::createTexturedQuadGeometry(_origin,_width,_height);
                    osg::StateSet* stateset = geometry->getOrCreateStateSet();
                    stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
					
                    osg::Geode* geode = new osg::Geode;
                    geode->addDrawable(geometry);
					
                    _levelSwitch->addChild(geode);
                }
            }
            _levelSwitch->setSingleChildOn(0);
            _gameGroup->addChild(_levelSwitch.get());
        }
		
		
        _gameIndex = _gameSwitch->getNumChildren();
        _gameSwitch->addChild(_gameGroup.get());
		
        // set up the text
        osg::Vec3 textPosition = _origin+_width*0.05f+_height*0.95f-osg::Vec3(0.0f,0.1f,0.0f);
        {
			/*
			 osgText::Text* text = new osgText::Text;
			 text->setText("Score : 0");
			 text->setFont("fonts/dirtydoz.ttf");
			 text->setPosition(textPosition);
			 text->setCharacterSize(_width.length()*0.04f);
			 text->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
			 text->setDataVariance(osg::Object::DYNAMIC);
			 text->setAxisAlignment(osgText::Text::XZ_PLANE);
			 
			 osg::Geode* geode = new osg::Geode;
			 geode->addDrawable(text);
			 _scoreTextList.push_back(text);
			 
			 textPosition -= _height*0.05f;
			 _levelText = new osgText::Text;
			 _levelText->setText("Level : 0");
			 _levelText->setFont("fonts/dirtydoz.ttf");
			 _levelText->setPosition(textPosition);
			 _levelText->setCharacterSize(_width.length()*0.04f);
			 _levelText->setColor(osg::Vec4(0.0f,0.2f,0.2f,1.0f));
			 _levelText->setDataVariance(osg::Object::DYNAMIC);
			 _levelText->setAxisAlignment(osgText::Text::XZ_PLANE);
			 
			 geode->addDrawable(_levelText.get());
			 
			 
			 _gameGroup->addChild(geode);
			 */
        }
		
		
    }
    
    _currentIndex = _welcomeIndex;
    _gameSwitch->setSingleChildOn(_currentIndex);
	
    return _gameSwitch.get();
}

void mHiGameWorld::addPlayer()
{
	Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", ADD_CHARACTER, 0);
	
	_numberOfPlayers++;
}

void mHiGameWorld::clearCatchables()
{
	Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "gameworld", "imageobject", SEND_MESSAGE_CLEARCATCHABLE, 0);
}