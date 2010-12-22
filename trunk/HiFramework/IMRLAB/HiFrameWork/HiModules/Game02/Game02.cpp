#include "HiModules/Game02/Game02.h"
#include "HiModules/TileNode.h"
#include "HiKernel/HiOSGRenderer.h"
#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiXmlNode.h"

#include <QuartzCore/QuartzCore.h>

#include "HiKernel/HiEventManager.h"
#include "HiModules/enumerations.h"

bool is_step2 = false;
bool game_clear = false;
int touch_count_ = 0;
std::vector<int> index_array_;
std::vector<int>::iterator itr;

class PickHandler2 : public osgGA::GUIEventHandler
{
public:
	PickHandler2() {};
	bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::PUSH):
			{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				if (view) pick(view,ea);
				return false;
			}
			default:
				return false;
		}
	}

protected:
	void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
	{
		osgUtil::LineSegmentIntersector::Intersections intersections;

		float x = ea.getX();
		float y = 1024 - ea.getY();
		osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
		osgUtil::IntersectionVisitor iv(picker.get());
		view->getCamera()->accept(iv);
		if (picker->containsIntersections() && is_step2)
		{
			intersections = picker->getIntersections();

			const osg::NodePath& nodepath = picker->getFirstIntersection().nodePath;
			TileNode* tile = static_cast<TileNode*>(nodepath.back());
			if(tile->GetID() == *itr)
			{
				tile->Activate(true, false);
				itr++;
				touch_count_++;
				HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
													"OsgSound",
													"OsgSound",
													HiModules::GOOD_EFFECT_SOUND,
													0);
			}
			else
			{
				game_clear = true;
				HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
													"OsgSound",
													"OsgSound",
													HiModules::BAD_EFFECT_SOUND,
													0);
			}
		}
	}
};


Game02::Game02(HiKernel::HiXmlNode *xml)
{
	if(xml != NULL)
		ParseXml(xml);	
}
Game02::~Game02()
{

}

bool Game02::HandleMessage(const HiKernel::HiTelegram &msg)
{
	//HiModule *pSender = ModuleManager->FindModule(msg.Sender);

	//switch ( msg.Msg )
	//{

	//}

	return HiKernel::ModuleManager->DefHandleMessage(msg,this);
}

void Game02::PostConfig()
{
	osg::Group* root = new osg::Group(); 

	for(int i = 0 ; i < (row_ * col_) ; i++)
	{
		TileNode* tile = new TileNode();
		root->addChild(tile->Init(i, row_, col_));
		tile_array_.push_back(tile);
	}

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); 
	HiKernel::OSGRenderer->getViewer()->setSceneData(root);
	HiKernel::OSGRenderer->getViewer()->addEventHandler(new PickHandler2());
	HiKernel::OSGRenderer->getViewer()->setCameraManipulator(new osgGA::TrackballManipulator()); 
	
	HiKernel::OSGRenderer->getViewer()->realize();
	
	HiKernel::OSGRenderer->getViewer()->getCamera()->setClearColor(osg::Vec4(197.0 / 255.0 ,165.0 / 255.0 , 137.0 / 255.0,1.0)); // r, g, b, a

	index_array_.clear();

	srand(time(NULL));
	while(index_array_.size() < count_)
	{
		int index = rand()%(row_*col_);
		bool check = true;
		for(std::vector<int>::iterator i = index_array_.begin() ; 
			i != index_array_.end() ; i++)
		{
			if(*i == index) {
				check = false;
				break;
			}
		}
		if(check)
			index_array_.push_back(index);
	}
	node_active_ = false;

	itr = index_array_.begin();

}
void Game02::Update()
{
#ifdef HI_IOS4
	CFTimeInterval dt;
#else
	double dt;
#endif
	static double active_start;
	Realize();
	
	if(count_ <= touch_count_)
	{
		count_ += 2;
		touch_count_ = 0;
		ClearGame();
	}

	if(game_clear)
		ClearGame();

	if(!is_step2)
	{
		if(!node_active_)
		{
			node_active_ = true;
#ifdef HI_IOS4
			active_start = CACurrentMediaTime();
#else
			active_start = timeGetTime();
#endif
			tile_array_[*itr]->Activate(true, false);
			itr++;
		}
#ifdef HI_IOS4
		dt = CACurrentMediaTime() - active_start;
#else
		dt = timeGetTime() - active_start;
#endif
		if(time_ < dt)
		{
			node_active_ = false;
			if(itr == index_array_.end())
			{
				for(itr = index_array_.begin() ; itr != index_array_.end() ; itr++)
				{
					tile_array_[*itr]->Activate(false, false);
				}
				itr = index_array_.begin();
				is_step2 = true;
			}
		}
	}

}
void Game02::Realize()
{

}

int Game02::ParseXml(HiKernel::HiXmlNode *xml)
{

	HiKernel::HiXmlNode* node = xml->FindChild("Size");

	node->GetAttrs("row", 1, &row_);
	node->GetAttrs("col", 1, &col_);

	node = xml->FindChild("Detail");

	node->GetAttrs("time", 1, &time_);
	node->GetAttrs("count", 1, &count_);

	return 0;
}


void Game02::ClearGame()
{
	for(itr = index_array_.begin() ; itr != index_array_.end() ; itr++)
	{
		tile_array_[*itr]->Activate(false, false);
	}
	index_array_.clear();

	srand(time(NULL));
	while(index_array_.size() < count_)
	{
		int index = rand()%(row_*col_);
		bool check = true;
		for(std::vector<int>::iterator i = index_array_.begin() ; 
			i != index_array_.end() ; i++)
		{
			if(*i == index) {
				check = false;
				break;
			}
		}
		if(check)
			index_array_.push_back(index);
	}
	node_active_ = false;
	is_step2 = false;

	itr = index_array_.begin();

	game_clear = false;
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT Game02* HiLoadModule_Game02(HiXmlNode* xml)
{
	Game02 *m = new Game02(xml);
	return m;
}

#endif