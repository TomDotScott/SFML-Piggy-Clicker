#include "UiButton.h"

#include "UiManager.h"
#include "../Globals.h"
#include "../TextureManager.h"
#include "../Input/Mouse.h"

UiButton::UiButton() :
	UiElement(eType::Button),
	m_sprite(nullptr)
{
	// Always render on top
	SetLayer(eLayer::FOREGROUND);
}

void UiButton::SetPosition(const sf::Vector2f& position)
{
	UiElement::SetPosition(position);

	m_sprite->SetPosition(position);
}

void UiButton::HandleMouse() const
{
	const Mouse& mouse = Mouse::Get();

	if (!mouse.IsButtonPressed(sf::Mouse::Button::Left))
	{
		return;
	}

	const sf::Vector2f mousePosition = static_cast<sf::Vector2f>(mouse.GetPosition());

	const sf::Vector2f bottomRight = m_position + m_sprite->GetSize();

	const bool inWidth = mousePosition.x >= m_position.x && mousePosition.x <= bottomRight.x;
	const bool inHeight = mousePosition.y >= m_position.y && mousePosition.y <= bottomRight.y;

	if (inWidth && inHeight)
	{
		printf("CLICKED\n");
	}

}

bool UiButton::ParseBeginElement(hoxml_context_t*& context)
{
	auto [xmlText, xmlLength] = UIMANAGER.GetLastXmlDetails();

	if (strcmp("Sprite", context->tag) == 0)
	{
		m_sprite = new UiSprite();
		if (!m_sprite->Load(context, xmlText, xmlLength))
		{
			printf(" UiPanel: Error loading sprite on line %u\n", context->line);
			return true;
		}
	}

	return UiElement::ParseBeginElement(context);
}

bool UiButton::ParseEndElement(hoxml_context_t*& context)
{
	if (!context)
	{
		return false;
	}

	if (context->tag && strcmp("Button", context->tag) == 0)
	{
		if (!m_sprite)
		{
			printf(" Closing tag found and no sprite was assigned to %s!\n", GetName().c_str());
			return false;
		}
		m_sprite->SetPosition(m_position);

		for (const sf::Drawable* const drawable : m_sprite->GetDrawablesList())
		{
			AddDrawable(drawable);
		}

		// When we have finished assigning the sprite, we can return true!
		return true;
	}

	if (context->content == nullptr || OnlyWhitespace(context->content))
	{
		return false;
	}

	return UiElement::ParseEndElement(context);
}
