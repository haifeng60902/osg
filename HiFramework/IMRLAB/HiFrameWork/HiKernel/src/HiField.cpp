
//
//  HiField.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#include "HiKernel/HiField.h"
#include "HiKernel/HiContainer.h"

using namespace HiKernel;

HiField::HiField(HiContainer *parent, std::string strName) : HiObject(strName),
m_pParent(parent), m_pInput(NULL), m_bDirty(false)
{
	m_pParent->AddField(this);
	/* m_sId = id;
	 m_strName = strName;
	 m_pInput = NULL;
	 m_iDirty = 0;
	 m_bSuspend = false;
	 */
}

HiField::~HiField()
{
	if(m_pInput != NULL)
	{
		delete m_pInput;
	}
	for(unsigned int i =0 ;i<m_vInputs.size(); i++)
	{
		delete m_vInputs[i];
		m_vInputs[i] = NULL;
	}
	for(unsigned int i =0; i<m_vOutputs.size(); i++)
	{
		delete m_vOutputs[i];
		m_vOutputs[i] = NULL;
	}
	m_vInputs.clear();
	m_vOutputs.clear();
}

void 
HiField::Connect(HiField *dest)
{
	if(dest == NULL)
		return ;
	
	m_vOutputs.push_back(dest);
	
	dest->GetInputs()->push_back(this);
}

void 
HiField::DisConnect(HiField *dest)
{
	// not implemended yet !!
	
	//	if(dest == NULL)
	//		return ;
	//
	//	if(m_vOutputs.empty())
	//		return ;
	//
	//	for(std::vector<HiField*>::iterator it=m_vOutputs.begin(); it!=m_vOutputs.end(); it++)
	//	{
	//		if(*it == dest)
	//		{
	//			m_vOutputs.erase(it);
	//			break;
	//		}
	//	}
	//	if(dest->GetInputs()->empty())
	//		return ;
	//
	//	for(std::vector<HiField*>::iterator it=m_vInputs.begin(); it!=m_vInputs.end(); it++)
	//	{
	//		if(*it == this)
	//		{
	//			m_vInputs.erase(it);
	//			break;
	//		}
	//	}
}

void 
HiField::Validate()
{
	if (m_pInput && m_pInput->GetDirty() != 0)
	{
		Copy(m_pInput);
		m_pInput = NULL;
		m_bDirty = true;
	}
}
//
//void HiField::SetID(float id)
//{
//	m_sId = id;
//}
//
//void HiField::SetName(std::string strName)
//{
//	m_strName = strName;
//}
//
void HiField::SetInput(HiField *pField)
{
	m_pInput = pField;
}

/*
 bool HiField::GetID()
 {
 return m_sId;
 }
 */

////
////void HiField::SetSuspend(bool bSuspend)
////{
////	m_bSuspend = bSuspend;
////}
//
//void HiField::Suspend()
//{
//	m_bSuspend = true;
//}
//
//void HiField::Resume()
//{
//	m_bSuspend = false;
//}
//
void HiField::SetDirty(bool bDirty)
{
	m_bDirty = bDirty;
}

bool 
HiField::GetDirty()
{
	return m_bDirty;
}
//
//bool HiField::GetSuspend()
//{
//	return m_bSuspend;
//}

/*
 const std::string HiField::GetName()
 {
 return m_strName;
 }
 */

HiField* HiField::GetInput()
{
	return m_pInput;
}

std::vector<HiField*>* HiField::GetInputs()
{
	return &m_vInputs;
}

std::vector<HiField*> HiField::GetOutputs()
{
	return m_vOutputs;
}



//std::string HiSFVec3f::m_strTypeName = "HiSFVec3f";

