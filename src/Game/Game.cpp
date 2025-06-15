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
	m_upgradeManager(),
	m_piggyCount(0),
	m_elapsedTime(0.f)
{
	UIMANAGER.Load("ui.xml");

	UIMANAGER.GetUiButton("Clicker")->OnButtonPressed([this] { OnPiggyClicked(); });
	UIMANAGER.GetUiButton("AUTO_CLICKER")->OnButtonPressed([this] { OnAutoClickerClicked(); });
	SetPiggiesText();
	SetPiggiesPerSecondText();
}

Game::~Game() = default;

void Game::Update()
{
	m_elapsedTime += Timer::Get().DeltaTime();

	UIMANAGER.Update();

	if (m_elapsedTime > 1)
	{
		m_piggyCount += m_upgradeManager.GetTotalPPS();

		SetPiggiesPerSecondText();

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
	if (!m_upgradeManager.CanAfford(eUpgradeType::AutoClicker, m_piggyCount))
	{
		return;
	}

	m_upgradeManager.BuyUpgrade(eUpgradeType::AutoClicker, m_piggyCount);

	UIMANAGER.GetUiButton("AUTO_CLICKER")->GetText()->SetText("%d", m_upgradeManager.GetUpgradePrice(eUpgradeType::AutoClicker));

	SetPiggiesPerSecondText();

	printf("Bought an auto clicker!\n");
}

void Game::SetPiggiesText() const
{
	UIMANAGER.GetUiPanel("PIGGIES_PANEL")->GetUiText("PIGGY_COUNT")->SetText("%llu Piggies", m_piggyCount);
}

void Game::SetPiggiesPerSecondText() const
{
	UIMANAGER.GetUiPanel("PIGGIES_PANEL")->GetUiText("PPS")->SetText("Per Second: %.2f", m_upgradeManager.GetTotalPPS());
}

void Game::IncrementCounter()
{
	m_piggyCount++;
}
