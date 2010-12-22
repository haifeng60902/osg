#ifndef _OSG_HUD_HEADER_
#define _OSG_HUD_HEADER_

#include "HiKernel/HiModule.h"

#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include <osg/MatrixTransform>
#include <osg/CullFace>
#include <osg/Light>
#include <osg/Notify>
#include <osg/ShadeModel>
#include <osg/LightModel>
#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Texture2D>
#include <osgUtil/GLObjectsVisitor>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <osg/PositionAttitudeTransform>


#include <osgCal/CoreModel>
#include <osgCal/Model>


namespace HiModules 
{
	struct s_CalCoreModel
	{
		//osgCal::Model* m_Model;
		std::string	m_CordId;
		int			m_CoreIndex;
		osg::ref_ptr<osgCal::CoreModel> m_CoreModel; 
		std::vector< std::string > animationNames;
		osg::ref_ptr<osgCal::MeshParameters> m_p;

		s_CalCoreModel() : m_CoreModel(new osgCal::CoreModel),
			m_p(new osgCal::MeshParameters)
		{}
	};
	struct s_CalInstanceModel
	{
		int m_index;
		osgCal::Model* m_Model;
		osg::ref_ptr<osgCal::BasicMeshAdder> m_meshAdder;
		osg::PositionAttitudeTransform* m_modelPos;
		osg::Vec3	m_Pos;
		osg::Vec4	m_Forword;

		osg::Vec4   m_Direction;
		osg::Vec4   m_BackVector;
		osg::Vec4   m_RightVector;
		osg::Vec4   m_LeftVector;
		osg::Vec4   m_UpVector;
		osg::Vec4   m_DownVector;

		osg::MatrixTransform* m_rotMat;

		s_CalInstanceModel() : m_modelPos(new osg::PositionAttitudeTransform),
			m_Model(new osgCal::Model),
			m_meshAdder(new osgCal::DefaultMeshAdder),
			m_rotMat(new osg::MatrixTransform)
		{}
	};

	class HI_DLLEXPORT mHiCal3dTest : public HiKernel::HiModule
	{
	protected :

//		HiOSGRenderer*						m_pOSGRender;


		// 캐릭터의 위치 이동 및 크기 회전 관련 멤버 변수
		osg::Group*	   m_pCalGroup;


	private:

	public :
		mHiCal3dTest(HiKernel::HiXmlNode* xml = NULL);
		virtual ~mHiCal3dTest ();

	public :

		std::string m_cfgFile;
		std::vector<s_CalCoreModel*>		m_calModel;

		typedef std::vector<s_CalInstanceModel*> CalInstance_array;
		CalInstance_array m_calChildModel;


		// 모델의 갯수
		int	m_ModelCount;

		osgCal::Model* makeModel( osgCal::CoreModel* cm,
			osgCal::BasicMeshAdder* ma,
			int animNum = -1) ;
		bool	addModel(int _id, float _x, float _y, float _z);

		void PostConfig(); 
		void Update();
		bool HandleMessage(const HiKernel::HiTelegram &msg);
		int  ParseXml(HiKernel::HiXmlNode *xml);

	public :

		osg::ref_ptr<osg::Group> m_pRoot;

	};

#endif

}