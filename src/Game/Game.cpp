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
	m_piggyCount(0),
	m_autoClickerCount(0)
{
	UIMANAGER.Load("ui.xml");

	m_player.SetPosition({
			static_cast<float>(GRAPHIC_SETTINGS.GetScreenDetails().m_ScreenCentre.x),
			0
		}
	);


	UIMANAGER.GetUiButton("Clicker")->OnButtonPressed([this] { OnPiggyClicked(); });
	UIMANAGER.GetUiButton("AUTO_CLICKER")->OnButtonPressed([this] { OnAutoClickerClicked(); });
	SetPiggiesText();
	SetPiggiesPerSecondText(0.f);
}

Game::~Game() = default;

void Game::Update()
{
	m_elapsedTime += Timer::Get().DeltaTime();

	UIMANAGER.Update();
	m_player.Update();

	if (m_elapsedTime > 1) {

		for (int i = 0; i < m_autoClickerCount; ++i)
		{
			m_piggyCount++;
		}

		const auto piggyDelta = static_cast<float>(m_piggyCount) - static_cast<float>(m_piggyCountLastTick);

		SetPiggiesPerSecondText(piggyDelta / m_elapsedTime);

		m_piggyCountLastTick = m_piggyCount;

		m_elapsedTime = 0.f;
	}

	SetPiggiesText();
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
	DrawText(window, sf::Vector2f{ 0, 10 }, 30, "%.1fFPS", Timer::Get().Fps());
#endif
}

void Game::OnPiggyClicked()
{
	IncrementCounter();
}

void Game::OnAutoClickerClicked()
{
	if (m_piggyCount < 10)
	{
		return;
	}

	m_piggyCount -= 10;
	m_autoClickerCount++;

	printf("Bought an auto clicker!\n");
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
}
