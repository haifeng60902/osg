/*
 *  PrjiManipulator.h
 *  iOSG
 *
 *  Created by whitefir0 on 09. 07. 16.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __PROJECTI_HIMODULES_MHIOSGCAMERA_PRJICAMERAMANIPULATOR_H__
#define __PROJECTI_HIMODULES_MHIOSGCAMERA_PRJICAMERAMANIPULATOR_H__

#include <osgGA/MatrixManipulator>
#include <osg/Quat>

class CPrjICameraManipulator : public osgGA::MatrixManipulator
{
protected :
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;
	
	osg::ref_ptr<osg::Node> m_pNode;
	
	double _modelScale;
	double _minimumZoomScale;
	
	bool _thrown;
	
	osg::Vec3d m_vCenter;
	osg::Quat m_qRotation;
	
	double m_dDistance;
	float _trackballSize;
    
    osg::Matrixf rotation_matrix_;
	
protected:	
	virtual ~CPrjICameraManipulator();

public :
	CPrjICameraManipulator();
	
	virtual const char* className() const
	{
		return "PrjICamera";
	}
	
	virtual void setByMatrix(const osg::Matrix& __matrix);
	virtual void setByInverseMatrix(const osg::Matrix& __matrix)
	{
		setByMatrix(osg::Matrix::inverse(__matrix));
	}
    void setRotationMatrix(const osg::Matrix& rotation_matrix);
	
	virtual osg::Matrix getMatrix() const;
	virtual osg::Matrix getInverseMatrix() const;
	virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const
	{
		return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
	}
	
	virtual float getFusionDistanceValue() const
	{
		return m_dDistance;
	}
    osg::Matrixf getRotationMatrix() const
    {
        return rotation_matrix_;
    }
	
	virtual void setNode(osg::Node* __srcNode);
	virtual const osg::Node* getNode() const;
	
	virtual osg::Node* getNode();
	virtual void home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual void home(double);
	virtual void init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	virtual void getUsage(osg::ApplicationUsage& usage) const;
	
	void computePosition(const osg::Vec3& eye,const osg::Vec3& lv,const osg::Vec3& up);
	
	void setPosition(float __fX, float __fY, float __fZ);
	void setCenter(float __fX, float __fY, float __fZ);
	void setUp(float __fX, float __fY, float __fZ);
	
	void setAutoCamera(bool __bSwitch);
};

#endif