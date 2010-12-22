#include "HiModules/mHiImageObject/CatchableObject.h"

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>


CatchableObject::CatchableObject()
{
    _stopped = false;
    _dangerous = false;
    
    _timeToRemove = -1.0; // do not remove.
    setFluidToAir();
}

void CatchableObject::setUpCatchablesMap(const FileList& fileList)
{
    for(FileList::const_iterator itr=fileList.begin();
        itr!=fileList.end();
        ++itr)
    {
        const std::string& filename = *itr;
        osg::Image* image = osgDB::readImageFile(filename);
        if (image)
        {
            osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
            stateset->setTextureAttributeAndModes(0,new osg::Texture2D(image),osg::StateAttribute::ON);
            stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
            stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
            
            osg::Vec3 width((float)(image->s())/(float)(image->t()),0.0f,0.0);
            osg::Vec3 height(0.0f,0.0f,1.0f);
            osg::Vec3 pos = (width+height)*-0.5f;

            osg::Geometry* geometry = osg::createTexturedQuadGeometry(pos,width,height);
            geometry->setStateSet(stateset.get());

            osg::Geode* geode = new osg::Geode;
            geode->addDrawable(geometry);

            s_objectMap[filename] = geode;
        }
    }
}

void CatchableObject::setObject(const std::string& filename, const std::string& name, const osg::Vec3& center, float characterSize, const osg::Vec3& velocity)
{
    _radius = 0.5f*characterSize;
    float Area = osg::PI*_radius*_radius;
    float Volume = Area*_radius*4.0f/3.0f;

    _velocity = velocity;
    _mass = 1000.0*Volume;

    if (s_objectMap.count(filename)!=0)
    {
        osg::PositionAttitudeTransform* scaleTransform = new osg::PositionAttitudeTransform;
        scaleTransform->setScale(osg::Vec3(characterSize,characterSize,characterSize));
        scaleTransform->addChild(s_objectMap[filename].get());

        _object = new osg::PositionAttitudeTransform;
        _object->setName(name);
        _object->setPosition(center);
        _object->addChild(scaleTransform);
    }
    else
    {
        osg::notify(osg::NOTICE)<<"CatchableObject::setObject("<<filename<<") not able to create catchable object."<<std::endl;
    }
}

void CatchableObject::update(double dt)
{
    if (_stopped) return;

    float Area = osg::PI*_radius*_radius;
    float Volume = Area*_radius*4.0f/3.0f;

    // compute force due to gravity + boyancy of displacing the fluid that the particle is emersed in.
    osg::Vec3 force = _acceleration * (_mass - _density*Volume);

    // compute force due to friction
    osg::Vec3 relative_wind = _velocity-_wind;            
    force -= relative_wind * Area * (_viscosityCoefficient + _densityCoefficeint*relative_wind.length());            

    // divide force by mass to get acceleration.
    _velocity += (force/dt)/_mass;
	_object->setPosition(_object->getPosition()+_velocity/dt);
}

bool CatchableObject::anyInside(const osg::Vec3& lower_left, const osg::Vec3& upper_right)
{
    osg::Vec3 pos = _object->getPosition();
    
    if (pos.x()+_radius < lower_left.x()) return false;
    if (pos.x()-_radius > upper_right.x()) return false;
    if (pos.z()+_radius < lower_left.z()) return false;
    if (pos.z()-_radius > upper_right.z()) return false;

    return true;
}

bool CatchableObject::centerInside(const osg::Vec3& center, float radius)
{
    osg::Vec3 delta = _object->getPosition() - center;
    return (delta.length()<radius);
}


void CatchableObject::explode()
{
//    osg::Vec3 position(0.0f,0.0f,0.0f);
//    osgParticle::ExplosionEffect* explosion = new osgParticle::ExplosionEffect(position, _radius);
//    osgParticle::ExplosionDebrisEffect* explosionDebri = new osgParticle::ExplosionDebrisEffect(position, _radius);
//    osgParticle::SmokeEffect* smoke = new osgParticle::SmokeEffect(position, _radius);
//    osgParticle::FireEffect* fire = new osgParticle::FireEffect(position, _radius);
//
//    explosion->setWind(_wind);
//    explosionDebri->setWind(_wind);
//    smoke->setWind(_wind);
//    fire->setWind(_wind);
//
//    _object->addChild(explosion);
//    _object->addChild(explosionDebri);
//    _object->addChild(smoke);
//    _object->addChild(fire);

    _dangerous = true;

}