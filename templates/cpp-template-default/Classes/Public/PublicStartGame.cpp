#include "PublicStartGame.h"
#include "PublicBaseHelper.h"

#ifdef BLOCK_BLAST
#include "BlockBlast/BlockBlastStartScene.h"
#include "BlockBlast/BlockBlastStructor.h"
#include "BlockBlast/BlockBlastAchievement.h"
#include "PublicStatistic.h"
#endif

void PublicStartGame(const std::string& gameName)
{
	if ("BLOCK_BLAST" == gameName)
	{
#ifdef BLOCK_BLAST
		PublicInitGame("BlockBlast");
		PublicGameStatistic::getInstance()->init("config/BlockBlastStatistic.json");
		BlockBlastDataManage::getInstance()->init();
		BlockBlastAchievementManager::getInstance()->init();
		PublicPreloadEffect(std::vector<std::string>{});
		PublicLoadSpriteFrameData("blockblastbase.plist");

		auto scene = BlockBlastStartScene::create();
		cocos2d::Director::getInstance()->replaceScene(scene);
#endif
	}
}