// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "gtest/gtest.h"
#include <Utils/TestUtils.h>

#include <hspp/Tasks/BasicTasks/GameEndTask.h>

using namespace Hearthstonepp;

TEST(GameEndTask, GetTaskID)
{
    const BasicTasks::GameEndTask gameEnd;
    EXPECT_EQ(gameEnd.GetTaskID(), +TaskID::GAME_END);
}

TEST(GameEndTask, Run)
{
    BasicTasks::GameEndTask gameEnd;
    TestUtils::PlayerGenerator gen(CardClass::DRUID, CardClass::ROGUE);

    gen.player1.id = 100;

    MetaData result = gameEnd.Run(gen.player1, gen.player2);
    EXPECT_EQ(result, MetaData::GAME_END);

    TaskMeta meta;
    result = gameEnd.Run(gen.player1, gen.player2, meta);
    EXPECT_EQ(result, MetaData::GAME_END);
    EXPECT_EQ(meta.id, +TaskID::GAME_END);
    EXPECT_EQ(meta.status, MetaData::GAME_END);
    EXPECT_EQ(meta.userID, gen.player1.id);
}
