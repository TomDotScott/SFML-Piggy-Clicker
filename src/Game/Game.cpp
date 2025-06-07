#include "Game.h"
#include <fstream>
#include <set>
#include <SFML/Graphics.hpp>

#include "../Engine/TextureManager.h"
#include "../Engine/Globals.h"
#include "../Engine/Timer.h"
#include "../Engine/Input/Keyboard.h"
#include "../Libs/hoxml.h"


Game::Game() :
	m_player(),
	m_piggyCount(0)
{
	UIMANAGER.Load("ui.xml");

	m_player.SetPosition({
			static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x),
			0
		}
	);


	UIMANAGER.GetUiButton("Clicker")->OnButtonPressed([this] { OnPiggyClicked(); });
	SetPiggiesText();
	SetPiggiesPerSecondText(0.f);
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
#endif
}

void Game::OnPiggyClicked()
{
	IncrementCounter();
}

void Game::SetPiggiesText() const
{
	UIMANAGER.GetUiPanel("PIGGIES_PANEL")->GetUiText("PIGGY_COUNT")->SetText("%llu Piggies", m_piggyCount);
}

void Game::SetPiggiesPerSecondText(const float pps)
{
	UIMANAGER.GetUiPanel("PIGGIES_PANEL")->GetUiText("PPS")->SetText("Per Second: %.2f", pps);
}

void Game::IncrementCounter()
{
	m_piggyCount++;
	SetPiggiesText();
}
