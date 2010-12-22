//
//  HiXmlNode.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#include "HiKernel/HiXmlNode.h"

#include <libxml/xmlreader.h>
#include <stack>
#include <sstream>


using namespace HiKernel;

#ifdef LIBXML_READER_ENABLED

HiXmlNode*
HiXmlNode::ParseFile(const char* _filename)
{
	LIBXML_TEST_VERSION
	
	HiXmlNode* root = NULL;
	xmlTextReaderPtr _reader;
	
	
	//_reader = xmlReaderForFile(_filename, NULL,
	//						XML_PARSE_DTDATTR |  /* default DTD attributes */
	//                        XML_PARSE_NOENT |    /* substitute entities */
	//                        XML_PARSE_DTDVALID); /* validate with the DTD */
	
	
	_reader = xmlReaderForFile(_filename, NULL,
							   XML_PARSE_DTDATTR |  /* default DTD attributes */
							   XML_PARSE_NOENT     /* substitute entities */
							   );
	
	
	if (_reader == NULL)  
	{
		fprintf(stderr, "Unable to open %s\n", _filename);
		return NULL;	
	}
	
	int ret = xmlTextReaderRead(_reader);
	// int pDepth = -1;
	
	HiXmlNode* _parent;
	std::stack<int> s;
	
	while (ret) 
	{
		
		HiXmlNode* node;  
		
		int thisType = xmlTextReaderNodeType(_reader);
		int cDepth = xmlTextReaderDepth(_reader);
		
		if ( thisType == XML_READER_TYPE_ELEMENT  || thisType == XML_READER_TYPE_TEXT) 
		{
			
			node = new HiXmlNode;
			
			if ( s.empty() ) 
			{
				root = node;
				_parent = root;
				s.push(cDepth );
			} 
			else if ( s.top() < cDepth) 
			{
				s.push(cDepth);
				_parent->AddChild(node);
				node->SetParent(_parent);
				_parent = node;
			} 
			else 
			{
				while ( s.top() >= cDepth ) 
				{
					//std::cout << s.top() << " --- " << cDepth << std::endl;
					_parent = _parent->m_pParent;
					s.pop();
				}  
				_parent->AddChild(node);
				node->SetParent(_parent);
				_parent = node;
				s.push(cDepth);
			}
			
			//std::cout << xmlTextReaderDepth(_reader) << " ";
			
			if ( thisType == XML_READER_TYPE_ELEMENT ) 
			{
				const xmlChar *name = xmlTextReaderConstName(_reader);
				
				node->SetName(name);
				
				//std::cout << "-----" << name << std::endl;
				
				if(xmlTextReaderHasAttributes(_reader)) 
				{
					
					while(xmlTextReaderMoveToNextAttribute(_reader)) 
					{
						//std::cout << "---" << xmlTextReaderConstName(_reader) << " = " << xmlTextReaderConstValue(_reader) << std::endl;
						node->SetAttrs(xmlTextReaderConstName(_reader), xmlTextReaderConstValue(_reader));
					}
					for ( int i=0;i<node->GetNumAttrs();i++) 
					{
						std::string _name;
						std::string _value;
						node->GetAttrs(i,_name,_value);
						//std::cout << _name << "=" << _value << std::endl;
					}
				}
				
			} 
			else if (thisType == XML_READER_TYPE_TEXT) 
			{
				const xmlChar *value = xmlTextReaderConstValue(_reader);
				node->SetValue(value);
				//_parent = _parent->_parent;                              //sylvi
				//std::cout << "-" << value << std::endl;
			}
		}      
		ret = xmlTextReaderRead(_reader);
	}
	
	if (xmlTextReaderIsValid(_reader) != 1) 
	{
		fprintf(stderr, "Document %s does not validate\n", _filename);
	}
	
	xmlFreeTextReader(_reader);
	if (ret != 0) 
	{
		fprintf(stderr, "%s : failed to parse\n", _filename);
	}
	
	xmlCleanupParser();
	xmlMemoryDump();
	
	return root;
}


