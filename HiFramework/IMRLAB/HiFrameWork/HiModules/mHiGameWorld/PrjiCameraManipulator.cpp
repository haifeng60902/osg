/*
 *  PrjiManipulator.cpp
 *  iOSG
 *
 *  Created by whitefir0 on 09. 07. 16.
 *  Copyright 2009 IMRLAB. All rights reserved.
 *
 */

#include "PrjiCameraManipulator.h"
#include <osg/Notify>

CPrjICameraManipulator::CPrjICameraManipulator()
{
	m_dDistance = 1.0f;
}

CPrjICameraManipulator::~CPrjICameraManipulator()
{
}

void CPrjICameraManipulator::setPosition(float __fX, float __fY, float __fZ)
{
	_homeEye.set(__fX,__fY,__fZ);
}
void CPrjICameraManipulator::setCenter(float __fX, float __fY, float __fZ)
{
	_homeCenter.set(__fX,__fY,__fZ);
}
void CPrjICameraManipulator::setUp(float __fX, float __fY, float __fZ)
{
    _homeUp.set(__fX,__fY,__fZ);
}
void CPrjICameraManipulator::setAutoCamera(bool __bSwitch)
{
	_autoComputeHomePosition = __bSwitch;
}

void CPrjICameraManipulator::setRotationMatrix(const osg::Matrix& rotation_matrix)
{
    rotation_matrix_ = rotation_matrix;
}

void CPrjICameraManipulator::setNode(osg::Node* __srcNode)
{
	m_pNode = __srcNode;
	
	if( getAutoComputeHomePosition())
		computeHomePosition();
}
const osg::Node* CPrjICameraManipulator::getNode() const
{
	return m_pNode.get();
}

osg::Node* CPrjICameraManipulator::getNode()
{
	return m_pNode.get();
}

void CPrjICameraManipulator::home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	home(ea.getTime());
	
	us.requestRedraw();
	us.requestContinuousUpdate(false);
}

void CPrjICameraManipulator::home(double)
{
	if(getAutoComputeHomePosition())
		computeHomePosition();
	
	computePosition(_homeEye, _homeCenter, _homeUp);
}

void CPrjICameraManipulator::init(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
}

bool CPrjICameraManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
//	switch(ea.getEventType())
//    {
//        case(GUIEventAdapter::FRAME):
//            if (_thrown)
//            {
//                if (calcMovement()) us.requestRedraw();
//            }
//            return false;
//        default:
//            break;
//    }
//	
//    if (ea.getHandled()) return false;
//	
//    switch(ea.getEventType())
//    {
//        case(GUIEventAdapter::PUSH):
//        {
//            flushMouseEventStack();
//            addMouseEvent(ea);
//            if (calcMovement()) us.requestRedraw();
//            us.requestContinuousUpdate(false);
//            _thrown = false;
//            return true;
//        }
//			
//        case(GUIEventAdapter::RELEASE):
//        {
//            if (ea.getButtonMask()==0)
//            {
//				
//                double timeSinceLastRecordEvent = _ga_t0.valid() ? (ea.getTime() - _ga_t0->getTime()) : DBL_MAX;
//                if (timeSinceLastRecordEvent>0.02) flushMouseEventStack();
//				
//                if (isMouseMoving())
//                {
//                    if (calcMovement())
//                    {
//                        us.requestRedraw();
//                        us.requestContinuousUpdate(true);
//                        _thrown = true;
//                    }
//                }
//                else
//                {
//                    flushMouseEventStack();
//                    addMouseEvent(ea);
//                    if (calcMovement()) us.requestRedraw();
//                    us.requestContinuousUpdate(false);
//                    _thrown = false;
//                }
//				
//            }
//            else
//            {
//                flushMouseEventStack();
//                addMouseEvent(ea);
//                if (calcMovement()) us.requestRedraw();
//                us.requestContinuousUpdate(false);
//                _thrown = false;
//            }
//            return true;
//        }
//			
//        case(GUIEventAdapter::DRAG):
//        {
//            addMouseEvent(ea);
//            if (calcMovement()) us.requestRedraw();
//            us.requestContinuousUpdate(false);
//            _thrown = false;
//            return true;
//        }
//			
//        case(GUIEventAdapter::MOVE):
//        {
//            return false;
//        }
//			
//        case(GUIEventAdapter::KEYDOWN):
//            if (ea.getKey()== GUIEventAdapter::KEY_Space)
//            {
//                flushMouseEventStack();
//                _thrown = false;
//                home(ea,us);
//                return true;
//            }
//            return false;
//        case(GUIEventAdapter::FRAME):
//            if (_thrown)
//            {
//                if (calcMovement()) us.requestRedraw();
//            }
//            return false;
//        default:
//            return false;
//    }
//    return false;
}

void CPrjICameraManipulator::getUsage(osg::ApplicationUsage& usage) const
{
}

void CPrjICameraManipulator::setByMatrix(const osg::Matrix& __matrix)
{
}

osg::Matrix CPrjICameraManipulator::getMatrix() const
{
	return osg::Matrix::translate(0.0,0.0,m_dDistance) * osg::Matrix::rotate(m_qRotation) * osg::Matrix::translate(m_vCenter) * rotation_matrix_;
}

osg::Matrix CPrjICameraManipulator::getInverseMatrix() const
{
	return (osg::Matrix::translate(-m_vCenter) *
    osg::Matrix::rotate(m_qRotation.inverse()) *
    osg::Matrix::translate(0.0,0.0,-m_dDistance) * rotation_matrix_);
}

void CPrjICameraManipulator::computePosition(const osg::Vec3& eye,const osg::Vec3& center,const osg::Vec3& up)
{	
    osg::Vec3 lv(center-eye);
	
    osg::Vec3 f(lv);
    f.normalize();
    osg::Vec3 s(f^up);
    s.normalize();
    osg::Vec3 u(s^f);
    u.normalize();
    
    osg::Matrix rotation_matrix(s[0],     u[0],     -f[0],     0.0f,
                                s[1],     u[1],     -f[1],     0.0f,
                                s[2],     u[2],     -f[2],     0.0f,
                                0.0f,     0.0f,     0.0f,      1.0f);
	
    m_vCenter = center;
    m_dDistance = lv.length();
    m_qRotation = rotation_matrix.getRotate().inverse();
}
