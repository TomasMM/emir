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
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#include "mir/server_configuration.h"
#include "mir/graphics/display.h"
#include "protobuf_socket_communicator.h"

namespace mf = mir::frontend;
namespace mg = mir::graphics;
namespace ms = mir::surfaces;

std::shared_ptr<mf::Communicator>
mir::DefaultServerConfiguration::make_communicator(
    std::shared_ptr<ms::ApplicationSurfaceOrganiser> const& surface_organiser,
    std::shared_ptr<mg::Display> const& display)
{
    return std::make_shared<mf::ProtobufSocketCommunicator>(
        socket_file, make_ipc_factory(surface_organiser, display));
}
