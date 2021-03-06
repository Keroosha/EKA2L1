/*
 * Copyright (c) 2020 EKA2L1 Team.
 * 
 * This file is part of EKA2L1 project.
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

#include <epoc/services/etel/subsess.h>
#include <epoc/services/etel/phone.h>
#include <epoc/services/context.h>
#include <epoc/utils/err.h>

#include <common/cvt.h>

namespace eka2l1 {
    etel_phone_subsession::etel_phone_subsession(etel_session *session, etel_phone *phone)
        : etel_subsession(session)
        , phone_(phone) {
    }

    void etel_phone_subsession::get_status(service::ipc_context *ctx) {
        ctx->write_arg_pkg<epoc::etel_phone_status>(0, phone_->status_);
        ctx->set_request_status(epoc::error_none);
    }
    
    void etel_phone_subsession::init(service::ipc_context *ctx) {
        if (!phone_->init()) {
            ctx->set_request_status(epoc::error_general);
            return;
        }

        ctx->set_request_status(epoc::error_none);
    }

    void etel_phone_subsession::enumerate_lines(service::ipc_context *ctx) {
        const std::uint32_t total_line = static_cast<std::uint32_t>(phone_->lines_.size());
        ctx->write_arg_pkg<std::uint32_t>(0, total_line);
        ctx->set_request_status(epoc::error_none);
    }

    void etel_phone_subsession::get_line_info(service::ipc_context *ctx) {
        struct line_info_package {
            std::int32_t index_;
            epoc::etel_line_info_from_phone info_;
        };

        std::optional<line_info_package> info_to_fill = ctx->get_arg_packed<line_info_package>(0);

        if (!info_to_fill) {
            ctx->set_request_status(epoc::error_argument);
            return;
        }
        
        if ((info_to_fill->index_ < 0) || (info_to_fill->index_ >= phone_->lines_.size())) {
            ctx->set_request_status(epoc::error_argument);
            return;
        }

        etel_line *line = phone_->lines_[info_to_fill->index_];

        info_to_fill->info_.name_.assign(nullptr, common::utf8_to_ucs2(line->name_));
        info_to_fill->info_.caps_ = line->caps_;
        info_to_fill->info_.sts_ = line->info_.sts_;

        ctx->write_arg_pkg<line_info_package>(0, info_to_fill.value());
        ctx->set_request_status(epoc::error_none);
    }

    void etel_phone_subsession::get_indicator_caps(service::ipc_context *ctx) {
        LOG_TRACE("Get indicator caps hardcoded");

        const std::uint32_t action_caps = epoc::etel_mobile_phone_indicator_cap_get;
        const std::uint32_t indicator_caps = epoc::etel_mobile_phone_indicator_charger_connected
            | epoc::etel_mobile_phone_indicator_network_avail
            | epoc::etel_mobile_phone_indicator_call_in_progress;

        ctx->write_arg_pkg<std::uint32_t>(0, action_caps);
        ctx->write_arg_pkg<std::uint32_t>(1, indicator_caps);

        ctx->set_request_status(epoc::error_none);
    }

    void etel_phone_subsession::get_indicator(service::ipc_context *ctx) {
        LOG_TRACE("Get indicator hardcoded");
        const std::uint32_t indicator = epoc::etel_mobile_phone_indicator_network_avail;

        ctx->set_request_status(epoc::error_none);
    }

    void etel_phone_subsession::dispatch(service::ipc_context *ctx) {
        switch (ctx->msg->function) {
        case epoc::etel_phone_init:
            init(ctx);
            break;

        case epoc::etel_phone_get_status:
            get_status(ctx);
            break;

        case epoc::etel_phone_enumerate_lines:
            enumerate_lines(ctx);
            break;

        case epoc::etel_phone_get_line_info:
            get_line_info(ctx);
            break;

        case epoc::etel_mobile_phone_get_indicators_cap:
            get_indicator_caps(ctx);
            break;

        case epoc::etel_mobile_phone_get_indicator:
            get_indicator(ctx);
            break;

        default:
            LOG_ERROR("Unimplemented etel phone opcode {}", ctx->msg->function);
            break;
        }
    }

    etel_phone::etel_phone(const epoc::etel_phone_info &info)
        : info_(info) {
        // Initialize default mode
        status_.detect_ = epoc::etel_modem_detect_not_present;
        status_.mode_ = epoc::etel_phone_mode_idle;
    }

    etel_phone::~etel_phone() {
    }

    bool etel_phone::init() {
        status_.detect_ = epoc::etel_modem_detect_present;
        return true;
    }
}