#ifndef VRAM_MEMORY_HPP
#define VRAM_MEMORY_HPP
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 110
/*
 * VRAM block allocation
 */



#include <memory>

namespace vram {
    namespace memory {
        class block;
        typedef std::shared_ptr<block> block_ref;

        // Total blocks and blocks currently free
        size_t pool_size();
        size_t pool_available();

        // Allocate pool of memory blocks, returns actual amount allocated (in bytes)
        size_t increase_pool(size_t size);

        // Get a new block of memory from the pool, returns nullptr if pool is empty
        block_ref allocate();

        /*
         * Block of allocated VRAM
         */

        class block : public std::enable_shared_from_this<block> {
            friend block_ref allocate();

        public:
            // Best performance/size balance
            static size_t size;// 128 * 1024;

            static size_t disk_size;
            block(const block& other) = delete;

            ~block();

            void read(off_t offset, size_t size, void* data) const;

            // Data may be freed afterwards, even if called with async = true
            void write(off_t offset, size_t size, const void* data, bool async = false);

            // Wait for all writes to this block to complete

        private:
            char* buffer;
            // True until first write (until then it contains leftover data from last use)

            block();
        };
    }
}

#endif
