#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <unordered_map>
#include <string>
#include <cstdint>

enum class eUpgradeType : uint8_t
{
	AutoClicker,
	COUNT
};

class UpgradeManager
{
public:
	UpgradeManager();

	bool CanAfford(eUpgradeType type, uint64_t piggyCount) const;
	bool BuyUpgrade(eUpgradeType type, uint64_t& piggyCount);
	float GetTotalPPS() const;
	int GetUpgradePrice(eUpgradeType type) const;
	float GetUpgradePPS(eUpgradeType type) const;
	int GetAmountOwned(eUpgradeType type) const;

	struct UpgradeData
	{
		std::string m_Name;
		int m_BasePrice;
		float m_PriceMultiplier;
		float m_BasePPS;
		float m_RateOfDecline;
	};

	struct UpgradeState
	{
		int m_AmountOwned = 0;
		int m_CachedPrice = 0;
		float m_CachedPPSPerUnit = 0.0f;
	};

	struct Upgrade
	{
		UpgradeData m_Data;
		UpgradeState m_State;
	};

private:
	void InitUpgrades();
	void RecalculateCachedPrices(eUpgradeType type);

	std::unordered_map<eUpgradeType, Upgrade> m_upgrades;
};

#endif
