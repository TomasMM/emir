/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Robert Carr <robert.carr@canonical.com>
 */
#include "mir/input/event_filter.h"
#include "src/input/android/event_filter_dispatcher_policy.h"
#include "mir_test/empty_deleter.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <androidfw/Input.h>

namespace mi = mir::input;
namespace mia = mi::android;

namespace
{
struct MockEventFilter : public mi::EventFilter
{
    MOCK_METHOD1(handles, bool(const droidinput::InputEvent*));
};
}

TEST(EventFilterDispatcherPolicy, offers_events_to_filter)
{
    using namespace ::testing;
    auto ev = new droidinput::KeyEvent();
    MockEventFilter filter;
    mia::EventFilterDispatcherPolicy policy(std::shared_ptr<MockEventFilter>(&filter, mir::EmptyDeleter()));
    uint32_t policy_flags;
    
    EXPECT_CALL(filter, handles(_)).Times(1).WillOnce(Return(false));

    // The policy filters ALL key events before queuing
    policy.interceptKeyBeforeQueueing(ev, policy_flags);
    EXPECT_EQ(policy_flags, droidinput::POLICY_FLAG_FILTERED);

    // Android uses alternate notation...the policy returns true if the event was NOT handled (e.g. the EventFilter
    // returns false)
    EXPECT_TRUE(policy.filterInputEvent(ev, 0));
}
