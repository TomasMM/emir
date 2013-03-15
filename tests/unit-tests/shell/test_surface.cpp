/*
 * Copyright © 2013 Canonical Ltd.
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

#include "src/server/shell/surface.h"
#include "mir/surfaces/surface.h"
#include "mir/shell/surface_creation_parameters.h"
#include "mir/surfaces/surface_stack_model.h"

#include "mir_test_doubles/mock_buffer_bundle.h"
#include "mir_test_doubles/mock_buffer.h"
#include "mir_test_doubles/stub_buffer.h"

#include <stdexcept>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace ms = mir::surfaces;
namespace msh = mir::shell;
namespace mf = mir::frontend;
namespace mc = mir::compositor;
namespace geom = mir::geometry;
namespace mtd = mir::test::doubles;

namespace
{
typedef testing::NiceMock<mtd::MockBufferBundle> StubBufferBundle;

class MockSurfaceStackModel : public ms::SurfaceStackModel
{
public:
    MockSurfaceStackModel()
    {
        using namespace testing;

        ON_CALL(*this, create_surface(_)).
            WillByDefault(Invoke(this, &MockSurfaceStackModel::do_create_surface));

        ON_CALL(*this, destroy_surface(_)).
            WillByDefault(Invoke(this, &MockSurfaceStackModel::do_destroy_surface));
    }

    MOCK_METHOD1(create_surface, std::weak_ptr<ms::Surface> (const mf::SurfaceCreationParameters&));

    MOCK_METHOD1(destroy_surface, void (std::weak_ptr<ms::Surface> const&));

private:
    std::weak_ptr<ms::Surface> do_create_surface(const mf::SurfaceCreationParameters& params)
    {
        surface = std::make_shared<ms::Surface>(
            params.name,
            std::make_shared<StubBufferBundle>());
        return surface;
    }

    void do_destroy_surface(std::weak_ptr<ms::Surface> const&)
    {
        surface.reset();
    }

    std::shared_ptr<ms::Surface> surface;
};

struct ShellSurface : testing::Test
{
    std::shared_ptr<StubBufferBundle> buffer_bundle;

    ShellSurface() :
        buffer_bundle(std::make_shared<StubBufferBundle>())
    {
        using namespace testing;

        ON_CALL(*buffer_bundle, bundle_size()).WillByDefault(Return(geom::Size()));
        ON_CALL(*buffer_bundle, get_bundle_pixel_format()).WillByDefault(Return(geom::PixelFormat::abgr_8888));
        ON_CALL(*buffer_bundle, secure_client_buffer()).WillByDefault(Return(std::shared_ptr<mtd::StubBuffer>()));
    }
};
}

TEST_F(ShellSurface, creation_and_destruction)
{
    MockSurfaceStackModel surface_stack;
    mf::SurfaceCreationParameters params;

    using namespace testing;
    InSequence sequence;
    EXPECT_CALL(surface_stack, create_surface(_)).Times(1);
    EXPECT_CALL(surface_stack, destroy_surface(_)).Times(1);

    msh::Surface test(
        surface_stack.create_surface(params),
        [&](std::weak_ptr<mir::surfaces::Surface> const& s) {surface_stack.destroy_surface(s);});
}

TEST_F(ShellSurface, destroy)
{
    using namespace testing;

    NiceMock<MockSurfaceStackModel> surface_stack;
    mf::SurfaceCreationParameters params;

    msh::Surface test(
        surface_stack.create_surface(params),
        [&](std::weak_ptr<mir::surfaces::Surface> const& s) {surface_stack.destroy_surface(s);});

    EXPECT_CALL(surface_stack, destroy_surface(_)).Times(1);

    test.destroy();

    Mock::VerifyAndClearExpectations(&surface_stack);
}

TEST_F(ShellSurface, client_buffer_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.client_buffer();
    });

    surface.reset();

    EXPECT_THROW({
        proxy_surface.client_buffer();
    }, std::runtime_error);
}

TEST_F(ShellSurface, size_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.size();
    });

    surface.reset();

    EXPECT_THROW({
        proxy_surface.size();
    }, std::runtime_error);
}

TEST_F(ShellSurface, pixel_format_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.pixel_format();
    });

    surface.reset();

    EXPECT_THROW({
        proxy_surface.pixel_format();
    }, std::runtime_error);
}

TEST_F(ShellSurface, hide_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.hide();
    });

    surface.reset();

    EXPECT_NO_THROW({
        proxy_surface.hide();
    });
}

TEST_F(ShellSurface, show_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.show();
    });

    surface.reset();

    EXPECT_NO_THROW({
        proxy_surface.show();
    });
}

TEST_F(ShellSurface, destroy_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.destroy();
    });

    surface.reset();

    EXPECT_NO_THROW({
        proxy_surface.destroy();
    });
}

TEST_F(ShellSurface, shutdown_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.shutdown();
    });

    surface.reset();

    EXPECT_NO_THROW({
        proxy_surface.shutdown();
    });
}

TEST_F(ShellSurface, advance_client_buffer_throw_behavior)
{
    auto surface = std::make_shared<ms::Surface>(__PRETTY_FUNCTION__, buffer_bundle);

    msh::Surface proxy_surface(surface);

    EXPECT_NO_THROW({
        proxy_surface.advance_client_buffer();
    });

    surface.reset();

    EXPECT_NO_THROW({
        proxy_surface.advance_client_buffer();
    });
}