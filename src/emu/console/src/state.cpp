/*
 * Copyright (c) 2019 EKA2L1 Team.
 * 
 * This file is part of EKA2L1 project 
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

#include <common/algorithm.h>
#include <common/log.h>
#include <common/path.h>
#include <console/state.h>
#include <gdbstub/gdbstub.h>

#include <debugger/imgui_debugger.h>
#include <debugger/logger.h>
#include <drivers/audio/audio.h>
#include <drivers/graphics/graphics.h>

#include <manager/device_manager.h>
#include <manager/manager.h>

#include <epoc/kernel.h>

namespace eka2l1::desktop {
    void emulator::stage_one() {
        // Initialize the logger
        logger = std::make_shared<imgui_logger>();
        log::setup_log(logger);

        // Start to read the configs
        conf.deserialize();

        // Initialize an empty root.
        symsys = std::make_unique<eka2l1::system>(nullptr, nullptr, &conf);
        symsys->init();

        first_time = true;
        launch_requests.max_pending_count_ = 100;

        // Make debugger. Go watch Case Closed.
        debugger = std::make_unique<eka2l1::imgui_debugger>(symsys.get(), logger.get(), [&](const std::u16string &path) {
            launch_requests.push(path);
        });

        symsys->set_debugger(debugger.get());
        symsys->set_device(conf.device);
        symsys->mount(drive_c, drive_media::physical, eka2l1::add_path(conf.storage, "/drives/c/"), io_attrib::internal);
        symsys->mount(drive_e, drive_media::physical, eka2l1::add_path(conf.storage, "/drives/e/"), io_attrib::removeable);

        if (conf.enable_gdbstub) {
            symsys->get_gdb_stub()->set_server_port(conf.gdb_port);
            symsys->get_gdb_stub()->init(symsys.get());
            symsys->get_gdb_stub()->toggle_server(true);
        }

        winserv = reinterpret_cast<eka2l1::window_server *>(symsys->get_kernel_system()->get_by_name<eka2l1::service::server>("!Windowserver"));

        stage_two_inited = false;
    }

    void emulator::stage_two() {
        if (!stage_two_inited) {
            manager::device_manager *dvcmngr = symsys->get_manager_system()->get_device_manager();
            manager::device *dvc = dvcmngr->get_current();

            if (!dvc) {
                LOG_ERROR("No current device is available. Stage two initialisation abort");
                return;
            }

            bool res = symsys->load_rom(add_path(conf.storage, add_path("roms", add_path(
                common::lowercase_string(dvc->firmware_code), "SYM.ROM"))));

            if (!res) {
                return;
            }

            // Mount the drive Z after the ROM was loaded. The ROM load than a new FS will be
            // created for ROM purpose.
            symsys->mount(drive_z, drive_media::rom,
                eka2l1::add_path(conf.storage, "/drives/z/"), io_attrib::internal | io_attrib::write_protected);

            // Create audio driver
            audio_driver = drivers::make_audio_driver(drivers::audio_driver_backend::cubeb);
            symsys->set_audio_driver(audio_driver.get());

            stage_two_inited = true;
        }
    }
}