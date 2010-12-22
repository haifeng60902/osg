#ifndef _IMAGEOBJECT_CATCHABLEOBJECT_H_
#define _IMAGEOBJECT_CATCHABLEOBJECT_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include <osg/Geode>
#include <osg/PositionAttitudeTransform>

#include <osgUtil/Optimizer>
#include <osgUtil/GLObjectsVisitor>

typedef std::vector<std::string>	FileList;
typedef std::map<std::string, osg::ref_ptr<osg::Node> >	ObjectMap;

static ObjectMap	s_objectMap;

class CatchableObject : public osg::Referenced
{
public:
	CatchableObject();

	void setObject(const std::string& filename, const std::string& name, const osg::Vec3& center, float size, const osg::Vec3& direction);

	bool anyInside(const osg::Vec3& lower_left, const osg::Vec3& top_right);

	bool centerInside(const osg::Vec3& center, float radius);

	void explode();

	bool dangerous() { return _dangerous; }

	void stop() { _stopped = true; }

	bool stopped() { return _stopped; }

	void setTimeToRemove(double time) { _timeToRemove=time; }

	double getTimeToRemove() { return _timeToRemove; }

	bool needToRemove(double time) { return  _timeToRemove>=0.0 && time>_timeToRemove; }

	static void setUpCatchablesMap(const FileList& fileList);

	//게임 핸들러에서 가져온 것
	void compleGLObjects(osg::State& state)
	{
		osgUtil::GLObjectsVisitor compile;
		compile.setState(&state);

		for(ObjectMap::iterator itr = s_objectMap.begin();
			itr != s_objectMap.end();
			++itr)
		{
			itr->second->accept(compile);
		}
	}
	//---------------------------


	// update position and velocity
	void update(double dt);

	/// Set the viscosity of the fluid.
	inline void setFluidViscosity(float v)
	{
		_viscosity = v;
		_viscosityCoefficient = 6 * osg::PI * _viscosity;
	}

	/// Get the viscosity of the fluid.
	inline float getFluidViscosity() const { return _viscosity; }

	/// Set the density of the fluid.
	inline void setFluidDensity(float d)
	{
		_density = d;
		_densityCoefficeint = 0.2f * osg::PI * _density;
	}

	/// Get the density of the fluid.
	inline float getFluidDensity() const { return _density; }


	/// Set the wind vector.
	inline void setWind(const osg::Vec3& wind) { _wind = wind; }

	/// Get the wind vector.
	inline const osg::Vec3& getWind() const { return _wind; }

	/// Set the acceleration vector.
	inline void setAcceleration(const osg::Vec3& v) { _acceleration = v; }

	/// Get the acceleration vector.
	inline const osg::Vec3& getAcceleration() const { return _acceleration; }

	/** Set the acceleration vector to the gravity on earth (0, 0, -9.81).
	The acceleration will be multiplied by the <CODE>scale</CODE> parameter.
	*/
	inline void setToGravity(float scale = 1.0f) { _acceleration.set(0, 0, -9.81f*scale); }

	/// Set the fluid parameters as for air (20캜 temperature).
	inline void setFluidToAir()
	{
		setToGravity(1.0f);
		setFluidDensity(1.2929f);
		setFluidViscosity(1.8e-5f);
	}

	/// Set the fluid parameters as for pure water (20캜 temperature).
	inline void setFluidToWater()
	{
		setToGravity(1.0f);
		setFluidDensity(1.0f);
		setFluidViscosity(1.002e-3f);
	}

	bool		updated;
	osg::ref_ptr<osg::PositionAttitudeTransform> _object;
	osg::Vec3	_velocity;
	float		_mass;
	float		_radius;
	bool		_stopped;
	bool		_dangerous;
	double		_timeToRemove;

protected:

	osg::Vec3	_acceleration;
	float		_viscosity;
	float		_density;
	osg::Vec3	_wind;

	float		_viscosityCoefficient;
	float		_densityCoefficeint;


};

#endif