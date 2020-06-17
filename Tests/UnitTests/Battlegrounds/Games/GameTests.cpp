// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Rosetta/Battlegrounds/Cards/Cards.hpp>
#include <Rosetta/Battlegrounds/Games/Game.hpp>
#include <Rosetta/Battlegrounds/Utils/GameUtils.hpp>

#include <vector>

using namespace RosettaStone;
using namespace Battlegrounds;

TEST_CASE("[Game] - Basic")
{
    Game game;
    game.Start();

    CHECK_EQ(game.GetGameState().phase, Phase::SELECT_HERO);

    auto minions = game.GetGameState().minionPool.GetMinions(1, 6, true);
    CHECK_EQ(static_cast<int>(minions.size()),
             game.GetGameState().minionPool.GetCount());

    for (auto& player : game.GetGameState().players)
    {
        for (const auto& hero : player.heroChoices)
        {
            auto heroCard = Cards::FindCardByDbfID(hero);
            CHECK_EQ(heroCard.GetCardType(), CardType::HERO);
            CHECK_EQ(heroCard.isCurHero, true);
        }

        player.SelectHero(1);
    }

    CHECK_EQ(game.GetGameState().phase, Phase::RECRUIT);

    for (auto& player : game.GetGameState().players)
    {
        CHECK_EQ(player.hero.health, player.hero.card.GetHealth());

        CHECK_EQ(player.remainCoin, 3);
        CHECK_EQ(player.totalCoin, 3);
        CHECK_EQ(player.currentTier, 1);
        CHECK_EQ(player.coinToUpgradeTavern, 5);

        const std::size_t numMinions =
            GetNumMinionsCanPurchase(player.currentTier);

        for (std::size_t i = 0; i < numMinions; ++i)
        {
            CHECK_EQ(player.tavernFieldZone[i].GetTier(), 1);
        }
    }

    Player& player1 = game.GetGameState().players.at(0);

    player1.PurchaseMinion(0);
    CHECK_EQ(player1.handZone.GetCount(), 1);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 2);
    CHECK_EQ(player1.remainCoin, 0);

    minions = game.GetGameState().minionPool.GetMinions(1, 6, true);
    CHECK_EQ(static_cast<int>(minions.size()),
             game.GetGameState().minionPool.GetCount() - 3 * 8);

    player1.PlayCard(0, 0);
    CHECK_EQ(player1.handZone.GetCount(), 0);
    CHECK_EQ(player1.recruitFieldZone.GetCount(), 1);

    player1.SellMinion(0);
    CHECK_EQ(player1.recruitFieldZone.GetCount(), 0);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 2);
    CHECK_EQ(player1.remainCoin, 1);

    minions = game.GetGameState().minionPool.GetMinions(1, 6, true);
    CHECK_EQ(static_cast<int>(minions.size()),
             game.GetGameState().minionPool.GetCount() - 3 * 8 + 1);

    player1.RefreshTavern();
    CHECK_EQ(player1.handZone.GetCount(), 0);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 3);
    CHECK_EQ(player1.remainCoin, 0);

    minions = game.GetGameState().minionPool.GetMinions(1, 6, true);
    CHECK_EQ(static_cast<int>(minions.size()),
             game.GetGameState().minionPool.GetCount() - 3 * 8);

    player1.remainCoin = 10;

    player1.PurchaseMinion(0);
    CHECK_EQ(player1.handZone.GetCount(), 1);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 2);
    CHECK_EQ(player1.remainCoin, 7);

    player1.PurchaseMinion(0);
    CHECK_EQ(player1.handZone.GetCount(), 2);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 1);
    CHECK_EQ(player1.remainCoin, 4);

    player1.PlayCard(0, 0);
    player1.PlayCard(0, 1);

    const int poolIdx1 = player1.recruitFieldZone[0].GetPoolIndex();
    const int poolIdx2 = player1.recruitFieldZone[1].GetPoolIndex();

    player1.RearrangeMinion(0, 1);
    CHECK_EQ(poolIdx1, player1.recruitFieldZone[1].GetPoolIndex());
    CHECK_EQ(poolIdx2, player1.recruitFieldZone[0].GetPoolIndex());

    player1.UpgradeTavern();
    CHECK_EQ(player1.currentTier, 1);
    CHECK_EQ(player1.remainCoin, 4);

    player1.remainCoin = 10;

    player1.UpgradeTavern();
    CHECK_EQ(player1.currentTier, 2);
    CHECK_EQ(player1.remainCoin, 5);

    player1.RefreshTavern();
    CHECK_EQ(player1.handZone.GetCount(), 0);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 4);
    CHECK_EQ(player1.remainCoin, 4);

    const std::size_t numMinions =
        GetNumMinionsCanPurchase(player1.currentTier);
    for (std::size_t i = 0; i < numMinions; ++i)
    {
        bool check = player1.tavernFieldZone[i].GetTier() == 1 ||
                     player1.tavernFieldZone[i].GetTier() == 2;
        CHECK_EQ(check, true);
    }

    for (auto& player : game.GetGameState().players)
    {
        player.CompleteRecruit();
    }
}

