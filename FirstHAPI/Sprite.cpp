#include "Sprite.h"

// Default Constructor
CSprite::CSprite(void)
{
}

// Default Destructor - deletes the image data
CSprite::~CSprite(void)
{
	delete [] m_data;
}

// Attempts to Load a texture into the sprite. If sprite isn't loaded, program will send out
// an error message and will end.
bool CSprite::LoadTexture(std::string fileName, bool framed, int &fWidth, int &fHeight, bool anim, const DWORD tFrame)
{
	// Use HAPI's Load Texture to attempt to load a texture
	if(!HAPI->LoadTexture(fileName, &m_data, &m_width, &m_height))
	{
		HAPI->UserMessage("Unable to load file by the name of: " + fileName +
							"\n Texture will not draw.", "Texture Not Loaded");
		return false;
	}

	// If texture has been loaded correctly, all data is initialised based on the output.
	m_sourceRect = CRectangle(0, 0, m_width, m_height);
	m_animation = anim;
	m_currentFrameX = 0; m_currentFrameY = 0;

	m_timeBetweenFrames = tFrame;
	m_time = HAPI->GetTime() + m_timeBetweenFrames;

	// If the framed flag was on, the image is used for animation
	if(framed)
	{
		m_frameWidth = fWidth; m_frameHeight = fHeight;
		m_frameRect = CRectangle(0, 0, m_frameWidth, m_frameHeight);
	}
	// Else use the source rectangle as the frame rectangle
	else
	{
		m_frameRect = m_sourceRect;
		m_frameWidth = m_width;
		m_frameHeight = m_height;
		fWidth = m_width;
		fHeight = m_height;
	}

	return true;
}

// Quickest method to draw a texture, can only be used for textures that will cover the entire width of a screen and won't be clipped
void CSprite::DrawTextureNonAlpha(BYTE* dest, int posX, int posY, int destWidth, int destHeight)
{
	BYTE* dstPtr = dest;

	dstPtr += (posX + (posY * destWidth)) * 4;

	memcpy(dstPtr, m_data, m_width*m_height*4);
}

