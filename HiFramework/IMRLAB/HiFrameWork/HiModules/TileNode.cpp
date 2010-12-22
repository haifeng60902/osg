//#include "HiModules/TileNode.h"
//
//#include <QuartzCore/QuartzCore.h>
//
//osg::PositionAttitudeTransform* TileNode::Init(int _id, int row, int col)
//{
//	id_ = _id;
//	is_actived_ = false;
//	is_moved_ = false;
//	interval_ = 0;
//	active_time_ = 0;
//	tx_ = -24; tz_ = 24;
//	width_ = 48.0/row; 
//	height_ = 48.0/col;
//	while(_id >= row)
//	{
//		_id -= row;
//		tz_-=height_ + 0.5;
//	}
//	tx_ += _id*(width_ + 0.5);
//
//	tileGeometry_ = new osg::Geometry();
//	tileTransform_ = 
//	new osg::PositionAttitudeTransform(); 
//
//	this->addDrawable(tileGeometry_); 
//
//	osg::Vec3Array* vertices = new osg::Vec3Array; 
//	vertices->push_back( osg::Vec3(-(width_/2), 0,-(height_/2)) ); // front left 
//	vertices->push_back( osg::Vec3( (width_/2), 0,-(height_/2)) ); // front right 
//	vertices->push_back( osg::Vec3( (width_/2), 0, (height_/2)) ); // back right 
//	vertices->push_back( osg::Vec3(-(width_/2), 0, (height_/2)) ); // back left 
//
//	tileGeometry_->setVertexArray( vertices ); 
//
//	osg::DrawElementsUInt* Base = 
//	new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0); 
//	Base->push_back(3); 
//	Base->push_back(2); 
//	Base->push_back(1); 
//	Base->push_back(0); 
//	tileGeometry_->addPrimitiveSet(Base); 
//
//	osg::Vec4Array* colors = new osg::Vec4Array; 
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	
//	osg::TemplateIndexArray
//	<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
//	colorIndexArray = 
//	new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
//	colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
//	colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
//	colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
//	colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
//
//	tileGeometry_->setColorArray(colors); 
//	tileGeometry_->setColorIndices(colorIndexArray);
//	tileGeometry_->setColorBinding(osg::Geometry::BIND_PER_VERTEX); 
//	
//	osg::Vec3 position(tx_,0,tz_); 
//	tileTransform_->setPosition( position ); 
//	tileTransform_->addChild(this);
//
//	return tileTransform_;
//}
//osg::PositionAttitudeTransform* TileNode::Init(osg::Vec3 pos)
//{
//	id_ = 0;
//	is_actived_ = false;
//	is_moved_ = false;
//	interval_ = 0;
//	active_time_ = 0;
//	tx_ = pos.x(); tz_ = pos.z();
//
//	tileTransform_ = 
//	new osg::PositionAttitudeTransform(); 
//	
//	tileTransform_->setPosition(pos); 
//	tileTransform_->addChild(this);
//
//	return tileTransform_;
//}
//void TileNode::Activate(bool active, bool move)
//{
//	is_actived_ = active;
//	is_moved_ = move;
//	if(is_actived_)
//	{
//#ifdef HI_IOS4
//		active_time_ = CACurrentMediaTime();
//#else
//		active_time_ = timeGetTime();
//#endif
//		interval_ = rand()%10 + 5;
//
//		osg::Vec4Array* colors = new osg::Vec4Array; 
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		
//		osg::TemplateIndexArray
//		<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
//		colorIndexArray = 
//		new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
//		colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
//		colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
//		colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
//		colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
//		
//		tileGeometry_->setColorArray(colors); 
//		tileGeometry_->setColorIndices(colorIndexArray);
//
//		tileGeometry_->setColorArray(colors); 
//	}
//	else
//	{
//		active_time_ = 0;
//		interval_ = 0;
//
//		osg::Vec4Array* colors = new osg::Vec4Array; 
//		colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//		
//		osg::TemplateIndexArray
//		<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
//		colorIndexArray = 
//		new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
//		colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
//		colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
//		colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
//		colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
//		
//		tileGeometry_->setColorArray(colors); 
//		tileGeometry_->setColorIndices(colorIndexArray);
//
//		tileGeometry_->setColorArray(colors); 
//	}
//		if(is_moved_)
//	{
//		osg::Vec4Array* colors = new osg::Vec4Array; 
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		colors->push_back(osg::Vec4(210.0 / 255.0, 181.0 / 255.0, 203.0 / 255.0, 1.0f) );
//		
//		osg::TemplateIndexArray
//		<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
//		colorIndexArray = 
//		new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
//		colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
//		colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
//		colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
//		colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
//		
//		tileGeometry_->setColorArray(colors); 
//		tileGeometry_->setColorIndices(colorIndexArray);
//
//		tileGeometry_->setColorArray(colors); 
//	}
//}
//
//void TileNode::Update()
//{
//	if(is_moved_)
//	{
//		switch(movement_)
//		{
//		case NONE:
//			break;
//		case UP:
//			tz_ += 0.7;
//			break;
//		case DOWN:
//			tz_ -= 0.7;
//			break;
//		case LEFT:
//			tx_ -= 0.7;
//			break;
//		case RIGHT:
//			tx_ += 0.7;
//			break;
//		default:
//			break;
//		}
//		osg::Vec3 pos(tx_, -0.2, tz_); 
//		SetPosition(pos);
//	}
//	else if(is_actived_)
//	{
//#ifdef HI_IOS4
//		CFTimeInterval dt = CACurrentMediaTime() - active_time_;
//		if(interval_ < dt)
//#else
//		double dt = timeGetTime() - active_time_;
//		if(interval_*300 < dt)
//#endif
//		{
//			Activate(false, false);
//		}
//	}
//}
//
//bool TileNode::InActivate()
//{
//	if(is_actived_)
//	{
//		Activate(false, false);
//		return true;
//	}
//	if(is_moved_)
//	{
//		Drawable(false);
//		return false;
//	}
//	return true;
//}
//
//void TileNode::SetPosition(osg::Vec3 pos)
//{
//	tx_ = pos.x(); tz_ = pos.z();
//	tileTransform_->setPosition(pos); 
//}
//void TileNode::Drawable(bool YnN)
//{
//	if(YnN)
//	{
//		SetGeometry();
//		this->addDrawable(tileGeometry_);
//		Activate(false, true);
//	}
//	else
//	{
//		this->removeDrawable(tileGeometry_); 
//		Activate(false, false);
//	}
//}
//osg::Vec3 TileNode::GetPosition()
//{
//	osg::Vec3 pos(tx_,0,tz_); 
//
//	return pos;
//}
//void TileNode::SetMovement(Movement move)
//{
//	movement_ = move;
//}
//void TileNode::SetGeometry()
//{
//	tileGeometry_ = new osg::Geometry();
//
//	osg::Vec3Array* vertices = new osg::Vec3Array; 
//	vertices->push_back( osg::Vec3(-(width_/2),-0.01,-(height_/2)) ); // front left 
//	vertices->push_back( osg::Vec3( (width_/2),-0.01,-(height_/2)) ); // front right 
//	vertices->push_back( osg::Vec3( (width_/2),-0.01, (height_/2)) ); // back right 
//	vertices->push_back( osg::Vec3(-(width_/2),-0.01, (height_/2)) ); // back left 
//
//	tileGeometry_->setVertexArray( vertices ); 
//
//	osg::DrawElementsUInt* Base = 
//	new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0); 
//	Base->push_back(3); 
//	Base->push_back(2); 
//	Base->push_back(1); 
//	Base->push_back(0); 
//	tileGeometry_->addPrimitiveSet(Base); 
//
//	osg::Vec4Array* colors = new osg::Vec4Array; 
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	colors->push_back(osg::Vec4(239.0 / 255.0, 243.0 / 255.0, 189.0 / 255.0, 1.0f) );
//	
//	osg::TemplateIndexArray
//	<unsigned int, osg::Array::UIntArrayType,4,4> *colorIndexArray;
//	colorIndexArray = 
//	new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType,4,4>;
//	colorIndexArray->push_back(0); // vertex 0 assigned color array element 0
//	colorIndexArray->push_back(1); // vertex 1 assigned color array element 1
//	colorIndexArray->push_back(2); // vertex 2 assigned color array element 2
//	colorIndexArray->push_back(3); // vertex 3 assigned color array element 3
//	
//	tileGeometry_->setColorArray(colors); 
//	tileGeometry_->setColorIndices(colorIndexArray);
//	tileGeometry_->setColorBinding(osg::Geometry::BIND_PER_VERTEX); 
//}