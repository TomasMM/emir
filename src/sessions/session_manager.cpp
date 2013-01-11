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
 * Authored by: Thomas Voss <thomas.voss@canonical.com>
 */

#include "mir/sessions/session_manager.h"
#include "mir/sessions/session.h"
#include "mir/sessions/session_container.h"
#include "mir/sessions/surface_factory.h"
#include "mir/sessions/focus_sequence.h"
#include "mir/sessions/focus_setter.h"

#include <memory>
#include <cassert>
#include <algorithm>

namespace msess = mir::sessions;

msess::SessionManager::SessionManager(
    std::shared_ptr<msess::SurfaceFactory> const& surface_factory,
    std::shared_ptr<msess::SessionContainer> const& container,
    std::shared_ptr<msess::FocusSequence> const& sequence,
    std::shared_ptr<msess::FocusSetter> const& focus_setter) :
    surface_factory(surface_factory),
    app_container(container),
    focus_sequence(sequence),
    focus_setter(focus_setter)
{
    assert(surface_factory);
    assert(sequence);
    assert(container);
    assert(focus_setter);
}

msess::SessionManager::~SessionManager()
{
}

std::shared_ptr<msess::Session> msess::SessionManager::open_session(std::string const& name)
{
    auto new_session = std::make_shared<msess::Session>(surface_factory, name);

    app_container->insert_session(new_session);
    focus_application = new_session;
    focus_setter->set_focus_to(new_session);
 
    return new_session;
}

void msess::SessionManager::close_session(std::shared_ptr<msess::Session> const& session)
{
    if (session == focus_application.lock())
    {
        focus_application = focus_sequence->predecessor_of(session);
        focus_setter->set_focus_to(focus_application.lock());
    }
    app_container->remove_session(session);
}

void msess::SessionManager::focus_next()
{
    auto focused = focus_application.lock();
    if (focused == NULL)
    {
        return;
    }
    auto next_focus = focus_sequence->successor_of(focused).lock();
    focus_application = next_focus;
    focus_setter->set_focus_to(next_focus);
}

void msess::SessionManager::shutdown()
{
    app_container->for_each([](std::shared_ptr<Session> const& session)
    {
        session->shutdown();
    });
}