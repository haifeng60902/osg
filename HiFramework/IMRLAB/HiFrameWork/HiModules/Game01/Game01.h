#ifndef _GAME01_HEADER_
#define _GAME01_HEADER_

#include "HiKernel/HiModule.h"

class TileNode;

class HI_DLLEXPORT Game01 : public HiKernel::HiModule
{
protected :
	std::vector<TileNode*> tile_array_;
	//TileNode* tile_array_[16];
	TileNode* move_tile_node_;

	int row_, col_;

public :
	Game01(HiKernel::HiXmlNode* xml = NULL);
	virtual ~Game01 ();

public :

	void PostConfig(); 
	void Update();
	void Realize();
	bool HandleMessage(const HiKernel::HiTelegram& msg);
	int  ParseXml(HiKernel::HiXmlNode *xml);

public :

};

#endif