#include "Visualisation.h"
#include "Sprite.h"

// Default constructor
CVisualisation::CVisualisation(void) : m_screen(0)
{
}

// Default Destructor - Delete the sprites in the vector list and clear it
CVisualisation::~CVisualisation(void)
{
	// Delete the pointers to the sprite data
	while(!m_spriteVector.empty())
	{
		delete m_spriteVector.back();
		m_spriteVector.pop_back();
	}
	m_spriteVector.clear();
}

// Initialise the screen and set the screen width/height
bool CVisualisation::Initialise(int sWidth, int sHeight)
{
	// Set the member variables of width and height
	m_sWidth = sWidth;
	m_sHeight = sHeight;

	// Initialise HAPI (potentially altering the width and height values)
	if(!HAPI->Initialise(&m_sWidth, &m_sHeight))
		return false;

	// Change the used font size and... if it didn't work... yeah.
	if(!HAPI->ChangeFont("Arial", 20, 700))
		HAPI->UserMessage("Yeah, it didn't work Liam. Give it up.", "Too hard; didn't work.");
	
	// Set the screen pointer to be HAPI's Screen Pointer
	m_screen = HAPI->GetScreenPointer();

	return true;
}

// Clear the screen to a set colour
void CVisualisation::ClearScreen(HAPI_TColour colour)
{
	// Loop through via the offset from the top-left pixel
	for (int offset = 0; offset < (m_sWidth * m_sHeight) * 4; offset += 4)
	{
		// Set colour via memcpy
		memcpy(m_screen + offset, &colour, 4);
	}
}

// Load a new texture into a new sprite object and add it to the list if successful
bool CVisualisation::LoadTexture(std::string fileName, int *id, int &fWidth, int &fHeight, bool framed, bool anim, const DWORD tFrame)
{
	// Create new sprite
	CSprite *newSprite = new CSprite();

	// If the texture doesn't load, return false and delete the pointer
	if(!newSprite->LoadTexture(fileName, framed, fWidth, fHeight, anim, tFrame))
	{
		// Set ID to be -1, to check when drawing
		*id = -1;
		return false;
	}

	// Set the id to be the size of the list without the sprite added
	*id = m_spriteVector.size();

	// Add the sprite to the list
	m_spriteVector.push_back(newSprite);
	
	return true;
}

// Draw a texture to the screen, either with alpha&clipping or not
void CVisualisation::DrawTexture(int id, int posX, int posY, bool alpha)
{
	// If the ID is less than 0, the texture hasn't been loaded correctly
	if(id >= 0)
	{
		// If alpha flag is on, draw with alpha
		if(alpha)
			m_spriteVector[id]->DrawTexture(m_screen, posX, posY, m_sWidth, m_sHeight);
		// else draw without alpha or clipping
		else
			m_spriteVector[id]->DrawTextureNonAlpha(m_screen, posX, posY, m_sWidth, m_sHeight);
	}
}

// Draw texture with animation
void CVisualisation::DrawTextureAnim(int id, int posX, int posY)
{
	// If the ID is less than 0, the texture hasn't been loaded correctly
	if(id >= 0)
		m_spriteVector[id]->DrawTextureAnim(m_screen, posX, posY, m_sWidth, m_sHeight);
}

// Draw a vertical strip of animation from a larger image
void CVisualisation::DrawTextureXAnimStrip(int id, int posX, int posY, int currentFrameX)
{
	if(id >= 0)
		m_spriteVector[id]->DrawTextureXAnimStrip(m_screen, posX, posY, m_sWidth, m_sHeight, currentFrameX);
}

// Draw a horizontal strip of animation from a larger image
void CVisualisation::DrawTextureYAnimStrip(int id, int posX, int posY, int currentFrameY)
{
	if(id >= 0)
 		m_spriteVector[id]->DrawTextureYAnimStrip(m_screen, posX, posY, m_sWidth, m_sHeight, currentFrameY);
}

// Draw a scrolling background
void CVisualisation::DrawScrollingBackground(int id, int posY)
{
	if(id >= 0)
		m_spriteVector[id]->DrawScrollingBackground(m_screen, posY, m_sWidth, m_sHeight);
}

// Draw a health bar
void CVisualisation::DrawHealthBar(int id, int posX, int posY, int h)
{
	if(id >= 0)
		m_spriteVector[id]->DrawHealthBar(m_screen, posX, posY, m_sWidth, m_sHeight, h);
}

// Render text to a screen using the provided parameters
void CVisualisation::RenderText(int posX, int posY, HAPI_TColour col, std::string text)
{
	HAPI->RenderText(posX, posY, col, text);
}

// Set the animation frame of a given sprite
bool CVisualisation::SetSpriteAnimFrame(int id, int cX, int cY)
{
	if(cX >= 0 && cY >= 0)
	{
		return m_spriteVector[id]->SetCurrentFrame(cX, cY);
	}
	return false;
}

// Draw the collision rectangle of an entity (or any given rectangle within the bounds, no clipping)
void CVisualisation::DrawCollisionRectangle(CRectangle rect)
{
	// Make a new Rectangle to be equal to the given rectangle
	CRectangle newRect = rect;

	// Clip it against the bounds of the screen
	newRect.ClipAgainst(CRectangle(1, 1, m_sWidth - 1, m_sHeight - 1));

	// If the final rectangle collides with the given rectangle, draw it
	if(newRect.Collide(CRectangle(10, 10, m_sWidth - 10, m_sHeight - 10)))
	{
		// Point to the screen data and set the colour of the rectangle to be white
		BYTE* tempPtr = m_screen;
		HAPI_TColour col = HAPI_TColour(255, 255, 255);

		// Increment to the top left of the rectangle
		tempPtr += (newRect.left + (m_sWidth * newRect.top)) * 4;

		// Draw the top line of the rectangle
		memcpy(tempPtr, &col, (newRect.Width() + 1) * 4);

		// Increment to the next line
		tempPtr += m_sWidth * 4;

		// Loop through to draw the sides of the rectangle
		for(unsigned int i = 0; i < newRect.Height() - 1; i++)
		{
			memcpy(tempPtr, &col, 4);
			tempPtr += newRect.Width() * 4;
			memcpy(tempPtr, &col, 4);
			tempPtr += (m_sWidth - newRect.Width()) * 4;
		}
		
		// Draw the bottom line of the rectangle
		memcpy(tempPtr, &col, (newRect.Width() + 1) * 4);
	}
}