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

void UiButton::OnButtonPressed(const std::function<void()>& callback)
{
	m_pressedEvent.On(callback);
}

void UiButton::OnLeftClickPressed()
{
	m_pressedEvent.Fire();
}

sf::Vector2f UiButton::GetBottomRight() const
{
	return GetPosition() + GetSize();
}

sf::Vector2f UiButton::GetSize() const
{
	return m_sprite->GetSize();
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
