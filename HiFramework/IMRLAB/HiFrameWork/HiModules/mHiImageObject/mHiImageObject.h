#ifndef _IMAGE_OBJECT_HEADER_
#define _IMAGE_OBJECT_HEADER_

#include "HiKernel/HiModule.h"
#include "HiKernel/HiField.h"
#include <iostream>
#include <sstream>

#include "HiModules/mHiImageObject/CatchableObject.h"
#include "HiModules/mHiImageObject/Character.h"

typedef std::list<osg::ref_ptr<CatchableObject> > CatchableObjectList;

class mHiImageObject : public HiKernel::HiModule
{
protected :
	Character*			_players[2];
	CatchableObjectList _catchableObjects;

	FileList	_benignCatachables;

	//postconfig½Ã Á¤ÀÇ
	osg::Vec3			_origin;
	osg::Vec3			_width;
	osg::Vec3			_height;
	osg::Vec3			_originBaseLine;
	osg::Vec3			_widthBaseLine;
	bool				_take_message;

	float				_chanceOfExplodingAtStart;
	
	unsigned int		_numberOfPlayers;
	
	HiKernel::HiVec3f*			_vxfield;
	float				_vx;

public :
	mHiImageObject(HiKernel::HiXmlNode* xml = NULL);
	virtual ~mHiImageObject ();

public :

	void PostConfig();
	void Update();
	void Realize();
	bool HandleMessage(const HiKernel::HiTelegram &msg);
	int  ParseXml(HiKernel::HiXmlNode *xml);

public :
	void addPlayer(Players player);

	void createNewCatchable();
	void clearCatchables();

};

#endif