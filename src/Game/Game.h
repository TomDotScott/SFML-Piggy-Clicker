#ifndef GAME_H
#define GAME_H
#include <set>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Player.h"
#include "../Engine/Event.h"
#include "../Engine/UI/UiManager.h"
#include "../Engine/ObjectPool.h"

class Game
{
public:
	Game();
	~Game();

	void Update();

	void Render(sf::RenderWindow& window) const;

private:
	Player m_player;

	uint64_t m_piggyCount;
	int m_autoClickerCount;

	uint64_t m_piggyCountLastTick;
	float m_elapsedTime;

	void OnPiggyClicked();

	void OnAutoClickerClicked();

	void SetPiggiesText() const;
	void SetPiggiesPerSecondText(float pps);
	void IncrementCounter();

#if !BUILD_MASTER
	template<typename... Args>
	static void DrawText(sf::RenderWindow& window, const sf::Vector2f& position, const int size, const char* fmt, Args... args)
	{
		if (auto* debugUi = UIMANAGER.GetUiText("DEBUG_TEXT"))
		{
			debugUi->SetPosition(position);
			debugUi->SetTextSize(size);
			debugUi->SetText(fmt, args...);

			UIMANAGER.DrawDebugText(window);
		}
	}
#endif
};


#endif
