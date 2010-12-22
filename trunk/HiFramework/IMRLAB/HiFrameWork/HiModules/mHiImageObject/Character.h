#ifndef	__IMAGEOBJECT_CHARACTER_H__
#define __IMAGEOBJECT_CHARACTER_H__


#include <iostream>
#include <sstream>

#include <osg/Geode>

#include <osg/PositionAttitudeTransform>

//게임 이벤트에서 가져온것
enum Players
{
	PLAYER_GIRL,
	PLAYER_BOY
};

class Character : public osg::Referenced
{
public:
	Character();

	void setCharacter(const std::string& filename, const std::string& name, const osg::Vec3& orgin, const osg::Vec3& width, const osg::Vec3& catchPos, float positionRatio);

	void setLives(const std::string& filename, const osg::Vec3& orgin, const osg::Vec3& delta, unsigned int numLives);

	void setCatches(const std::string& filename, const osg::Vec3& orgin, const osg::Vec3& delta, unsigned int numLives);

	void moveLeft();
	void moveRight();
	void moveTo(float positionRatio);

	void reset();

	void resetCatches();

	bool addCatch();

	bool looseLife();

	osg::Vec3 getCurrentCenterOfBasket() const { return _character->getPosition()+_centerBasket; }
	float getCurrentRadiusOfBasket() const { return _radiusBasket; }

	osg::Vec3 getLowerLeft() const { return _character->getPosition(); }
	osg::Vec3 getUpperRight() const { return _character->getPosition(); }

	bool			_updated;
	osg::Vec3		_origin;
	osg::Vec3		_width;
	float			_positionRatio;
	osg::ref_ptr<osg::PositionAttitudeTransform>	_character;
	unsigned int	_numLives;
	osg::ref_ptr<osg::Switch>	_livesSwitch;
	unsigned int	_numCatches;
	osg::ref_ptr<osg::Switch>	_catchSwitch;
	osg::Vec3		_centerBasket;
	float			_radiusBasket;

};


#endif