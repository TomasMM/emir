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
 * Kevin DuBois <kevin.dubois@canonical.com>
 */

#ifndef MIR_COMPOSITOR_BUFFER_BUNDLE_H_
#define MIR_COMPOSITOR_BUFFER_BUNDLE_H_

#include "buffer_texture_binder.h"
#include "buffer_queue.h"
#include "buffer.h"

#include <atomic>
#include <mutex>
#include <memory>
#include <vector>

namespace mir
{
namespace compositor
{
class BufferSwapper;
class BufferBundle : public BufferTextureBinder,
                     public BufferQueue
{
 public:
    BufferBundle();
    ~BufferBundle();

    void add_buffer(std::shared_ptr<Buffer> buffer);
    int remove_all_buffers();

    void set_swap_pattern( BufferSwapper *pattern);

    std::shared_ptr<Buffer> dequeue_client_buffer();
    void queue_client_buffer(std::shared_ptr<Buffer> buffer);

 protected:
    BufferBundle(const BufferBundle&) = delete;
    BufferBundle& operator=(const BufferBundle&) = delete;

    // From BufferTextureBinder
    void lock_back_buffer();
    void unlock_back_buffer();

    std::shared_ptr<Buffer> back_buffer();

  private:
    std::vector<std::shared_ptr<Buffer>> buffer_list;
    std::mutex buffer_list_guard;
    std::mutex back_buffer_guard;
    
    std::shared_ptr<Buffer> compositor_buffer;
    std::shared_ptr<Buffer> client_buffer;
};

}
}

#endif /* MIR_COMPOSITOR_BUFFER_BUNDLE_H_ */