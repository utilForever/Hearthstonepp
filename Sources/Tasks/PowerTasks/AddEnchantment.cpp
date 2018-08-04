/*************************************************************************
> File Name: DestroyWeapon.cpp
> Project Name: Hearthstonepp
> Author: Young-Joong Kim
> Purpose: Implement DestroyWeapon
> Created Time: 2018/07/22
> Copyright (c) 2018, Young-Joong Kim
*************************************************************************/
#include <Tasks/PowerTasks/AddEnchantment.h>

namespace Hearthstonepp::PowerTask
{
AddEnchantmentTask::AddEnchantmentTask(std::string&& cardID,
                                       EntityType entityType)
    : m_cardID(cardID), m_entityType(entityType)
{
    // Do nothing
}

MetaData AddEnchantmentTask::Impl(Player&, Player&)
{
    const Card* enchantmentCard = Cards::GetInstance()->FindCardByID(m_cardID);
    if (enchantmentCard == nullptr)
    {
        return MetaData::INVALID;
    }

    Power* power = Cards::GetInstance()->FindCardByID(m_cardID)->power;
    if (power == nullptr)
    {
        return MetaData::INVALID;
    }

    power->enchant->ActivateTo(target);

    return MetaData::INVALID;
}
}  // namespace Hearthstonepp::PowerTask