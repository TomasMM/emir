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

#ifndef MIR_GRAPHICS_DISPLAY_H_
#define MIR_GRAPHICS_DISPLAY_H_

#include "mir/geometry/forward.h"

namespace mir
{
namespace graphics
{
class display
{
public:
    virtual geometry::rectangle view_area() = 0;

protected:
    display() = default;
    ~display() = default;
private:
    display(display const&) = delete;
    display& operator=(display const&) = delete;
};
}
}

#endif /* MIR_GRAPHICS_DISPLAY_H_ */