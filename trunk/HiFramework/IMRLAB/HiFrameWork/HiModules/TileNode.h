#ifndef _TILENODE_HEADER_
#define _TILENODE_HEADER_

#include <osg/Geode>
#include <osg/Geometry>

#include <osg/PositionAttitudeTransform>

typedef enum {
	NONE = 0,
	UP,
	DOWN,
	LEFT,
	RIGHT
}Movement;

class TileNode : public osg::Geode
{
private:
	Movement movement_;
	bool	 is_actived_;
	bool	 is_moved_;

	double	interval_, active_time_;

	float	tx_, tz_;
	int		id_;

	float width_, height_;


	osg::Geometry* tileGeometry_;
	osg::PositionAttitudeTransform* tileTransform_;
public:
	osg::PositionAttitudeTransform* Init(int _id, int row, int col);
	osg::PositionAttitudeTransform* Init(osg::Vec3 pos);
	osg::Vec3	GetPosition();
	bool		InActivate();
	void		Activate(bool active, bool move);
	void		SetGeometry();						
	void		Update();
	void		SetPosition(osg::Vec3 pos);
	void		SetMovement(Movement move);
	void		Drawable(bool YnN);

	void		SetID(int _id) {id_ = _id;}
	void		SetWidth(float width) {width_ = width;}
	void		SetHeight(float height) {height_ = height;}
	float		GetWidth() {return width_;}
	float		GetHeight() {return height_;}
	int			GetID() {return id_;}
	float		GetX() {return tx_;}
	float		GetZ() {return tz_;}
	bool		IsMoved() {return is_moved_;}
};



#endif