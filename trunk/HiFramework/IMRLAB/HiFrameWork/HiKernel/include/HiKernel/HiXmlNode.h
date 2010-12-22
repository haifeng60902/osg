//
//  HiXmlNode.h
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//

#pragma once

#include "HiDLL.h"
#include <vector>
#include <string>


namespace HiKernel {
	
	
	class HI_DLLEXPORT HiXmlNode
	{
		typedef std::pair<std::string, std::string> Attribute;
		
	public:
		
		static HiXmlNode* ParseFile(const char* _filename);
		static HiXmlNode* ParseString(char* _str);
		
		HiXmlNode();
		HiXmlNode(const std::string _name);
		HiXmlNode(const unsigned char* _name);
		//HiXmlNode(char*name, char*fmt, ...);
		
		~HiXmlNode();
		
		/* Name */    
		const std::string GetName();
		void SetName(const std::string& _name);
		void SetName(const unsigned char* _name);
		int NameCmp(const std::string& str);
		// --- int NameCaseSensitiveCmp(const std::string& str);
		
		// Attributes 
		int GetNumAttrs(); 
		const std::string GetAttrs(const std::string& _name);
		
		int GetAttrs(int i, std::string& _name, std::string& _value);
		
		int GetAttrs(const std::string &name, std::vector<std::string> & dst);
		int GetAttrs(const std::string &name, int num, double* dst);
		int GetAttrs(const std::string &name, int num, float* dst);
		int GetAttrs(const std::string &name, int num, int* dst);
		
		int GetAttrs(const std::string &name, double* dst);
		int GetAttrs(const std::string &name, float* dst);
		int GetAttrs(const std::string &name, int* dst);
		int GetAttrs(const std::string &name, bool *dst);
		
		int SetAttrs(const std::string& _name, const std::string& _val);
		int SetAttrs(const unsigned char* _name, const unsigned char* _val);
		int RemoveAttrs(const std::string& _name);
		// int FindAttr(const std::string & name);
		// int RemoveAttrs(int i);
		
		// Value
		const std::string GetValue();
		void SetValue(const std::string val);
		void SetValue(const unsigned char* val);
		
		// Children 
		HiXmlNode* GetParent();
		int GetNumChildren();
		HiXmlNode* GetChild(int i);
		HiXmlNode* FindChild(const std::string _name);
		int AddChild(HiXmlNode*child);
		int RemoveChild(HiXmlNode* child);
		void SetParent(HiXmlNode* _parent);
		
		// int AddLeaf(char* _name, char* fmt, ...)
		// int RemoveChild(int i) 
		// int Write(char* filename)
		// int Write(FILE*fp, int indent)
		// int getInputMngrKey(int* key);
		// int getNotifyLevel(int* nl);
		// HiXmlNode* TravLoadExtRefs()
		
	private:     
		std::string m_sName;
		std::string m_sValue;
		
		HiXmlNode* m_pParent;
		std::vector<Attribute>  m_listAttrs;
		std::vector<HiXmlNode*> m_listChildren;
		
	};
}
