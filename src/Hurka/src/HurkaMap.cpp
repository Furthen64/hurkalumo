#include "HurkaMap.hpp"



// (--)
HurkaMap::HurkaMap(std::string _mapName, TextureManager *_textMgr)
{
    textMgr = _textMgr;
    mapName = _mapName;
}




void HurkaMap::testList()
{
    //http://thispointer.com/stdlist-tutorial-and-usage-details/

    std::cout << "Här var du sesnast 2018-02-25, och vid core\n";

    std::cout << "\n";
    std::cout << "TestList()\n";
    std::cout << "\n";

    std::list<Block> blockList;

    Block house001 ({0,1}, "HOUSE001", textMgr);
    blockList.push_back(house001);

    Block tree001( {0,0}, "TREE001", textMgr);
    blockList.push_back(tree001);



    //
/*    std::list<Block>::iterator it = blockList.begin();
    it = blockList.begin();


    it++;
    it++;*/



    // iterate over all items
     for (std::list<Block>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
     {
         int iddd = (*itAll).getTextureID();
         std::cout << "TextureID of current block: " << iddd << "\n";

     }


}

