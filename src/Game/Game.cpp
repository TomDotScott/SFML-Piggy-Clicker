#include "Game.h"
#include <fstream>
#include <set>
#include <SFML/Graphics.hpp>

#include "../Engine/TextureManager.h"
#include "../Engine/Globals.h"
#include "../Engine/Timer.h"
#include "../Libs/hoxml.h"

float Game::Boundary::m_Damping = 0.8f;

Game::Game() :
	// TODO: Make this data be read from a file, I am fed up of editing stuff in C++ files
	m_boundaries{
		{
			TRANSFORMED_VECTOR(sf::Vector2f(424.f, 84.f)), TRANSFORMED_VECTOR(sf::Vector2f(455.f, 696.f)),
			VECTOR2F_RIGHT
		},
		{
			TRANSFORMED_VECTOR(sf::Vector2f(825.f, 84.f)), TRANSFORMED_VECTOR(sf::Vector2f(856.f, 696.f)), VECTOR2F_LEFT
		},
		{ TRANSFORMED_VECTOR(sf::Vector2f(424.f, 665.f)), TRANSFORMED_VECTOR(sf::Vector2f(856.f, 696.f)), VECTOR2F_UP }
},
m_player()
{
	UIMANAGER.Load("ui.xml");

	m_player.SetPosition({
			static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x),
			0
		}
	);

	// TODO: This is a bit stinky! I think it would probably be better to make this be another texture
	const sf::Vector2f boxSize = {
		TRANSFORMED_SCALAR(5.f), TRANSFORMED_SCALAR(static_cast<float>(TEXTUREMANAGER.GetTexture("Box")->getSize().y))
	};

	m_playerGuideLine.setSize(boxSize);
	m_playerGuideLine.setFillColor(static_cast<sf::Color>(0xFFFFFF40));
	m_playerGuideLine.setOrigin({ m_playerGuideLine.getLocalBounds().size.x / 2.f, 0.f });
}

Game::~Game() = default;

void Game::Update()
{
	m_player.Update();

	UIMANAGER.GetUiPanel("ScoreBubble_Panel")->GetUiText("ScoreBubble_Score")->SetText(
		std::to_string(m_player.GetPoints()).c_str());
}

void Game::Render(sf::RenderWindow& window) const
{
	// Draw the background items first
	UIMANAGER.RenderBackground(window);

#if !BUILD_MASTER
	for (const auto& [topLeft, bottomRight, normal] : m_boundaries)
	{
		sf::RectangleShape rect{ bottomRight - topLeft };
		rect.setFillColor(static_cast<sf::Color>(0x808080FF));
		rect.setPosition(topLeft);
		window.draw(rect);
	}
#endif

	// Then the midground stuff
	UIMANAGER.RenderMidground(window);

	// Then the game elements
	window.draw(m_playerGuideLine);

	// Then, in front of everything, the foreground stuff
	UIMANAGER.RenderForeground(window);

#if !BUILD_MASTER
	DrawText(window, VECTOR2F_ZERO, 30, "%.1fFPS", Timer::Get().Fps());
	DrawText(window, VECTOR2F_ZERO + sf::Vector2f{ 0.f, 100.f }, 10, "Active Fruit: %llu", 69);
#endif
}