#include "HiKernel/HiFramework.h"
#include "HiModules/mHiImageObject/mHiImageObject.h"
#include "HiKernel/HiEventManager.h"
#include "HiModules/enumerations.h"
#include "HiKernel/HiTelegram.h"

using namespace HiKernel;
using namespace HiModules;

#include <vector>

mHiImageObject::mHiImageObject(HiXmlNode *xml)
{
	_numberOfPlayers = 0;
	_players[0] = NULL;
	_players[1] = NULL;

	_origin.set(0.0f,0.0f,0.0f);
    _width.set(768.0f,0.0f,0.0f);
    _height.set(0.0f,0.0f,1024.0f);
    _widthBaseLine = _width*0.9f;
    _originBaseLine = _origin+_width*0.5-_widthBaseLine*0.5f;

	_chanceOfExplodingAtStart = 0.1f;


	_benignCatachables.push_back("aa.tga");
//    _benignCatachables.push_back("b.png");
//    _benignCatachables.push_back("c.png");
//    _benignCatachables.push_back("m.png");
//    _benignCatachables.push_back("n.png");
//    _benignCatachables.push_back("s.png");
//    _benignCatachables.push_back("t.png");
//    _benignCatachables.push_back("u.png");
//    _benignCatachables.push_back("ball.png");

	CatchableObject::setUpCatchablesMap(_benignCatachables);

	_vx = 100;
	_vxfield = new HiVec3f(this, "pt");
	

	if(xml != NULL)
		ParseXml(xml);	
}
mHiImageObject::~mHiImageObject()
{
}

bool mHiImageObject::HandleMessage(const HiTelegram &msg)
{

	if(msg.Msg == ADD_CHARACTER)
	{
		addPlayer(PLAYER_GIRL);
	}
	else if(msg.Msg == SEND_MESSAGE_GIVECHARACTER)
	{
		osg::PositionAttitudeTransform* object_get = _players[0]->_character.get();
		Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", GAMEGROUP_ADDCHILD_OBJECT, object_get);
	}
	else if(msg.Msg == SEND_MESSAGE_GIVELIVESWITCH)
	{
		osg::Switch* switch_get = _players[0]->_livesSwitch.get();
		Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", GAMEGROUP_ADDCHILD_SWITCH, switch_get);
	}
	else if(msg.Msg == SEND_MESSAGE_GIVECATCHSWITCH)
	{
		osg::Switch* switch_get = _players[0]->_catchSwitch.get();
		Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", GAMEGROUP_ADDCHILD_SWITCH, switch_get);
	}
	else if(msg.Msg == SEND_MESSAGE_PLAYER1LEFT)
	{
		_players[0]->moveLeft();
	}
	else if(msg.Msg == SEND_MESSAGE_PLAYER1RIGHT)
	{
		_players[0]->moveRight();
	}
	else if(msg.Msg == SEND_MESSAGE_CREATECATCHABLE)
	{
		createNewCatchable();
	}
	else if(msg.Msg == SEND_MESSAGE_CLEARCATCHABLE)
	{	
		clearCatchables();
	}
	else if(msg.Msg == IPHONE_TOUCHED)
	{

	}


	return true;
}

void mHiImageObject::PostConfig()
{


}
void mHiImageObject::Update()
{
	bool _iscleared = false;
	NSDate*	eventstart = [NSDate date];
	static NSTimeInterval timer = [eventstart timeIntervalSinceReferenceDate];
	NSTimeInterval dt = [eventstart timeIntervalSinceReferenceDate] - timer;
	timer = [eventstart timeIntervalSinceReferenceDate];
//	CatchableObjectList::iterator removeObject;
	// move objects
	for(CatchableObjectList::iterator itr=_catchableObjects.begin();
		itr!=_catchableObjects.end();
		)
	{
		(*itr)->update(1/dt);

		
		bool removeEntry = false;

		for(unsigned int i=0;i<_numberOfPlayers;++i)
		{
			bool inBasket = ((*itr)->centerInside(_players[i]->getCurrentCenterOfBasket(),_players[i]->getCurrentRadiusOfBasket()));
				
			if ((*itr)->dangerous())
			{
				if ((*itr)->anyInside(_players[i]->getLowerLeft(),_players[i]->getUpperRight()) || inBasket)
				{
					// player has hit or caught a dangerous object, must loose a life.
					if (!_players[i]->looseLife())
					{
						Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", SEND_MESSAGE_LOSTINDEX, 0);

					}
					else
					{
						clearCatchables();
						_iscleared = true;
					}
				}
			}
			else if (inBasket)
			{
				// player has caught a safe object.
				Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", SEND_MESSAGE_UPDATESCOREWITHCATCH, 0);  

				if (!_players[i]->addCatch())
				{
					_players[i]->resetCatches();
					
					Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", SEND_MESSAGE_UPDATESCOREWITHLEVEL, 0);
				}

				removeEntry = true;
			}
		}
		
		if(_iscleared)
			break;

		if (!(*itr)->anyInside(_origin, _origin+_width+_height) || 
			(*itr)->needToRemove([eventstart timeIntervalSinceReferenceDate]) ||
			removeEntry)
		{
			// need to remove
			// remove child from parents.
			osg::ref_ptr<osg::PositionAttitudeTransform> child = (*itr)->_object;
			osg::Node::ParentList parents = child->getParents();
			for(osg::Node::ParentList::iterator pitr=parents.begin();
				pitr!=parents.end();
				++pitr)
			{
				(*pitr)->removeChild(child.get());
			}

			// remove child from catchable list
			_catchableObjects.erase(itr++);

		}
		else {
			itr++;
		}
//		else if ((*itr)->anyInside(_origin, _origin+_width) && !(*itr)->stopped())
//		{
//			// hit base line
//			(*itr)->explode();
//			(*itr)->stop();
//			(*itr)->setTimeToRemove(timeGetTime()+3.0);
//		}

	}

	_vx = *_vxfield->Get();
	if (_numberOfPlayers>=1) _players[0]->moveTo(_vx);

}
void mHiImageObject::Realize()
{

}

