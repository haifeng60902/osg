#include "HiModules/mHiImageObject/Character.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osg/Texture2D>
#include <osg/Switch>


Character::Character():
    _positionRatio(0.5f),
    _numLives(3),
    _numCatches(0)
{
}


void Character::setCharacter(const std::string& filename, const std::string& name, const osg::Vec3& origin, const osg::Vec3& width, const osg::Vec3& catchPos, float positionRatio)
{
    _origin = origin;
    _width = width;
    _positionRatio = positionRatio;
    _numLives = 3;
    _numCatches = 0;

    float _characterSize = _width.length()*0.2f;

    osg::Image* image = osgDB::readImageFile(filename);
    if (image)
    {
        osg::Vec3 pos(-0.5f*_characterSize,0.0f,0.0f);
        osg::Vec3 width(_characterSize*((float)image->s())/(float)(image->t()),0.0f,0.0);
        osg::Vec3 height(0.0f,0.0f,_characterSize);

        osg::Geometry* geometry = osg::createTexturedQuadGeometry(pos,width,height);
        osg::StateSet* stateset = geometry->getOrCreateStateSet();
        stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(geometry);

        _character = new osg::PositionAttitudeTransform;
        _character->setName(name);
        _character->addChild(geode);
        
        moveTo(positionRatio);

        _centerBasket = width*catchPos.x() + height*catchPos.y() + pos;
        _radiusBasket = width.length()*catchPos.z();

    }
    
}

void Character::setLives(const std::string& filename, const osg::Vec3& origin, const osg::Vec3& delta, unsigned int numLives)
{
    float characterSize = delta.length();

    _numLives = numLives;
    _livesSwitch = new osg::Switch;

    osg::Image* image = osgDB::readImageFile(filename);
    if (image)
    {
        osg::StateSet* stateset = _livesSwitch->getOrCreateStateSet();
        stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

        for(unsigned int i=0; i<numLives; ++i)
        {
            osg::Vec3 pos = origin + delta*(float)i + osg::Vec3(0.0f,0.0f,0.0f);
            osg::Vec3 width(characterSize*((float)image->s())/(float)(image->t()),0.0f,0.0);
            osg::Vec3 height(0.0f,0.0f,characterSize);

            osg::Geometry* geometry = osg::createTexturedQuadGeometry(pos,width,height);

            osg::Geode* geode = new osg::Geode;
            geode->addDrawable(geometry);

            _livesSwitch->addChild(geode,true);

        }
    }

}

void Character::setCatches(const std::string& filename, const osg::Vec3& origin, const osg::Vec3& delta, unsigned int numCatches)
{
    float characterSize = delta.length();

    _numCatches = 0;
    _catchSwitch = new osg::Switch;

    osg::Image* image = osgDB::readImageFile(filename);
    if (image)
    {
        osg::StateSet* stateset = _catchSwitch->getOrCreateStateSet();
        stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
        stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
        stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

        for(unsigned int i=0; i<numCatches; ++i)
        {
            osg::Vec3 pos = origin + delta*(float)i + osg::Vec3(0.0f,0.0f,0.0f);
            osg::Vec3 width(characterSize,0.0f,0.0);
            osg::Vec3 height(0.0f,0.0f,characterSize*((float)image->t())/(float)(image->s()));

            osg::Geometry* geometry = osg::createTexturedQuadGeometry(pos,width,height);

            osg::Geode* geode = new osg::Geode;
            geode->addDrawable(geometry);

            _catchSwitch->addChild(geode,false);

        }
    }

}

void Character::moveLeft()
{
    moveTo(_positionRatio - 0.01f);
}

void Character::moveRight()
{
    moveTo(_positionRatio + 0.01f);
}

void Character::moveTo(float positionRatio)
{
    if (positionRatio<0.0f) positionRatio = 0.0f;
    if (positionRatio>1.0f) positionRatio = 1.0f;

    _positionRatio = positionRatio;
	_character->setPosition(_origin+_width*+positionRatio);
}

void Character::reset()
{
    _numCatches = 0;
    _numLives = _livesSwitch->getNumChildren();

    _livesSwitch->setAllChildrenOn();
    _catchSwitch->setAllChildrenOff();
}

void Character::resetCatches()
{
    _numCatches = 0;
    _catchSwitch->setAllChildrenOff();
}

bool Character::addCatch()
{
    if (!_catchSwitch || _numCatches>=_catchSwitch->getNumChildren()) return false;
    
    _catchSwitch->setValue(_numCatches,true);
    ++_numCatches;
    
    return true;
}

bool Character::looseLife()
{
    if (!_livesSwitch || _numLives==0) return false;
    
    --_numLives;
    _livesSwitch->setValue(_numLives,false);
    
    return (_numLives!=0);
}
