// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "gtest/gtest.h"
#include <Utils/TestUtils.h>

#include <hspp/Tasks/BasicTasks/BriefTask.h>

using namespace Hearthstonepp;

TEST(BriefTask, GetTaskID)
{
    const BasicTasks::BriefTask brief;
    EXPECT_EQ(brief.GetTaskID(), +TaskID::BRIEF);
}

TEST(BriefTask, Run)
{
    BasicTasks::BriefTask brief;
    TestUtils::PlayerGenerator gen(CardClass::DRUID, CardClass::ROGUE);

    gen.player1.id = 100;

    MetaData result = brief.Run(gen.player1, gen.player2);
    EXPECT_EQ(result, MetaData::BRIEF);

    TaskMeta meta;
    result = brief.Run(gen.player1, gen.player2, meta);
    EXPECT_EQ(result, MetaData::BRIEF);
    EXPECT_EQ(meta.id, +TaskID::BRIEF);
    EXPECT_EQ(meta.status, MetaData::BRIEF);
    EXPECT_EQ(meta.userID, gen.player1.id);
}
