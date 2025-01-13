#include "ToScreen.h"
#include "Define.h"

float ConvertToScreen(const float& world) {
	// 今回のワールド座標系からスクリーン座標系は
	// 原点位置がyにマップの高さ分ずれていて、y軸が反転
	const float kWorldToScreenTranslate = kRealMapHeight;
	const float kWorldToScreenScale = -1.0f;
	return { (world * kWorldToScreenScale) + kWorldToScreenTranslate };
}