int mHiImageObject::ParseXml(HiXmlNode *xml)
{

	return 0;
}

#ifndef HI_IOS4
extern "C" 
HI_DLLEXPORT mHiImageObject* HiLoadModule_mHiImageObject(HiXmlNode* xml)
{
	mHiImageObject *m = new mHiImageObject(xml);
	return m;
}
#endif

void mHiImageObject::addPlayer(Players player)
{
	osg::Vec3 livesPosition;
	osg::Vec3 catchesPosition;
	if (_numberOfPlayers==0)
	{
		livesPosition = _originBaseLine+osg::Vec3(0.0f,-0.5f,0.0f);
		catchesPosition = _originBaseLine+osg::Vec3(100.0f,-0.5f,0.0f);
	}
	else
	{
		livesPosition = _originBaseLine+osg::Vec3(1000.0f,-0.5f,000.0f);
		catchesPosition = _originBaseLine+osg::Vec3(1100.0f,-0.5f,0.0f);
	}
        
	switch(player)
	{
	case PLAYER_GIRL:
		{
			std::string player_one = "girl.tga"; 
			osg::Vec3 catchPos(0.2, 0.57, 0.34);

			if(_players[_numberOfPlayers] == NULL)
				_players[_numberOfPlayers] = new Character();

			_players[_numberOfPlayers]->setCharacter(player_one,"girl", _originBaseLine + osg::Vec3(0.0f,-1.0f,0.0f), _widthBaseLine, catchPos, 0.5f);
			_players[_numberOfPlayers]->setLives(player_one,livesPosition, osg::Vec3(0.0f,0.0f,100.0f),3);
			_players[_numberOfPlayers]->setCatches("broach.tga",catchesPosition, osg::Vec3(0.0f,0.0f,100.0f),10);

			++_numberOfPlayers;
		
			break;
		}
	case PLAYER_BOY:
		{
			std::string player_two = "boy.tga"; 
			osg::Vec3 catchPos(0.8, 0.57, 0.34);

			if(_players[_numberOfPlayers] == NULL)
				_players[_numberOfPlayers] = new Character();

			_players[_numberOfPlayers]->setCharacter(player_two,"boy", _originBaseLine + osg::Vec3(0.0f,-2.0f,0.0f), _widthBaseLine, catchPos, 0.5f);
			_players[_numberOfPlayers]->setLives(player_two,livesPosition, osg::Vec3(0.0f,0.0f,100.0f),3);
			_players[_numberOfPlayers]->setCatches("broach.tga",catchesPosition, osg::Vec3(0.0f,0.0f,100.0f),10);

			++_numberOfPlayers;
			break;
		}
	}
}

void mHiImageObject::createNewCatchable()
{
	if (_benignCatachables.empty()) return;

    unsigned int catachableIndex = (unsigned int)((float)_benignCatachables.size()*(float)rand()/(float)RAND_MAX);
    if (catachableIndex>=_benignCatachables.size()) catachableIndex = _benignCatachables.size()-1;
    
    const std::string& filename = _benignCatachables[catachableIndex];

    float ratio = ((float)rand() / (float)RAND_MAX);
    float size = 20.0f+100.0f*((float)rand() / (float)RAND_MAX);
    float angle = osg::PI*0.25f + 0.5f*osg::PI*((float)rand() / (float)RAND_MAX);
    float speed = 200.0f*((float)rand() / (float)RAND_MAX);

    CatchableObject* catchableObject = new CatchableObject;
    osg::Vec3 position = _origin+_height+_width*ratio + osg::Vec3(0.0f,-0.7f,0.0f);
    osg::Vec3 velocity(-cosf(angle)*speed,0.0f,-sinf(angle)*speed);
    //std::cout<<"angle = "<<angle<<" velocity="<<velocity<<std::endl;
    catchableObject->setObject(filename,"boy",position,size,velocity);
    _catchableObjects.push_back(catchableObject);

    float r = (float)rand() / (float)RAND_MAX;
    if (r < _chanceOfExplodingAtStart)
    {
       catchableObject->explode(); 
    }
	osg::PositionAttitudeTransform* object_get = catchableObject->_object.get();

	Dispatch->GiveMessage(SEND_MSG_IMMEDIATELY, "imageobject", "gameworld", GAMEGROUP_ADDCHILD_OBJECT, object_get);

//	_gameGroup->addChild();


}
void mHiImageObject::clearCatchables()
{
	 for(CatchableObjectList::iterator itr=_catchableObjects.begin();
		 itr!=_catchableObjects.end();
		 ++itr)
	{
		// need to remove
		// remove child from parents.
		osg::ref_ptr<osg::PositionAttitudeTransform> child = (*itr)->_object;
		osg::Node::ParentList parents = child->getParents();
		for(osg::Node::ParentList::iterator pitr=parents.begin();
			pitr!=parents.end();
			++pitr)
		{
			(*pitr)->removeChild(child.get());
		}
	}

	_catchableObjects.clear();
}