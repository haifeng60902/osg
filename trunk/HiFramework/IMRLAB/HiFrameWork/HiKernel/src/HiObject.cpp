//
//  HiObject.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#include "HiKernel/HiObject.h"

using namespace HiKernel;

int HiObject::m_siIndex = 0;


HiObject::HiObject(const std::string _name) : m_sName(_name)
{
	m_iIndex = m_siIndex++;
}

HiObject::~HiObject() 
{
}

HiObject* HiObject::Find(std::vector<HiObject*> &_list, const std::string& _name)
{
	for( std::vector<HiObject*>::iterator i= _list.begin();i!= _list.end();i++)		
		if ( !(*i)->GetName().compare(_name) )
			return (*i);
	return 0;
}
HiObject* HiObject::Find(std::vector<HiObject*> &_list, int id)
{
	for( std::vector<HiObject*>::iterator i=_list.begin();i!=_list.end();i++)
		if ( (*i)->GetID() == id)
			return (*i);
	return 0;
}

const std::string HiObject::GetName()                                             
{ 
	return m_sName.empty() ? "NULL_NAME": m_sName; 
}

void HiObject::SetName(const std::string& _name)                  
{ 
	m_sName = _name; 
}

int HiObject::GetID()                                                                  
{ 
	return m_iIndex;  
}


//bool HiObject::HandleMessage(const HiTelegram &msg)
//{
//	return true;
//}