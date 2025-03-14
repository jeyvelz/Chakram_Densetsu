#include "Object.h"
#include "Novice.h"
#include "ToScreen.h"

void DrawObject(Vector2 screenPos, float drawWidthHalf, float drawHeightHalf,
	int animePosX, int animePosY, int graph_, DIRECTION direction, int shakeX, int shakeY, unsigned int const color) {

	if (direction == RIGHT) {
		Novice::DrawQuad(static_cast<int>(screenPos.x - drawWidthHalf) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) - drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x + drawWidthHalf) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) - drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x - drawWidthHalf) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) + drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x + drawWidthHalf) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) + drawHeightHalf) + shakeY,
			animePosX, animePosY, static_cast<int>(drawWidthHalf * 2.f), static_cast<int>(drawHeightHalf * 2.f), graph_, color);
	} else {
		Novice::DrawQuad(static_cast<int>(screenPos.x - drawWidthHalf * -1.0f) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) - drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x + drawWidthHalf * -1.0f) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) - drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x - drawWidthHalf * -1.0f) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) + drawHeightHalf) + shakeY,
			static_cast<int>(screenPos.x + drawWidthHalf * -1.0f) + shakeX, static_cast<int>(ConvertToScreen(screenPos.y) + drawHeightHalf) + shakeY,
			animePosX, animePosY, static_cast<int>(drawWidthHalf * 2.f), static_cast<int>(drawHeightHalf * 2.f), graph_, color);
	}
}
