#include "memory.hpp"
#include <vector>

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <mutex>

using std::lock_guard;
using std::mutex;
using std::string;
using std::dynamic_pointer_cast;
static std::mutex memmutex;
#include <iostream>
using namespace std;
namespace vram {
    namespace memory {

        // Connection with OpenCL
        std::vector<char*> pool;
        size_t total_blocks = 0;

        size_t device_num;

        size_t block::disk_size = 0;

        size_t block::size= 128 * 1024;

        size_t pool_size() {
            return total_blocks;
        }

        size_t pool_available() {
            return pool.size();
        }

        size_t increase_pool(size_t size) {
            //cout<<"increasing pool size. cur size = "<< pool.size()<<endl;
            lock_guard<mutex> local_lock(memmutex);
            int block_count = 1 + (size - 1) / block::size;

            for (int i = 0; i < block_count; i++) {
                char* buf = (char*)malloc(block::size);
                if (buf !=NULL){
                    pool.push_back(buf);
                    total_blocks++;
                } else {

                    printf("Mem pool size %0.4lf GB\n",(double)total_blocks*block::size/1024/1024/1024);
                    fflush(stdout);
                    return i * block::size;
                }
            }

            printf("Mem pool size %0.4lf GB\n",(double)total_blocks*block::size/1024/1024/1024);
            fflush(stdout);

            return block_count * block::size;
        }

        block_ref allocate() {
            if (pool.size() != 0) {
                return block_ref(new block());
            } else {

                increase_pool(memory::block::disk_size);

                return block_ref(new block());
            }
        }

        block::block() {
            buffer = pool.back();
            pool.pop_back();
        }

        block::~block() {
            pool.push_back(buffer);
        }

        void block::read(off_t offset, size_t size, void* data) const {
            memcpy(data,buffer+offset,size);
        }

        void block::write(off_t offset, size_t size, const void* data, bool async) {
            memcpy((char*)(buffer+offset),data,size);
        }

    }
}
