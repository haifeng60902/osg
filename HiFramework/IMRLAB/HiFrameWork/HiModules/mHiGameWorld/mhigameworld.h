#ifndef _GAME_WORLD_HEADER_
#define _GAME_WORLD_HEADER_

#include "HiKernel/HiModule.h"
#include "HiKernel/HiField.h"
#include <iostream>
#include <sstream>
#include <osg/Switch>
#include <osg/Timer>
typedef std::vector<std::string>	FileList;

class mHiGameWorld : public HiKernel::HiModule
{
public:
	osg::Matrix getCameraPosition();

	osg::Node* createScene();

	void setFOVY(float fovy) { _fovy = fovy; }
	float getFOVY() const { return _fovy; }

	void clearCatchables();

	void resetLevel()
	{
		_level = 0;
		_levelSwitch->setSingleChildOn(_level);
		clearCatchables();

		updateLevelText();

		_levelStartTick = osg::Timer::instance()->tick();
	}

	void nextLevel()
	{
		++_level;
		if (_level < _levelSwitch->getNumChildren())
		{
			_levelSwitch->setSingleChildOn(_level);
			clearCatchables();
		}

		updateLevelText();

		_levelStartTick = osg::Timer::instance()->tick();
	}

	bool gameComplete()
	{
		return _level >= _levelSwitch->getNumChildren();
	}

	void resetGame()
	{
		_currentScore = 0;

		updateTextWithScore();

		clearCatchables();
		resetLevel();

		for(unsigned int i=0;i<_numberOfPlayers;++i)
		{
			//_players[i].reset();
		}
	}

	void updateScoreWithCatch()
	{
		_currentScore += 1;
		updateTextWithScore();
	}

	void updateScoreWithLevel()
	{
		osg::Timer_t newTick = osg::Timer::instance()->tick();
		double timeForLevel = osg::Timer::instance()->delta_s(_levelStartTick, newTick);

		// a ten second level gets you 10 points, 
		// a twenty second levels gets you 5 points.        
		_currentScore += static_cast<unsigned int>(10000.0f/(timeForLevel*timeForLevel));

		updateTextWithScore();

	}

	void updateTextWithScore()
	{
		std::ostringstream os;
		os<<"Score: "<<_currentScore;

		std::string textString = os.str();

		/*
		for(TextList::iterator itr = _scoreTextList.begin();
			itr != _scoreTextList.end();
			++itr)
		{
			(*itr)->setText(textString);
		}*/
	}        

	void updateLevelText()
	{
		std::ostringstream os;
		os<<"Level: "<<_level+1;
//		_levelText->setText(os.str());
	}

	void addPlayer();	//캐릭터의 함수 호출


protected :
//	typedef std::vector<osg::ref_ptr<osgText::Text> > TextList;

	float		_characterSize;
	float		_fovy;
	unsigned	_level;
	float		_initialNumDropsPerSecond;
	osg::ref_ptr<osg::Switch>			_gameSwitch;
	osg::ref_ptr<osg::Group>			_gameGroup;
	osg::ref_ptr<osg::Switch>			_levelSwitch;
	
	unsigned int						_currentIndex;
	unsigned int						_welcomeIndex;
	unsigned int						_lostIndex;
	unsigned int						_wonIndex;
	unsigned int						_gameIndex;
	osg::Timer_t						_levelStartTick;
	unsigned int						_currentScore;
//	osg::ref_ptr<osgText::Text>			_levelText;
//	TextList							_scoreTextList;
	FileList							_backgroundFiles;
	bool								_leftKeyPressed;
	bool								_rightKeyPressed;
	bool								_touched;


	//postconfig시 정의
	osg::Vec3			_origin;
	osg::Vec3			_width;
	osg::Vec3			_height;
	osg::Vec3			_originBaseLine;
	osg::Vec3			_widthBaseLine;

	float				_chanceOfExplodingAtStart;
	
	unsigned int		_numberOfPlayers;
	
	HiKernel::HiVec3f*			_vxfield;
	float				_vx;

public :
	mHiGameWorld(HiKernel::HiXmlNode* xml = NULL);
	virtual ~mHiGameWorld ();

public :

	void PostConfig(); 
	void Update();
	void Realize();
	bool HandleMessage(const HiKernel::HiTelegram &msg);
	int  ParseXml(HiKernel::HiXmlNode *xml);

public :

};

#endif