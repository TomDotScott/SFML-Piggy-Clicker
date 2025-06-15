#include "UpgradeManager.h"

UpgradeManager::UpgradeManager()
{
	InitUpgrades();
}

void UpgradeManager::InitUpgrades()
{
	// Set up the UpgradeData
	// TODO: Load this from XML
	const std::unordered_map<eUpgradeType, UpgradeData> upgradeData = {
		{ eUpgradeType::AutoClicker, { "Auto Clicker", 10, 1.5f, 1.f, 1.f } },
	};

	// Then the UpgradeStates
	std::unordered_map<eUpgradeType, UpgradeState> upgradeStates;
	for (const auto& [type, data] : upgradeData)
	{
		upgradeStates[type] = UpgradeState{
			0,
			upgradeData.at(type).m_BasePrice,
			static_cast<float>(upgradeData.at(type).m_BasePPS)
		};
	}

	// Finally, add them to the map
	for (int i = 0; i < static_cast<int>(eUpgradeType::COUNT); ++i)
	{
		const eUpgradeType type = static_cast<eUpgradeType>(i);
		m_upgrades[type] = {
			upgradeData.at(type),
			upgradeStates.at(type)
		};
	}
}

bool UpgradeManager::CanAfford(const eUpgradeType type, const uint64_t piggyCount) const
{
	const auto it = m_upgrades.find(type);
	if (it == m_upgrades.end())
	{
		return false;
	}

	return piggyCount >= static_cast<uint64_t>(it->second.m_State.m_CachedPrice);
}

bool UpgradeManager::BuyUpgrade(const eUpgradeType type, uint64_t& piggyCount)
{
	if (!CanAfford(type, piggyCount))
	{
		return false;
	}

	UpgradeState& state = m_upgrades[type].m_State;

	piggyCount -= state.m_CachedPrice;
	state.m_AmountOwned++;

	RecalculateCachedPrices(type);

	return true;
}

void UpgradeManager::RecalculateCachedPrices(const eUpgradeType type)
{
	UpgradeState& state = m_upgrades[type].m_State;
	const UpgradeData& def = m_upgrades[type].m_Data;

	state.m_CachedPrice = static_cast<int>(
		def.m_BasePrice * std::pow(def.m_PriceMultiplier, state.m_AmountOwned)
		);

	state.m_CachedPPSPerUnit = def.m_BasePPS * std::powf(def.m_RateOfDecline, static_cast<float>(state.m_AmountOwned));
}

float UpgradeManager::GetTotalPPS() const
{
	float total = 0.0f;
	for (const auto& [type, upgrade] : m_upgrades)
	{
		total += upgrade.m_State.m_CachedPPSPerUnit * static_cast<float>(upgrade.m_State.
			m_AmountOwned);
	}
	return total;
}

int UpgradeManager::GetUpgradePrice(const eUpgradeType type) const
{
	const auto it = m_upgrades.find(type);

	if (it != m_upgrades.end())
	{
		return it->second.m_State.m_CachedPrice;
	}

	return 0;
}

float UpgradeManager::GetUpgradePPS(const eUpgradeType type) const
{
	const auto it = m_upgrades.find(type);
	if (it != m_upgrades.end())
	{
		return it->second.m_State.m_CachedPPSPerUnit;
	}

	return 0.0f;
}

int UpgradeManager::GetAmountOwned(const eUpgradeType type) const
{
	const auto it = m_upgrades.find(type);
	if ((it != m_upgrades.end()))
	{
		return it->second.m_State.m_AmountOwned;
	}

	return 0;
}
