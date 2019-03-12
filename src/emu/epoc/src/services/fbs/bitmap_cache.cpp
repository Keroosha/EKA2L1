/*
 * Copyright (c) 2019 EKA2L1 Team
 * 
 * This file is part of EKA2L1 project
 * (see bentokun.github.com/EKA2L1).
 * 
 * Initial contributor: pent0
 * Contributors:
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <epoc/services/fbs/bitmap_cache.h>
#include <epoc/services/fbs/fbs.h>

#include <common/time.h>
#include <algorithm>

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace eka2l1::epoc {
    bitmap_cache::bitmap_cache(fbs_server *serv_, graphics_driver_client_ptr cli_) 
        : serv(serv_), cli(cli_) {
    }

    std::uint64_t bitmap_cache::hash_bitwise_bitmap(epoc::bitwise_bitmap *bw_bmp) {
        std::uint64_t hash = 0xB1711A3F;

        // Hash using XXHASH
        XXH64_state_t *const state = XXH64_createState();
        XXH64_reset(state, hash);

        // First, hash the single bitmap header
        XXH64_update(state, reinterpret_cast<const void*>(&bw_bmp->header_), sizeof(loader::sbm_header));

        // Now, hash the byte width and UID, if it changes, we need to update
        XXH64_update(state, reinterpret_cast<const void*>(&bw_bmp->byte_width_), sizeof(bw_bmp->byte_width_));
        XXH64_update(state, reinterpret_cast<const void*>(&bw_bmp->uid_), sizeof(bw_bmp->uid_));

        // Lastly, we needs to hash the data, to see if anything changed
        XXH64_update(state, serv->get_large_chunk_pointer(bw_bmp->data_offset_), bw_bmp->header_.compressed_len);

        hash = XXH64_digest(state);
        XXH64_freeState(state);

        return hash;
    }

    std::int64_t bitmap_cache::get_suitable_bitmap_index(){
        // First time, will scans through the bitmap array to find empty box
        // Sometimes, app might purges a lot of bitmaps at same time
        for (std::int64_t i = MAX_CACHE_SIZE - 1; i >= 0; i--) {
            if (!bitmaps[i]) {
                return i;
            }
        }

        // Next, we will compare the timestamp
        std::uint64_t oldest_timestamp_idx = 0;
        for (std::int64_t i = 1; i < MAX_CACHE_SIZE; i++) {
            if (timestamps[i] < timestamps[oldest_timestamp_idx]) {
                oldest_timestamp_idx = i; 
            }
        }

        return oldest_timestamp_idx;
    }
    
    drivers::handle bitmap_cache::add_or_get(epoc::bitwise_bitmap *bmp) {
        std::int64_t idx = 0;
        std::uint64_t crr_timestamp = common::get_current_time_in_microseconds_since_1ad();

        std::uint64_t hash = 0;

        bool should_upload = true;

        auto bitmap_ite = std::find(bitmaps.begin(), bitmaps.end(), bmp);
        if (bitmap_ite == bitmaps.end()) {
            // If the bitmap is not in the bitmap array
            if (last_free < MAX_CACHE_SIZE) {
                // Use last free
                idx = last_free++;
            } else {
                idx = get_suitable_bitmap_index();
            }
            
            bitmaps[idx] = bmp;
        } else {            
            // Else, get the index
            idx = std::distance(bitmaps.begin(), bitmap_ite);

            // Check if we should upload or not, by calcuting the hash
            hash = hash_bitwise_bitmap(bmp);
            should_upload = hash != (hashes[idx]);
        }

        if (should_upload) {
            driver_textures[idx] = cli->upload_bitmap(driver_textures[idx]
                , reinterpret_cast<char*>(serv->get_large_chunk_pointer(bmp->data_offset_))
                , bmp->header_.compressed_len, bmp->header_.size_pixels.width(), bmp->header_.size_pixels.height()
                , bmp->header_.bit_per_pixels);

            hashes[idx] = hash;
        }

        timestamps[idx] = crr_timestamp;
        return driver_textures[idx];
    }
}
