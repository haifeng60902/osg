//
//  HiField.cpp
//  HiKernel GLES1.1
//
//  Created by Park Changhoon on 10. 7. 23..
//  Copyright http://www.imrlab.hoseo.edu 2010. All rights reserve
//


#pragma once

#include "HiDLL.h"
#include "HiObject.h"

namespace HiKernel {
	
	
	class HiContainer;
	
	
	class HI_DLLEXPORT HiField : public HiObject
	{
	private:
		HiContainer*	m_pParent;
		// short			m_sId;  //  HiObject??id?� 중복?�으�??�외...
		
		// std::string		m_strName; //  HiObject??id?� 중복?�으�??�외...
		
		
		//bool			m_bSuspend;
		bool				m_bDirty;
		
		HiField*					m_pInput;
		std::vector<HiField*>		m_vInputs;
		std::vector<HiField*>		m_vOutputs;
		//std::vector<bool>			m_vActives;
		
		
		//unsigned char				m_cEventClass;
		
	protected:
		
		
		
	public:
		HiField(HiContainer *parent, std::string strName);
		// HiField(HiContainer *parent, short id, std::string strName); //  HiObject??id?� 중복?�으�??�외...
		
		virtual ~HiField();
		
		void						Connect(HiField *dest);			// FieldâÂ¶ Ã¸Â¨âÂ· (destâÂ¶ OutputsÃ¸Â° Â«â¢Î©Â¨ Â«ÅâÃ destÂ¿Â« inputsÃ¸Â° thisâÂ¶ Â«â¢Î©Â¨)
		void						DisConnect(HiField *dest);		// FieldâÂ¶ Â¡Â¶ââ (destâÂ¶ OutputsÃ¸Â°Âºâ  Â¡Â¶ââÂ«ÅâÃ destÂ¿Â« inputsÃ¸Â°Âºâ  thisâÂ¶ Â¡Â¶ââ)
		//
		//	void						SetID(float id);							// idÂºÂ¬ââ
		//	void						SetName(std::string strName);				// Â¿ÃâÃ ÂºÂ¬ââ
		//
		void						SetInput(HiField *pField);					// FieldâÂ¶ ÂºÂ¬
		//	
		//	void						Suspend();
		//	void						Resume();
		//
		void						Validate();
		//
		virtual void				Copy(HiField *pField) = 0;
		virtual	void				SetScale(float *fScale) = 0;
		virtual void				SetOffset(float *fOffset) = 0;
		virtual void				SetOrder(int *iOrder) = 0;
		//
		virtual const std::string	GetTypeName()	{		return "HiField";			}
		
		//	short						GetID();						//  HiObject??id?� 중복?�으�??�외... 
		void						SetDirty(bool iDirty);
		bool							GetDirty();						// DirtyâÂ¶ Ã¦ÃÃ¦ÃÃ¸Â¬Â¥Å¸
		//bool						GetActive(int id);
		//void						SetActive(int id, bool active);
		
		
		// const	std::string			GetName();						//  HiObject??id?� 중복?�으�??�외... // Â¿ÃâÃÂ¿Âª Ã¦ÃÃ¦ÃÃ¸Â¬Â¥Å¸ (constâÅ Â«ÅÂ¥Â¬ Â¿ÃÂ¿ÃÂ¥Â¬ Â¿ÃâÃÂ¿Ã Ï?Å¸â¤ÃÃ¦ÃÂºÂ± Ã¦Â»ÂµÂ«Â±â? Ã£Ã¶Ï?ÃÃ¸Â°)
		HiField*					GetInput();						// inputÂ¿Âª Ã¦ÃÃ¦Ã Ã¸Â¬Â¥Å¸
		std::vector<HiField*>*		GetInputs();					// inputsÂ¿Âª Ã¦ÃÃ¦ÃÃ¸Â¬Â¥Å¸
		std::vector<HiField*>		GetOutputs();					// outputsâÂ¶ Ã¦ÃÃ¦ÃÃ¸Â¬Â¥Å¸
	};
	
	
	//
	//class HI_DLLEXPORT HiSFVec3f : public HiField
	//{
	//public:
	//#ifndef __IPHONE_TOUCH__
	//	D3DXVECTOR3		*m_vStorage;			// ?�Ø«œ¥�??�™
	//	D3DXVECTOR3		m_vScale;
	//	D3DXVECTOR3		m_vOffset;
	//	D3DXVECTOR3		m_vOrder;
	//#else
	//    double          magnetic_heading_;
	//    double          zx_rotation_angle_;
	//    
	//    float           offset_x_;
	//    float           offset_y_;
	//    float           offset_z_;
	//    
	//    float           order_x_;
	//    float           order_y_;
	//    float           order_z_;
	//    
	//    double           storage_x_;
	//    double           storage_y_;
	//    double           storage_z_;
	//    
	//    float           scale_x_;
	//    float           scale_y_;
	//    float           scale_z_;
	//#endif
	//public:
	//	HiSFVec3f(HiContainer *parent, short id, std::string strName);
	//    
	//#ifndef __IPHONE_TOUCH__
	//	HiSFVec3f(HiContainer *parent, short id, std::string strName, D3DXVECTOR3 *val);
	//#endif
	//	virtual ~HiSFVec3f();
	//
	//	void Copy(HiField *pField);
	//    
	//	void SetScale(float *fScale) { /*m_vScale.x = fScale[0]; m_vScale.y = fScale[1]; m_vScale.z = fScale[2];*/ }
	//	void SetOffset(float *fOffset) { /*m_vOffset.x = fOffset[0]; m_vOffset.y = fOffset[1]; m_vOffset.z = fOffset[2];*/ }
	//	void SetOrder(int *iOrder) { /*m_vOrder.x = iOrder[0]; m_vOrder.y = iOrder[1]; m_vOrder.z = iOrder[2];*/ }
	//
	//#ifndef __IPHONE_TOUCH__
	//	void Set(const D3DXVECTOR3 &vVec);
	//#else
	//    void Set(double source_value_x,
	//             double source_value_y,
	//             double source_value_z,
	//             double magnetic_heading,
	//             double zx_rotation_angle);
	//#endif
	//
	//#ifndef __IPHONE_TOUCH__
	//	D3DXVECTOR3	& get()				{		return (*m_vStorage);	}
	//#endif
	//	const std::string	GetTypeName()		{		return "HiSFVec3f";		}
	//};
	//
	
	class  HI_DLLEXPORT HiVec3f : public HiField
	{
	public:
		float* m_pStorage;
		float m_vOffset[3];
		float m_vScale[3];
		int m_vOrder[3];
		
	public:
		HiVec3f(HiContainer *parent, std::string strName);
		~HiVec3f();
		
		// HiVec3f(HiContainer *parent, short id, std::string strName); //  HiObject??id?� 중복?�으�??�외...
		// HiVec3f(HiContainer *parent, short id, std::string strName, float *val); // chpark = ?�멸???�출 부분에??문제가 ?�을 ???�기 ?�문???�외?�켜 문제�?방�???		virtual ~HiVec3f();
		const std::string GetTypeName();	
		
		void Set(const float vVec[3]);
		const float* Get();
		
		void Copy(HiField *pField);
		
		void SetScale(float *fScale);
		void SetOffset(float *fOffset);
		void SetOrder(int *iOrder);
	};
	
}