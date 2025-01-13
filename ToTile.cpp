#include "ToTile.h"

/// <summary>
/// Xタイルへの変換関数
/// </summary>
/// <param name="posX">xの位置</param>
/// <returns>タイルの位置</returns>
int BlockX(float posX) {
	return static_cast<int>(posX) / kBlockSize;
}

/// <summary>
/// Yタイルへの変換関数
/// </summary>
/// <param name="posY">yの位置</param>
/// <returns>タイルの位置</returns>
int BlockY(float posY) {
	return static_cast<int>(ConvertToScreen(posY)) / kBlockSize;
}
