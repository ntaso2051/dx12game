#include "Food.h"
#include "Game.h"
#include "ParameterComponent.h"
#include "Hero.h"
#include "SpriteComponent.h"
#include "ImguiWrapper.h"

Food::Food(Game* game, XMFLOAT3 pos, Const::TexId::Type texId) :Item(game, pos, texId) {
	mName = u8"ÇœÇÒ";
	mType = Item::Type::Once;
	mDetails = u8"Ç‹ÇÒÇ’Ç≠Ç«Ç™Ç©Ç¢Ç”Ç≠Ç∑ÇÈ";
}

Food::Food(Game* game) : Item(game) {
	mName = u8"ÇœÇÒ";
	mType = Item::Type::Once;
	mDetails = u8"Ç‹ÇÒÇ’Ç≠Ç«Ç™Ç©Ç¢Ç”Ç≠Ç∑ÇÈ";
}



Food::~Food() {}

void Food::Adapt() {
	ParameterComponent* pc = static_cast<ParameterComponent*>(mGame->GetHero()->GetComponent("ParameterComponent"));
	pc->SetHunger((pc->GetHunger() + 50 > 100) ? (100) : (pc->GetHunger() + 50));
	mGame->GetImgui()->Cout(u8"Ç®Ç»Ç©Ç™Ç¢Ç¡ÇœÇ¢Ç…Ç»Ç¡ÇΩÅI");
	Remove();
}

void Food::Deadapt() {

}

void Food::Remove() {
	Item::Remove();
	Deadapt();
}