TEST_CASE("[Game] - CalculateRank")
{
    Game game;
    game.Start();

    auto& players = game.GetGameState().players;
    players.at(0).hero.health = 30;
    players.at(1).hero.health = 0;
    players.at(2).hero.health = 15;
    players.at(3).hero.health = 40;
    players.at(4).hero.health = 20;
    players.at(5).hero.health = 0;
    players.at(6).hero.health = 5;
    players.at(7).hero.health = 0;

    players.at(1).playState = PlayState::LOST;
    players.at(5).playState = PlayState::LOST;
    players.at(7).playState = PlayState::LOST;

    const auto result = game.CalculateRank();
    CHECK_EQ(result.size(), 5);
    CHECK_EQ(std::get<0>(result[0]), 3);
    CHECK_EQ(std::get<1>(result[0]), 40);
    CHECK_EQ(std::get<0>(result[1]), 0);
    CHECK_EQ(std::get<1>(result[1]), 30);
    CHECK_EQ(std::get<0>(result[2]), 4);
    CHECK_EQ(std::get<1>(result[2]), 20);
    CHECK_EQ(std::get<0>(result[3]), 2);
    CHECK_EQ(std::get<1>(result[3]), 15);
    CHECK_EQ(std::get<0>(result[4]), 6);
    CHECK_EQ(std::get<1>(result[4]), 5);
}

TEST_CASE("[Game] - DetermineOpponent")
{
    Game game;
    game.Start();

    auto& players = game.GetGameState().players;
    players.at(0).hero.health = 30;
    players.at(1).hero.health = 20;
    players.at(2).hero.health = 15;
    players.at(3).hero.health = 40;
    players.at(4).hero.health = 20;
    players.at(5).hero.health = 10;
    players.at(6).hero.health = 5;
    players.at(7).hero.health = 35;

    players.at(0).playerIdxFoughtLastTurn = 1;
    players.at(1).playerIdxFoughtLastTurn = 0;
    players.at(2).playerIdxFoughtLastTurn = 7;
    players.at(3).playerIdxFoughtLastTurn = 5;
    players.at(4).playerIdxFoughtLastTurn = 6;
    players.at(5).playerIdxFoughtLastTurn = 3;
    players.at(6).playerIdxFoughtLastTurn = 4;
    players.at(7).playerIdxFoughtLastTurn = 2;

    auto result = game.DetermineOpponent();
    for (const auto& pair : result)
    {
        const std::size_t player1Idx = std::get<0>(pair);
        const std::size_t player2Idx = std::get<1>(pair);

        CHECK_NE(players.at(player1Idx).playerIdxFoughtLastTurn, player2Idx);
    }
}

TEST_CASE("[Game] - Freeze")
{
    Game game;
    game.Start();

    CHECK_EQ(game.GetGameState().phase, Phase::SELECT_HERO);

    for (auto& player : game.GetGameState().players)
    {
        player.SelectHero(1);
    }

    CHECK_EQ(game.GetGameState().phase, Phase::RECRUIT);

    Player& player1 = game.GetGameState().players.at(0);

    player1.PurchaseMinion(0);
    CHECK_EQ(player1.handZone.GetCount(), 1);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 2);
    CHECK_EQ(player1.remainCoin, 0);

    player1.FreezeTavern();

    for (auto& player : game.GetGameState().players)
    {
        player.CompleteRecruit();
    }

    CHECK_EQ(game.GetGameState().phase, Phase::RECRUIT);

    CHECK_EQ(player1.handZone.GetCount(), 1);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 2);
    CHECK_EQ(player1.remainCoin, 4);

    for (auto& player : game.GetGameState().players)
    {
        player.CompleteRecruit();
    }

    CHECK_EQ(game.GetGameState().phase, Phase::RECRUIT);

    CHECK_EQ(player1.handZone.GetCount(), 1);
    CHECK_EQ(player1.tavernFieldZone.GetCount(), 3);
    CHECK_EQ(player1.remainCoin, 5);
}