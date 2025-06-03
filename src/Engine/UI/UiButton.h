#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "UiElement.h"
#include "UiSprite.h"

class UiButton : public UiElement
{
public:
	UiButton();

	void SetPosition(const sf::Vector2f& position) override;

	// TODO: This SUCKS! Make a proper event system and make the UI be event driven
	// Buttons can subscribe to left click events, and have a list of callbacks to call if they have been clicked
	// This will do for now to get things up and running!
	void HandleMouse() const;

private:
	UiSprite* m_sprite;

	bool ParseBeginElement(hoxml_context_t*& context) override;
	bool ParseEndElement(hoxml_context_t*& context) override;
};

#endif