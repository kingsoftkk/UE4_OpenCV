/*
* VisionHUD: Heads-Up Display class for machine vision projects with UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "VisionHUD.h"

AVisionHUD::AVisionHUD()
{
	// Get Minimap render target from blueprint
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapTexObj(RENDER_TARGET_NAME);
	MiniMapTextureRenderTarget = MiniMapTexObj.Object;

	// Creates Texture2D to store MiniMapTex content
	// NB: This should match the Render Target Format specified in the T_Minimap blueprint
	MiniMapTexture = UTexture2D::CreateTransient(MiniMapTextureRenderTarget->SizeX, MiniMapTextureRenderTarget->SizeY, RENDER_TARGET_PIXEL_FORMAT);

#if WITH_EDITORONLY_DATA
	MiniMapTexture->MipGenSettings = TMGS_NoMipmaps;
#endif
	MiniMapTexture->SRGB = MiniMapTextureRenderTarget->SRGB;

	MiniMapRenderTarget = MiniMapTextureRenderTarget->GameThread_GetRenderTargetResource();
}

void AVisionHUD::DrawHUD()
{

	Super::DrawHUD();	

	// Adapted from https://answers.unrealengine.com/questions/193827/how-to-get-texture-pixels-using-utexturerendertarg.html

	// Read the pixels from the RenderTarget and store them in a FColor array
	MiniMapRenderTarget->ReadPixels(MiniMapSurfData);

	int rows = MiniMapTextureRenderTarget->SizeY;
	int cols = MiniMapTextureRenderTarget->SizeX;

	for (int x = 0; x < cols; ++x) {

		for (int y = 0; y < rows; ++y) {

			int k = x + y * cols;

			FColor PixelColor = MiniMapSurfData[k];

			DrawRect(PixelColor, LEFTX+x, TOPY+y, 1, 1);

			// RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
			//imgdata[k] = (byte)(0.21 *PixelColor.R + 0.72 * PixelColor.G + 0.07 * PixelColor.B);
		}
	}

	float rightx = LEFTX + cols;
	float bottomy = TOPY + rows;

	drawBorder(LEFTX, TOPY, rightx, TOPY);
	drawBorder(rightx, TOPY, rightx, bottomy);
	drawBorder(rightx, bottomy, LEFTX, bottomy);
	drawBorder(LEFTX, bottomy, LEFTX, TOPY);
}

void AVisionHUD::drawBorder(float lx, float uy, float rx, float by)
{
	DrawLine(lx, uy, rx, by, BORDER_COLOR, BORDER_WIDTH);
}
