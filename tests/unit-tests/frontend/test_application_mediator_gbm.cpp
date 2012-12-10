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
 * Authored by: Alexandros Frantzis <alexandros.frantzis@canonical.com>
 */

#include "mir/frontend/application_listener.h"
#include "mir/frontend/application_mediator.h"
#include "mir/frontend/resource_cache.h"
#include "mir/frontend/session.h"
#include "mir/frontend/session_store.h"
#include "mir/frontend/surface_organiser.h"
#include "mir/graphics/display.h"
#include "mir/graphics/drm_authenticator.h"
#include "mir/graphics/platform.h"
#include "mir/graphics/platform_ipc_package.h"
#include "mir/exception.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace mf = mir::frontend;
namespace mg = mir::graphics;
namespace mc = mir::compositor;
namespace ms = mir::surfaces;
namespace geom = mir::geometry;
namespace mp = mir::protobuf;

namespace
{

/*
 * TODO: Fix design so that it's possible to unit-test ApplicationMediator
 * without having to create doubles for classes so deep in its dependency
 * hierarchy.
 *
 * In particular, it would be nice if mf::Session was stubable/mockable.
 */

class StubSurfaceOrganiser : public mf::SurfaceOrganiser
{
 public:
    std::weak_ptr<ms::Surface> create_surface(const ms::SurfaceCreationParameters& /*params*/)
    {
        return std::weak_ptr<ms::Surface>();
    }

    void destroy_surface(std::weak_ptr<ms::Surface> const& /*surface*/) {}

    void hide_surface(std::weak_ptr<ms::Surface> const& /*surface*/) {}

    void show_surface(std::weak_ptr<ms::Surface> const& /*surface*/) {}

    std::vector<std::shared_ptr<ms::Surface>> surfaces;
};

class StubSessionStore : public mf::SessionStore
{
public:
    StubSessionStore()
        : organiser{std::make_shared<StubSurfaceOrganiser>()}
    {
    }

    std::shared_ptr<mf::Session> open_session(std::string const& /*name*/)
    {
        return std::make_shared<mf::Session>(organiser, "stub");
    }

    void close_session(std::shared_ptr<mf::Session> const& /*session*/) {}

    void shutdown() {}

    std::shared_ptr<mf::SurfaceOrganiser> organiser;
};

class StubDisplay : public mg::Display
{
 public:
    geom::Rectangle view_area() const { return geom::Rectangle(); }
    void clear() {}
    bool post_update() { return true; }
};

class MockAuthenticatingPlatform : public mg::Platform, public mg::DRMAuthenticator
{
 public:
    std::shared_ptr<mc::GraphicBufferAllocator> create_buffer_allocator(
            const std::shared_ptr<mg::BufferInitializer>& /*buffer_initializer*/)
    {
        return std::shared_ptr<mc::GraphicBufferAllocator>();
    }

    std::shared_ptr<mg::Display> create_display()
    {
        return std::make_shared<StubDisplay>();
    }

    std::shared_ptr<mg::PlatformIPCPackage> get_ipc_package()
    {
        return std::make_shared<mg::PlatformIPCPackage>();
    }

    MOCK_METHOD1(drm_auth_magic, void(drm_magic_t));
};

}

struct ApplicationMediatorGBMTest : public ::testing::Test
{
    ApplicationMediatorGBMTest()
        : session_store{std::make_shared<StubSessionStore>()},
          mock_platform{std::make_shared<MockAuthenticatingPlatform>()},
          graphics_display{std::make_shared<StubDisplay>()},
          listener{std::make_shared<mf::NullApplicationListener>()},
          resource_cache{std::make_shared<mf::ResourceCache>()},
          mediator{session_store, mock_platform, graphics_display,
                   listener, resource_cache},
          null_callback{google::protobuf::NewPermanentCallback(google::protobuf::DoNothing)}
    {
    }

    std::shared_ptr<mf::SessionStore> const session_store;
    std::shared_ptr<MockAuthenticatingPlatform> const mock_platform;
    std::shared_ptr<mg::Display> const graphics_display;
    std::shared_ptr<mf::ApplicationListener> const listener;
    std::shared_ptr<mf::ResourceCache> const resource_cache;
    mf::ApplicationMediator mediator;

    std::unique_ptr<google::protobuf::Closure> null_callback;
};

TEST_F(ApplicationMediatorGBMTest, drm_auth_magic_uses_drm_authenticator)
{
    mp::ConnectParameters connect_parameters;
    mp::Connection connection;

    drm_magic_t const drm_magic{0x10111213};
    int const no_error{0};

    EXPECT_CALL(*mock_platform, drm_auth_magic(drm_magic))
        .Times(1);

    mediator.connect(nullptr, &connect_parameters, &connection, null_callback.get());

    mp::DRMMagic magic;
    mp::DRMAuthMagicStatus status;
    magic.set_magic(drm_magic);

    mediator.drm_auth_magic(nullptr, &magic, &status, null_callback.get());

    EXPECT_EQ(no_error, status.status_code());
}

TEST_F(ApplicationMediatorGBMTest, drm_auth_magic_sets_status_code_on_error)
{
    using namespace testing;

    mp::ConnectParameters connect_parameters;
    mp::Connection connection;

    drm_magic_t const drm_magic{0x10111213};
    int const error_number{667};

    EXPECT_CALL(*mock_platform, drm_auth_magic(drm_magic))
        .WillOnce(Throw(mir::Exception() << boost::errinfo_errno(error_number)));

    mediator.connect(nullptr, &connect_parameters, &connection, null_callback.get());

    mp::DRMMagic magic;
    mp::DRMAuthMagicStatus status;
    magic.set_magic(drm_magic);

    mediator.drm_auth_magic(nullptr, &magic, &status, null_callback.get());

    EXPECT_EQ(error_number, status.status_code());
}