HiXmlNode*
HiXmlNode::ParseString(char* str)
{
	LIBXML_TEST_VERSION
	//sylvi
	
	HiXmlNode* root = NULL;
	xmlTextReaderPtr _reader;
	
	_reader = xmlReaderForMemory(str, strlen(str), NULL, NULL, XML_PARSE_DTDATTR);
	
	int ret = xmlTextReaderRead(_reader);
	// int pDepth = -1;
	
	HiXmlNode* _parent;
	std::stack<int> s;
	
	while (ret)
	{
		
		HiXmlNode* node;  
		
		int thisType = xmlTextReaderNodeType(_reader);
		int cDepth = xmlTextReaderDepth(_reader);
		
		//printf("depth %d\n", cDepth);
		
		if ( thisType == XML_READER_TYPE_ELEMENT  || thisType == XML_READER_TYPE_TEXT) 
		{
			
			node = new HiXmlNode;
			
			if ( s.empty() ) 
			{
				root = node;
				_parent = root;
				s.push(cDepth );
			} 
			else if ( s.top() < cDepth) 
			{
				s.push(cDepth);
				_parent->AddChild(node);
				node->SetParent(_parent);
				_parent = node;
			} 
			else 
			{
				while ( s.top() >= cDepth ) 
				{
					//std::cout << s.top() << " --- " << cDepth << std::endl;
					_parent = _parent->m_pParent;
					s.pop();
				}  
				_parent->AddChild(node);
				node->SetParent(_parent);
				_parent = node;
				s.push(cDepth);
			}
			
			//std::cout << xmlTextReaderDepth(_reader) << " ";
			
			if ( thisType == XML_READER_TYPE_ELEMENT ) 
			{
				const xmlChar *name = xmlTextReaderConstName(_reader);
				
				node->SetName(name);
				
				//std::cout << "-----" << name << std::endl;
				
				if(xmlTextReaderHasAttributes(_reader)) 
				{
					while(xmlTextReaderMoveToNextAttribute(_reader)) 
					{
						//std::cout << "---" << xmlTextReaderConstName(_reader) << " = " << xmlTextReaderConstValue(_reader) << std::endl;
						node->SetAttrs(xmlTextReaderConstName(_reader), xmlTextReaderConstValue(_reader));
					}
					for ( int i=0;i<node->GetNumAttrs();i++) 
					{
						std::string _name;
						std::string _value;
						node->GetAttrs(i,_name,_value);
						//std::cout << _name << "=" << _value << std::endl;
					}
				}
				
			} 
			else if (thisType == XML_READER_TYPE_TEXT) 
			{
				const xmlChar *value = xmlTextReaderConstValue(_reader);
				node->SetValue(value);
				//_parent = _parent->_parent;
				//std::cout << "-" << value << std::endl;
			}
		}      
		ret = xmlTextReaderRead(_reader);
	}
	
	
	xmlFreeTextReader(_reader);
	if (ret != 0) 
	{
		fprintf(stderr, "%s : failed to parse\n", str);
	}
	
	//end sylvi
	xmlCleanupParser();
	xmlMemoryDump();
	
	return root;
}


HiXmlNode::HiXmlNode()
{
}

HiXmlNode::HiXmlNode(const std::string _name) : m_sName(_name)
{
}

HiXmlNode::HiXmlNode(const unsigned char* _name) : m_sName((const char*) _name)  
{
}

HiXmlNode::~HiXmlNode()
{
	// delete
	// free
}


// Name

const std::string 
HiXmlNode::GetName()                             
{ 
	return m_sName; 
}

void 
HiXmlNode::SetName(const std::string& _name)                  
{ 
	m_sName=_name;  
}

void 
HiXmlNode::SetName(const unsigned char* _name)                
{ 
	if (_name)  
		m_sName=(const char*)_name; 
}

int 
HiXmlNode::NameCmp(const std::string& _str)                     
{ 
	return m_sName.compare(_str); 
}

// Attribute

int 
HiXmlNode::GetNumAttrs()										
{ 
	return m_listAttrs.size(); 
}

int
HiXmlNode::GetAttrs(int i, std::string& _name, std::string& _value)
{
	if ( !m_listAttrs.empty() && 0 <= i && i < m_listAttrs.size() ) 
	{
		_name = m_listAttrs[i].first;
		_value = m_listAttrs[i].second;
		return 1;
	}
	return 0;
}

const std::string
HiXmlNode::GetAttrs(const std::string& _name)
{
	std::string _result;
	_result.clear();
	
	for(std::vector<Attribute>::iterator it = m_listAttrs.begin(); it!=m_listAttrs.end(); it++) 
	{
		if ( !(*it).first.compare( _name) ) 
		{
			_result = (*it).second;
			break;
		}
	}
	return _result;
}


int
HiXmlNode::GetAttrs(const std::string &_name, std::vector<std::string> &_dst)
{
	std::string _val = GetAttrs(_name);
	
	if ( _val.empty() ) return 0;
	
	// convert from ',' to ' '      
	for(int i=0;i<_val.size();i++)
		if ( _val[i] == ',' ) _val[i]=' ';
	
	std::istringstream _buff(_val);
	std::string _temp;
	
	while ( _buff >> _temp ) 
	{
		_dst.push_back(_temp);
	}
	
	return _dst.size();
}

