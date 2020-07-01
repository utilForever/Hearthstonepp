// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Battlegrounds/Conditions/SelfCondition.hpp>
#include <Rosetta/Battlegrounds/Models/Minion.hpp>

namespace RosettaStone::Battlegrounds
{
SelfCondition::SelfCondition(std::function<bool(Minion&)> func)
    : m_func(std::move(func))
{
    // Do nothing
}
}  // namespace RosettaStone::Battlegrounds