// Draw the texture based on it being a single sprite's image
void CSprite::DrawTexture(BYTE* dest, int posX, int posY, int destWidth, int destHeight)
{
	m_sourceRect.Translate(posX - m_sourceRect.left, posY - m_sourceRect.top);

	// If the object is off the screen, it is not drawn, therefore the method leaves
	if (m_sourceRect.right < 0 || m_sourceRect.left >= destWidth ||
		m_sourceRect.bottom < 0 || m_sourceRect.top >= destHeight)
		return;

	// Declare pointers to point at screen and source
	BYTE* destPtr = dest;
	BYTE* srcPtr = m_data;

	// Generate the values of clip
	int clipX, clipY;
	if (m_sourceRect.left < 0)
		clipX = m_sourceRect.left;
	else if (m_sourceRect.right > destWidth)
		clipX = destWidth - m_sourceRect.left;
	else
		clipX = 0;

	if (m_sourceRect.top < 0)
		clipY = m_sourceRect.top;
	else if (m_sourceRect.bottom > destHeight)
		clipY = destHeight - m_sourceRect.top;
	else
		clipY = 0;
	
	// If the texture is clipped to the left of the screen
	if (clipX < 0)
	{
		// If the texture is clipped above the screen
		if (clipY < 0)
		{
			srcPtr += (-clipX + -clipY * m_width) * 4;
			for (int y = 0; y < clipY + m_height; y++)
			{
				for (int x = 0; x < clipX + m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX) * 4;
				destPtr += (destWidth - (m_width + clipX)) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			srcPtr += -clipX * 4;
			destPtr += m_sourceRect.top * destWidth * 4;
			for (int y = 0; y < m_height; y++)
			{
				for (int x = 0; x < clipX + m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX) * 4;
				destPtr += (destWidth - (m_width + clipX)) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			srcPtr += -clipX * 4;
			destPtr += (destHeight - clipY) * destWidth * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < clipX + m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX) * 4;
				destPtr += (destWidth - (m_width + clipX)) * 4;
			}
		}
	}
	// If the texture is to in the middle of the screen
	else if (clipX == 0)
	{
		// If the texture is clipped above the screen
		if (clipY < 0)
		{
			srcPtr += -clipY * m_width * 4;
			destPtr += m_sourceRect.left * 4;
			for (int y = 0; y < clipY + m_height; y++)
			{
				for (int x = 0; x < m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				destPtr += (destWidth - m_width) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			destPtr += (m_sourceRect.left + m_sourceRect.top * destWidth) * 4;
			for (int y = 0; y < m_height; y++)
			{
				for (int x = 0; x < m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				destPtr += (destWidth - m_width) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			destPtr += (m_sourceRect.left + m_sourceRect.top * destWidth) * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < m_width; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				destPtr += (destWidth - m_width) * 4;
			}
		}
	}
	// If the texture is clipped to the right of the screen
	else if (clipX > 0)
	{
		if (clipY < 0)
		{
			srcPtr += -clipY * m_width * 4;
			destPtr += m_sourceRect.left * 4;
			for (int y = 0; y < clipY + m_height; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_width - clipX) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			destPtr += (m_sourceRect.left + m_sourceRect.top * destWidth) * 4;
			for (int y = 0; y < m_height; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_width - clipX) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			destPtr += (m_sourceRect.left + m_sourceRect.top * destWidth) * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_width - clipX) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
	}
}

// Draw the texture based on it being an animated sprite's image
void CSprite::DrawTextureAnim(BYTE* dest, int posX, int posY, int destWidth, int destHeight)
{
	// Translate the image to the correct position
	m_frameRect.Translate(posX - m_frameRect.left, posY - m_frameRect.top);

	// If the object is off the screen, it is not drawn, therefore the method leaves
	if (m_frameRect.right < 0 || m_frameRect.left >= destWidth ||
		m_frameRect.bottom < 0 || m_frameRect.top >= destHeight)
		return;

	// Declare pointers to point at screen and source
	BYTE* destPtr = dest;
	BYTE* srcPtr = m_data;

	// Generate the values of clip
	int clipX, clipY;
	if (m_frameRect.left < 0)
		clipX = m_frameRect.left;
	else if (m_frameRect.right > destWidth)
		clipX = destWidth - m_frameRect.left;
	else
		clipX = 0;

	if (m_frameRect.top < 0)
		clipY = m_frameRect.top;
	else if (m_frameRect.bottom > destHeight)
		clipY = destHeight - m_frameRect.top;
	else
		clipY = 0;
	
	// Increment source pointer to correct frame image of animation
	srcPtr += ((m_currentFrameX * m_frameWidth) + (m_currentFrameY * m_frameHeight * m_width)) * 4;

	// If the texture is clipped to the left of the screen
	if (clipX < 0)
	{
		// If the texture is clipped above the screen
		if (clipY < 0)
		{
			srcPtr += (-clipX + -clipY * m_width) * 4;
			for (int y = 0; y < clipY + m_frameHeight; y++)
			{
				for (int x = 0; x < clipX + m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - (m_frameWidth + clipX)) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			srcPtr += -clipX * 4;
			destPtr += m_frameRect.top * destWidth * 4;
			for (int y = 0; y < m_frameHeight; y++)
			{
				for (int x = 0; x < clipX + m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - (m_frameWidth + clipX)) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			srcPtr += -clipX * 4;
			destPtr += (destHeight - clipY) * destWidth * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < clipX + m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (-clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - (m_frameWidth + clipX)) * 4;
			}
		}
	}
	// If the texture is to in the middle of the screen
	else if (clipX == 0)
	{
		// If the texture is clipped above the screen
		if (clipY < 0)
		{
			srcPtr += -clipY * m_width * 4;
			destPtr += m_frameRect.left * 4;
			for (int y = 0; y < clipY + m_frameHeight; y++)
			{
				for (int x = 0; x < m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (- m_frameWidth + m_width) * 4;
				destPtr += (destWidth - m_frameWidth) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			destPtr += (m_frameRect.left + m_frameRect.top * destWidth) * 4;
			for (int y = 0; y < m_frameHeight; y++)
			{
				for (int x = 0; x < m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (- m_frameWidth + m_width) * 4;
				destPtr += (destWidth - m_frameWidth) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			destPtr += (m_frameRect.left + m_frameRect.top * destWidth) * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < m_frameWidth; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (- m_frameWidth + m_width) * 4;
				destPtr += (destWidth - m_frameWidth) * 4;
			}
		}
	}
	// If the texture is clipped to the right of the screen
	else if (clipX > 0)
	{
		if (clipY < 0)
		{
			srcPtr += -clipY * m_width * 4;
			destPtr += m_frameRect.left * 4;
			for (int y = 0; y < clipY + m_frameHeight; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_frameWidth - clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
		// If the texture isn't clipped in the Y-axis
		else if (clipY == 0)
		{
			destPtr += (m_frameRect.left + m_frameRect.top * destWidth) * 4;
			for (int y = 0; y < m_frameHeight; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_frameWidth - clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
		// If the texture is clipped below the screen
		else if (clipY > 0)
		{
			destPtr += (m_frameRect.left + m_frameRect.top * destWidth) * 4;
			for (int y = 0; y < clipY; y++)
			{
				for (int x = 0; x < clipX; x++)
				{
					// Take out the texture colours and alpha value for the pixel
					BYTE b = srcPtr[0];
					BYTE g = srcPtr[1];
					BYTE r = srcPtr[2];
					float alpha = srcPtr[3];

					// Take out the screen's colour at this position
					BYTE destB = destPtr[0];
					BYTE destG = destPtr[1];
					BYTE destR = destPtr[2];

					// Normalize the alpha value
					alpha /= 255.0f;

					// Set the resultant RGB values
					BYTE resultB = (BYTE)((alpha * b) + ((1.0f - alpha) * destB));
					BYTE resultG = (BYTE)((alpha * g) + ((1.0f - alpha) * destG));
					BYTE resultR = (BYTE)((alpha * r) + ((1.0f - alpha) * destR));

					// Set up the colour variable
					HAPI_TColour result(resultR, resultG, resultB);

					// Copy the resultant colour into the screen
					memcpy(destPtr, &result, 4);

					// Increment both pointers
					destPtr += 4;
					srcPtr += 4;

				}
				// Increment pointers to next line
				srcPtr += (m_frameWidth - clipX - m_frameWidth + m_width) * 4;
				destPtr += (destWidth - clipX) * 4;
			}
		}
	}

	// Check if Sprite is animated and change frame if timer is greater than target time
	if(m_animation && HAPI->GetTime() >= m_time)
		RunAnimation();

}

// Draw the texture based on it being an animated sprite's image going through Y values with a single X value
void CSprite::DrawTextureXAnimStrip(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int currentFrameX)
{
	m_currentFrameX = currentFrameX;
	DrawTextureAnim(dest, posX, posY, destWidth, destHeight);
}

// Draw the texture based on it being an animated sprite's image going through X values with a single Y value
void CSprite::DrawTextureYAnimStrip(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int currentFrameY)
{
	m_currentFrameY = currentFrameY;
	DrawTextureAnim(dest, posX, posY, destWidth, destHeight);
}

// Draw the texture based on it being a scrolling background
void CSprite::DrawScrollingBackground(BYTE* dest, int posY, int destWidth, int destHeight)
{
	// The position cannot go over 600, as that's the edge of the play-screen
	int currentPosY = posY % 600;

	// The background rectangle is the source rectangle, clipped against the area available to draw it
	CRectangle bgRect = m_sourceRect;
	bgRect.ClipAgainst(CRectangle(0, 0, destWidth, destHeight - 128));

	// The background is then split into 2 rectangles, so each part draws part of the image
	CRectangle newRect1(0, 0, destWidth, bgRect.bottom - currentPosY);
	CRectangle newRect2(0, newRect1.bottom, destWidth, bgRect.bottom);

	// Point to the destination and source data without altering those pointers
	BYTE* dstPtr = dest;
	BYTE* srcPtr = m_data;

	// increment the source pointer to the current position
	srcPtr += m_width * currentPosY * 4;

	// Apply the first rectangle's texture
	memcpy(dstPtr, srcPtr, newRect1.Width() * newRect1.Height() * 4);

	// Increment the destination pointer to the next rectangle
	// and the source pointer to be set to the start of the data again
	dstPtr += newRect1.Width() * newRect1.Height() * 4;
	srcPtr = m_data;

	// Apply the second rectangle's texture
	memcpy(dstPtr, srcPtr, newRect2.Width() * newRect2.Height() * 4);
	
}

// Draw the texture based on it being a health bar
void CSprite::DrawHealthBar(BYTE* dest, int posX, int posY, int destWidth, int destHeight, int h)
{
	// Using the passed in health value, work out the percentage of the texture to draw
	float health = h / 100.0f;

	// Set the new width of the texture that will be drawn
	int width = (int)(m_width * health);

	// Point to the data without altering the base pointers
	BYTE* dstPtr = dest;
	BYTE* srcPtr = m_data;

	// Increment the destination pointer to the position the health bar will be drawn
	dstPtr += (posX + (posY * destWidth)) * 4;

	// Loop through drawing each horizontal line of pixels
	for(int i = 0; i < m_height; i++)
	{
		memcpy(dstPtr, srcPtr, width * 4);
		dstPtr += destWidth * 4;
		srcPtr += m_width * 4;
	}
}

// Private method to move to the next frame
void CSprite::RunAnimation()
{
	// Check if the frame would go off the source image in the X dimension
	if((m_currentFrameX + 1) * m_frameWidth >= m_width)
	{
		// Check if the frame would go off the source image in the Y dimension
		if((m_currentFrameY + 1) * m_frameHeight >= m_height)
		{
			// Reset frame indexes to 0
			m_currentFrameX = 0;
			m_currentFrameY = 0;
		}
		else
		{
			// Reset X to 0 but increment Y index
			m_currentFrameX = 0;
			m_currentFrameY++;
		}
	}
	else
	{
		// Increment X index
		m_currentFrameX++;
	}

	// Reset timer
	m_time = HAPI->GetTime() + m_timeBetweenFrames;
}

// Method to physically set the current frame of the image
// e.g. for a sprite-sheet of a character facing in different directions
bool CSprite::SetCurrentFrame(int currentX, int currentY)
{
	if((currentX + 1) * m_frameWidth >= m_width && (currentY + 1) * m_frameHeight >= m_height)
		return false;
	else
	{
		m_currentFrameX = currentX;
		m_currentFrameY = currentY;
		return true;
	}
}