/*
 
 HiSFVec3f::HiSFVec3f(HiContainer *parent, short id, std::string strName) : HiField(parent, id, strName)
 {
 parent->AddField(this);
 
 #ifndef HI_IOS4
 m_vStorage = new D3DXVECTOR3;
 m_vStorage->x = 0.0f;
 m_vStorage->y = 0.0f;
 m_vStorage->z = 0.0f;
 m_vScale.x = 1.0f;
 m_vScale.y = 1.0f;
 m_vScale.z = 1.0f;
 m_vOffset.x = 0.0f;
 m_vOffset.y = 0.0f;
 m_vOffset.z = 0.0f;
 m_vOrder.x = 1.0f;
 m_vOrder.y = 1.0f;
 m_vOrder.z = 1.0f;
 #else
 magnetic_heading_ = 0.0f;
 zx_rotation_angle_ = 0.0f;
 
 offset_x_ = 0.0f;
 offset_y_ = 0.0f;
 offset_z_ = 0.0f;
 
 order_x_ = 1.0f;
 order_y_ = 1.0f;
 order_z_ = 1.0f;
 
 storage_x_ = 0.0f;
 storage_y_ = 0.0f;
 storage_z_ = 0.0f;
 
 scale_x_ = 1.0f;
 scale_y_ = 1.0f;
 scale_z_ = 1.0f;
 #endif
 }
 
 //HiSFVec3f::HiSFVec3f(HiContainer *parent, short id, std::string strName, D3DXVECTOR3 *val) : HiField(parent, id, strName)
 //{
 //	parent->AddField(this);
 
 //	m_vStorage = val;
 //	m_vScale.x = 1.0f;
 //	m_vScale.y = 1.0f;
 //	m_vScale.z = 1.0f;
 //	m_vOffset.x = 0.0f;
 //	m_vOffset.y = 0.0f;
 //	m_vOffset.z = 0.0f;
 //	m_vOrder.x = 1.0f;
 //	m_vOrder.y = 1.0f;
 //	m_vOrder.z = 1.0f;
 //}
 
 HiSFVec3f::~HiSFVec3f()
 {
 //	m_vStorage = NULL;
 }
 
 void HiSFVec3f::Copy(HiField *pField)
 {
 #ifndef HI_IOS4
 D3DXVECTOR3 temp = *(((HiSFVec3f *)pField)->m_vStorage);
 m_vStorage->x = temp.x* m_vScale.x   + m_vOffset.x;
 m_vStorage->y = temp.y* m_vScale.y + m_vOffset.y;
 m_vStorage->z = temp.z* m_vScale.z + m_vOffset.z;
 #else
 float parameter_field_x = static_cast<HiSFVec3f*>(pField)->storage_x_;
 float parameter_field_y = static_cast<HiSFVec3f*>(pField)->storage_y_;
 float parameter_field_z = static_cast<HiSFVec3f*>(pField)->storage_z_;
 
 storage_x_ = parameter_field_x * scale_x_ + offset_x_;
 storage_y_ = parameter_field_y * scale_y_ + offset_y_;
 storage_z_ = parameter_field_z * scale_z_ + offset_z_;
 
 magnetic_heading_ = static_cast<HiSFVec3f*>(pField)->magnetic_heading_;
 zx_rotation_angle_ = static_cast<HiSFVec3f*>(pField)->zx_rotation_angle_;
 #endif
 }
 
 void HiSFVec3f::Set(const D3DXVECTOR3 &vVec)
 {
 *m_vStorage = vVec;
 SetDirty(1);
 }
 #else
 void HiSFVec3f::Set(double source_value_x,
 double source_value_y,
 double source_value_z,
 double magnetic_heading,
 double zx_rotation_angle)
 {
 storage_x_ = source_value_x; 
 storage_y_ = source_value_y;
 storage_z_ = source_value_z;
 
 magnetic_heading_ = magnetic_heading;
 zx_rotation_angle_ = zx_rotation_angle;
 
 SetDirty(1);
 }
 #endif
 */



HiVec3f::HiVec3f(HiContainer *parent, std::string strName) : HiField(parent, strName)
{	
	m_pStorage = new float[3];
	
	m_vScale[0] = m_vScale[1] = m_vScale[2] = 1.0f;
	m_vOffset[0] = m_vOffset[1] = m_vOffset[2] = 0.0f;
	
	m_vOrder[0] = 0;
	m_vOrder[1] = 1;
	m_vOrder[2] = 2;
}


/* 
 // chpark = ?åÎ©∏???∏Ï∂ú Î∂ÄÎ∂ÑÏóê??Î¨∏Ï†úÍ∞Ä ?àÏùÑ ???àÍ∏∞ ?åÎ¨∏???úÏô∏?úÏºú Î¨∏Ï†úÎ•?Î∞©Ï??? HiVec3f::HiVec3f(HiContainer *parent, short id, std::string strName, float *val) : HiField(parent, id, strName)
 {
 m_pStorage = val;
 } */

HiVec3f::~HiVec3f()
{
	delete m_pStorage;
}

void HiVec3f::Set(const float vVec[3])
{
	memcpy(m_pStorage, vVec, sizeof(float) * 3 );
	SetDirty(true);
}

void 
HiVec3f::Copy(HiField *pField)
{	
	float* sorc = static_cast<HiVec3f*> (pField)->m_pStorage;
	
	for (int i=0; i<3; i++) {
		int idx = m_vOrder[i];
		m_pStorage[i] = sorc[idx] * m_vScale[i] + m_vOffset[i];
	}
}

const float*
HiVec3f::Get()
{
	return m_pStorage;
}

const std::string 
HiVec3f::GetTypeName()
{
	return "HiVec3f";
}

void 
HiVec3f::SetScale(float *fScale)
{
	memcpy(m_vScale, fScale, sizeof(float)*3);
}

void 
HiVec3f::SetOffset(float *fOffset)
{
	memcpy(m_vOffset, fOffset, sizeof(float)*3);
}

void 
HiVec3f::SetOrder(int *iOrder)
{
	memcpy(m_vOrder, iOrder, sizeof(int)*3);
}