int
HiXmlNode::GetAttrs(const std::string &_name, int _num, double* _dst)
{
	std::string _val = GetAttrs(_name);
	if ( !_val.empty()) 
	{
		std::istringstream  _buff(_val);
		for(int i=0;i<_num;i++)
			_buff >> _dst[i];
		return 1;
	}
	return 0;
}



int
HiXmlNode::GetAttrs(const std::string &_name, int _num, float* _dst)
{
	std::string _val = GetAttrs(_name);
	if ( !_val.empty()) 
	{
		std::istringstream  _buff(_val);
		for(int i=0;i<_num;i++)
			_buff >> _dst[i];
		return 1;
	}
	return 0;
}

int
HiXmlNode::GetAttrs(const std::string &_name, int _num, int* _dst)
{
	std::string _val = GetAttrs(_name);
	if ( !_val.empty())
	{
		std::istringstream _buff(_val);
		for(int i=0;i<_num;i++)
			_buff >> _dst[i];
		return 1;
	}
	return 0;
}


int 
HiXmlNode::GetAttrs(const std::string &_name, double* _dst)      
{ 
	return GetAttrs(_name,1,_dst); 
}

int 
HiXmlNode::GetAttrs(const std::string &_name, float* _dst)       
{ 
	return GetAttrs(_name,1,_dst); 
} 

int 
HiXmlNode::GetAttrs(const std::string &_name, int* _dst)        
{ 
	return GetAttrs(_name,1,_dst); 
}

int
HiXmlNode::GetAttrs(const std::string &_name, bool *_dst)
{
	std::string _val = GetAttrs(_name);
	if ( !_val.empty()) 
	{
		if ( _val == "true" )
			*_dst = true;
		else
			*_dst = false;
		return 1;
	}
	return 0;
}

int
HiXmlNode::SetAttrs(const std::string& _name, const std::string& _val)
{
	Attribute _attr;
	_attr.first = _name;
	_attr.second = _val;
	m_listAttrs.push_back(_attr);
	return m_listAttrs.size();
}      

int
HiXmlNode::SetAttrs(const unsigned char* _name, const unsigned char* _val)
{
	Attribute _attr;
	_attr.first = (const char*)_name;
	_attr.second = (const char*)_val;
	m_listAttrs.push_back(_attr);
	return m_listAttrs.size();
}

int
HiXmlNode::RemoveAttrs(const std::string& _name)
{      
	for(std::vector<Attribute>::iterator it = m_listAttrs.begin(); it!=m_listAttrs.end(); it++) 
	{
		if ( !(*it).first.compare( _name) ) 
		{
			m_listAttrs.erase(it);
			return 1;
		}
	}
	return 0;
}

// value

const std::string 
HiXmlNode::GetValue()                                    
{ 
	return m_sValue; 
}

void 
HiXmlNode::SetValue(const std::string _val)                            
{ 
	m_sValue = _val; 
}

void 
HiXmlNode::SetValue(const unsigned char* _val)                         
{ 
	if (_val)   m_sValue = (const char*)_val; 
}

//children

HiXmlNode* 
HiXmlNode::GetParent()                                                          
{ 
	return m_pParent; 
}

int 
HiXmlNode::GetNumChildren()                                                           
{ 
	return m_listChildren.size(); 
}

HiXmlNode* 
HiXmlNode::GetChild(int i)                                                      
{ 
	return m_listChildren[i]; 
}

HiXmlNode*
HiXmlNode::FindChild(const std::string _name)
{
	for(std::vector<HiXmlNode*>::iterator it = m_listChildren.begin(); it!=m_listChildren.end(); it++) 
	{
		if ( !(*it)->GetName().compare(_name) ) 
		{
			return (*it);
		}
	}
	return NULL;
}

int
HiXmlNode::AddChild(HiXmlNode*_child)
{
	m_listChildren.push_back(_child);
	return 1;
}

int
HiXmlNode::RemoveChild(HiXmlNode* _child)
{
	for(std::vector<HiXmlNode*>::iterator it = m_listChildren.begin(); it!=m_listChildren.end(); it++) 
	{
		if ( (*it) == _child ) 
		{
			m_listChildren.erase(it);
			return 1;
		}
	}
	return 0;
}

void 
HiXmlNode::SetParent(HiXmlNode* _parent)                                      
{ 
	m_pParent = _parent; 
}

#endif
