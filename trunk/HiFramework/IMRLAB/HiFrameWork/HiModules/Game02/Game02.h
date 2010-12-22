#ifndef _GAME02_HEADER_
#define _GAME02_HEADER_

#include "HiKernel/HiModule.h"

class TileNode;

class HI_DLLEXPORT Game02 : public HiKernel::HiModule
{
protected :
	std::vector<TileNode*>	tile_array_;

	//TileNode* tile_array_[16];

	int row_, col_, time_, count_;

	bool node_active_;

public :
	Game02(HiKernel::HiXmlNode* xml = NULL);
	virtual ~Game02 ();

public :

	void PostConfig(); 
	void Update();
	void Realize();
	bool HandleMessage(const HiKernel::HiTelegram &msg);
	int  ParseXml(HiKernel::HiXmlNode *xml);

public :

	void ClearGame();

};

#endif