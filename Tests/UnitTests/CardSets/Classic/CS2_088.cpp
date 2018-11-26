// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Utils/CardSetUtils.h>

#include <hspp/Actions/Generic.h>
#include <hspp/Cards/Cards.h>

using namespace Hearthstonepp;
using namespace BasicTasks;

TEST(ClassicCardSet, CS2_088)
{
    GameAgent agent(CardClass::DRUID, CardClass::ROGUE, PlayerType::PLAYER1);
    TaskAgent& taskAgent = agent.GetTaskAgent();

    Player& currentPlayer = agent.GetCurrentPlayer();
    Player& opponentPlayer = agent.GetCurrentPlayer().GetOpponent();
    currentPlayer.SetMaximumMana(10);
    currentPlayer.SetAvailableMana(10);
    opponentPlayer.SetMaximumMana(10);
    opponentPlayer.SetAvailableMana(10);
    opponentPlayer.GetHero()->health = 24;

    const auto card1 = Generic::DrawCard(
        currentPlayer,
        Cards::GetInstance().FindCardByName("Acidic Swamp Ooze"));
    EXPECT_EQ(currentPlayer.GetHand().size(), 1u);
    EXPECT_EQ(currentPlayer.GetHand()[0]->card->name, "Acidic Swamp Ooze");

    const auto card2 = Generic::DrawCard(
        opponentPlayer,
        Cards::GetInstance().FindCardByName("Guardian of Kings"));
    EXPECT_EQ(opponentPlayer.GetHand().size(), 1u);
    EXPECT_EQ(opponentPlayer.GetHand()[0]->card->name, "Guardian of Kings");

    GameAgent::RunTask(currentPlayer, PlayCardTask(taskAgent, card1));
    EXPECT_EQ(currentPlayer.GetField()[0]->card->name, "Acidic Swamp Ooze");

    GameAgent::RunTask(opponentPlayer, PlayCardTask(taskAgent, card2));
    EXPECT_EQ(opponentPlayer.GetField()[0]->card->name, "Guardian of Kings");
    EXPECT_EQ(opponentPlayer.GetHero()->maxHealth,
              opponentPlayer.GetHero()->health);
}