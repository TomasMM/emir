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
 * Authored by:
 *   Kevin DuBois <kevin.dubois@canonical.com>
 */
#ifndef MIR_GRAPHICS_ANDROID_ANDROID_BUFFER_HANDLE_DEFAULT_H_
#define MIR_GRAPHICS_ANDROID_ANDROID_BUFFER_HANDLE_DEFAULT_H_

#include "mir/graphics/android/android_buffer_handle.h"

#include <system/window.h>

namespace mir
{
namespace graphics
{
namespace android
{

class AndroidBufferHandleDefault: public AndroidBufferHandle
{
public:
    explicit AndroidBufferHandleDefault(ANativeWindowBuffer buf, compositor::PixelFormat pf, BufferUsage use);

    geometry::Width width() const;
    geometry::Height height() const;
    geometry::Stride stride() const;
    compositor::PixelFormat format() const;
    BufferUsage usage() const;

    EGLClientBuffer get_egl_client_buffer() const;

private:
    const ANativeWindowBuffer anw_buffer;

    /* we save these so that when other parts of the system query for the mir
       types, we don't have to convert back */
    const compositor::PixelFormat pixel_format;
    const BufferUsage buffer_usage;
};

}
}
}

#endif /*MIR_GRAPHICS_ANDROID_ANDROID_BUFFER_HANDLE_DEFAULT_H_ */