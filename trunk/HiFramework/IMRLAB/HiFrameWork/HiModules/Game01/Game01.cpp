#include <osgViewer/api/iPhone/GraphicsWindowiPhone>
#include "HiModules/Game01/Game01.h"
#include "HiModules/TileNode.h"
#include "HiKernel/HiOSGRenderer.h"
#include "HiKernel/HiModuleManager.h"
#include "HiKernel/HiXmlNode.h"

#include "HiKernel/HiEventManager.h"
#include "HiModules/enumerations.h"

bool move_tile_activated = false;

class PickHandler : public osgGA::GUIEventHandler
{
public:
	PickHandler() {};
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
//        float x = ea.getXmin() + (ea.getXmax()-ea.getXmin())*x/768;
//        float y = ea.getYmin() + (ea.getYmax()-ea.getYmin())*y/1024;
		osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
		osgUtil::IntersectionVisitor iv(picker.get());
		view->getCamera()->accept(iv);
		if (picker->containsIntersections())
		{
			intersections = picker->getIntersections();

			const osg::NodePath& nodepath = picker->getFirstIntersection().nodePath;
			TileNode* tile = static_cast<TileNode*>(nodepath.back());
			move_tile_activated = tile->InActivate();
			if(move_tile_activated)
			{
				HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
													"OsgSound",
													"OsgSound",
													HiModules::BAD_EFFECT_SOUND,
													0);
			}
			else {
				HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
													"OsgSound",
													"OsgSound",
													HiModules::GOOD_EFFECT_SOUND,
													0);
			}

		}
		else {
			HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
									  "OsgSound",
									  "OsgSound",
									  HiModules::BAD_EFFECT_SOUND,
									  0);
		}

	}
};


Game01::Game01(HiKernel::HiXmlNode *xml)
{
	row_ = 0; col_ = 0;
	if(xml != NULL)
		ParseXml(xml);	
}
Game01::~Game01()
{

}

bool Game01::HandleMessage(const HiKernel::HiTelegram &msg)
{
	//HiModule *pSender = ModuleManager->FindModule(msg.Sender);

	//switch ( msg.Msg )
	//{

	//}

	return HiKernel::ModuleManager->DefHandleMessage(msg,this);

}

void Game01::PostConfig()
{
	move_tile_activated = false;

	osg::Group* root = new osg::Group();

	for(int i = 0 ; i < (row_ * col_) ; i++)
	{
		TileNode* tile = new TileNode();
		root->addChild(tile->Init(i, row_, col_));
		tile_array_.push_back(tile);
	}
	move_tile_node_ = new TileNode();
	root->addChild(move_tile_node_->Init(tile_array_[0]->GetPosition()));
	move_tile_node_->SetWidth(tile_array_[0]->GetWidth());
	move_tile_node_->SetHeight(tile_array_[0]->GetHeight());

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF); 
	HiKernel::OSGRenderer->getViewer()->setSceneData(root);
	HiKernel::OSGRenderer->getViewer()->addEventHandler(new PickHandler());
	HiKernel::OSGRenderer->getViewer()->setCameraManipulator(new osgGA::TrackballManipulator()); 
	
	HiKernel::OSGRenderer->getViewer()->getCamera()->setClearColor(osg::Vec4(197.0 / 255.0 ,165.0 / 255.0 , 137.0 / 255.0,1.0)); // r, g, b, a

	HiKernel::OSGRenderer->getViewer()->realize();


}
void Game01::Update()
{

	Realize();
	
	if(!move_tile_node_->IsMoved() && move_tile_activated)
		move_tile_activated = false;

	for(int i = 0 ; i < (row_ * col_) ; i++)
	{
		//if(rand()%800 < 1)
		//{
		//	tile_array_[i]->Activate(true, false);
		//}
		tile_array_[i]->Update();
	}
	if(rand()%100 < 1 && !move_tile_activated)
	{
		move_tile_activated = true;
		int num;
		while(1)
		{
			num = rand()%(row_ * col_);
			if(num < row_ || num > (row_ * col_) - (row_-1) ||
				num%row_ == row_-1 ||
				num%row_ == 0)
				break;
		}
		if(num == 0)
		{
			if(rand()%2 < 1)
				move_tile_node_->SetMovement(RIGHT);
			else
				move_tile_node_->SetMovement(DOWN);
		}
		else if(num == row_ - 1)
		{
			if(rand()%2 < 1)
				move_tile_node_->SetMovement(LEFT);
			else
				move_tile_node_->SetMovement(DOWN);
		}
		else if(num == (row_ * col_) - row_)
		{
			if(rand()%2 < 1)
				move_tile_node_->SetMovement(RIGHT);
			else
				move_tile_node_->SetMovement(UP);
		}
		else if(num == (row_ * col_) - 1)
		{
			if(rand()%2 < 1)
				move_tile_node_->SetMovement(LEFT);
			else
				move_tile_node_->SetMovement(UP);
		}
		else if(num < row_)
		{
			move_tile_node_->SetMovement(DOWN);
		}
		else if(num > (row_ * col_) - (row_-1))
		{
			move_tile_node_->SetMovement(UP);
		}
		else if(num%row_ == row_-1)
		{
			move_tile_node_->SetMovement(LEFT);
		}
		else if(num%row_ == 0)
		{
			move_tile_node_->SetMovement(RIGHT);
		}
		move_tile_node_->SetPosition(tile_array_[num]->GetPosition());
		move_tile_node_->Drawable(true);
	}
	if(move_tile_activated)
	{
		float tx, tz;
		move_tile_node_->Update();
		tx = move_tile_node_->GetX();
		tz = move_tile_node_->GetZ();
		if(tx < -25 || tx > 25 || tz < -25 || tz > 25) {
			move_tile_activated = move_tile_node_->InActivate();
			HiKernel::Dispatch->GiveMessage(HiKernel::SEND_MSG_IMMEDIATELY,
												"OsgSound",
												"OsgSound",
												HiModules::BAD_EFFECT_SOUND,
												0);
		}
	}

}
void Game01::Realize()
{

}

int Game01::ParseXml(HiKernel::HiXmlNode *xml)
{
	HiKernel::HiXmlNode* node = xml->FindChild("Size");

	node->GetAttrs("row", 1, &row_);
	node->GetAttrs("col", 1, &col_);

	return 0;
}

#ifndef HI_IOS4

extern "C" 
HI_DLLEXPORT Game01* HiLoadModule_Game01(HiXmlNode* xml)
{
	Game01 *m = new Game01(xml);
	return m;
}

#endif