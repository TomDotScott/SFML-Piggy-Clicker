#include "Game.h"
#include <fstream>
#include <set>
#include <SFML/Graphics.hpp>

#include "../Engine/TextureManager.h"
#include "../Engine/Globals.h"
#include "../Engine/Timer.h"
#include "../Libs/hoxml.h"

Game::Game() :
	m_player()
{
	UIMANAGER.Load("ui.xml");

	m_player.SetPosition({
			static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x),
			0
		}
	);
}

Game::~Game() = default;

void Game::Update()
{
	UIMANAGER.Update();
	m_player.Update();
}

void Game::Render(sf::RenderWindow& window) const
{
	// Draw the background items first
	UIMANAGER.RenderBackground(window);

	// Then the midground stuff
	UIMANAGER.RenderMidground(window);

	// Then, in front of everything, the foreground stuff
	UIMANAGER.RenderForeground(window);

#if !BUILD_MASTER
	DrawText(window, VECTOR2F_ZERO, 30, "%.1fFPS", Timer::Get().Fps());
	DrawText(window, VECTOR2F_ZERO + sf::Vector2f{ 0.f, 100.f }, 10, "Active Fruit: %llu", 69);
#endif
}