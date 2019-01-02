/*
 * Copyright (c) 1994-2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (c) 2019 EKA2L1 Team
 * 
 * This file is part of EKA2L1 project / Symbian Source Code
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

#pragma once

#include <common/vecx.h>
#include <cstdint>

namespace eka2l1::epoc {
    enum class graphics_orientation { 
        normal,
        rotated90, 
        rotated180,
        rotated270 
    };

    /*! \brief Screen display mode.
     *
     * Depend on the display mode, the bitmap sends it will have the specified attribute.
    */
    enum class display_mode {
        none,
        gray2,          ///< Monochrome display mode (1 bpp)
        gray4,          ///< Four grayscales display mode (2 bpp)
        gray16,         ///< 16 grayscales display mode (4 bpp) 
        gray256,        ///< 256 grayscales display mode (8 bpp) 
        color16,        ///< Low colour EGA 16 colour display mode (4 bpp) 
        color256,       ///< 256 colour display mode (8 bpp) 
        color64k,       ///< 64,000 colour display mode (16 bpp) 
        color16m,       ///< True colour display mode (24 bpp) 
        rgb,            ///< (Not an actual display mode used for moving buffers containing bitmaps)
        color4k,        ///< 4096 colour display (12 bpp). 
        color16mu,      ///< True colour display mode (32 bpp, but top byte is unused and unspecified) 
        color16ma,      ///< Display mode with alpha (24bpp colour plus 8bpp alpha) 
        color16map,     ///< Pre-multiplied Alpha display mode (24bpp color multiplied with the alpha channel value, plus 8bpp alpha)
        color_last
    };

    enum class pointer_cursor_mode {
        none,           ///< The device don't have a pointer (touch)
        fixed,          ///< Use the default system cursor
        normal,         ///< Can't understand yet
        window,         ///< Can't understand yet
    };

    enum class window_type {
        redraw,
        backed_up,
        blank
    };

    enum class event_modifier {
        repeatable = 0x001,
        keypad = 0x002,
        left_alt = 0x004,
        right_alt = 0x008,
        alt = 0x010,
        left_ctrl = 0x020,
        right_ctrl = 0x040,
        ctrl = 0x080,
        left_shift = 0x100,
        right_shift = 0x200,
        shift = 0x400,
        left_func = 0x800,
        right_func = 0x1000,
        func = 0x2000,
        caps_lock = 0x4000,
        num_lock = 0x8000,
        scroll_lock = 0x10000,
        key_up = 0x20000,
        special = 0x40000,
        double_click = 0x80000,
        modifier_pure_key_code = 0x100000,
        cancel_rot = 0x200000,
        no_rot = 0x0,
        rotate90 = 0x400000,
        rotate180 = 0x800000,
        rotate270 = 0x1000000,
        all_mods = 0x1FFFFFFF                   
    };
    
    enum class event_type {
        button1down,
        button1up,
        button2down,
        button2up,
        drag,
        move,
        button_repeat,
        repeat,
        switch_on,
        out_of_range
    };

    enum class event_code {
        /** Null event.
        This can be sent, but should be ignored by clients. */
        null,

        /** Key event.
        This is the event that is sent when a character has been received from the
        keyboard.

        If an EEventKey event is associated with an EEventKeyDown or EEventKeyUp
        event (typically EEventKeyDown), the EEventKey event occurs after the
        EEventKeyDown/EEventKeyUp event.

        In practice, the only keys potentially likely to have their EEventKey event
        generated on the up rather than the down are modifier keys. */
        key,

        /** Key up event.
         
        If an EEventKey event is associated with an EEventKeyUp event (which is
        rarely the case), the EEventKey event occurs after the EEventKeyUp event. */
        key_up,
        
        /** Key down event.
        
        If an EEventKey event is associated with an EEventKeyDown event (which
        is typically the case), the EEventKey event occurs after the EEventKeyDown event. */
        key_down,

        /** Modifier changed event.
        
        This is an event generated by the window server when
        the state of one of the modifier keys changes.
        It is not reported unless explicitly requested by a window.
        @see RWindowTreeNode::EnableModifierChangedEvents(). */
        modifier_change,

        /** Pointer event.
        
        This event is sent when the user presses or releases a pointer button (or
        the equivalent action, depending on the type of pointing device), drags the
        pointer, moves it or uses the pointer to switch on the device. */
        touch,
        
        /** Pointer enter event.
	    This occurs when the user moves the pointer into a window with a pointer button
        pressed (or equivalent action depending on the type of pointing device). If
        move events are being generated, this event also occurs when the user moves
        the pointer into the window. */
	    touch_enter,

        /** Pointer exit event.
        Occurs when the user moves the pointer out of a window with a pointer button
        pressed (or equivalent action depending on the type of pointing device). If
        move events are being generated, this event also occurs when the user moves
        the pointer out of the window. */
        touch_exit,

        /** Pointer move buffer ready event.
        Occurs when the pointer move buffer is ready to be retrieved by the client. */
        event_pointer_buffer_ready,
        
        /** Drag and drop */
        drag_and_drop,
        
        /** Focus lost event.
        Occurs when a window group loses keyboard focus. */
        focus_lost,		//10

        /** Focus gained event.
        Occurs when a window group gains keyboard focus. */
        focus_gained,

        /** On event.
        This event type is not reported unless explicitly requested by a window.
        @see RWindowTreeNode::EnableOnEvents(). */
        switch_on,

        /** Password event.
        Occurs when the window server enters password mode. It is sent to the group
        window of the currently active password window.
        This is the window server mode where the user is required to enter a password
        before any further actions can be performed.
        @deprecated	*/
        event_password,

        /** Window group changed event. This occurs whenever a window group is destroyed,
        and whenever a window group's name changes
        This event type is not reported unless explicitly requested by a window.
        @see RWindowTreeNode::EnableGroupChangeEvents(). */
        window_groups_changed,

        /** Error event.
        Occurs when an error occurs. See TWsErrorMessage::TErrorCategory for the types
        of errors.
        This event type is not reported unless explicitly requested by a window.
        @see RWindowTreeNode::EnableErrorMessages(). */
        event_error_msg,		//15

        /** Message ready event.
        Occurs when a session sends a message to this window group using RWsSession::SendMessageToWindowGroup(). */
        event_messages_ready,

        invalid,	// For internal use only

        /** Off event.
        This is issued when an off event is received by the window server from the
        base.
        If for some reason the event can't be delivered, or there is no-one to deliver
        it to, then a call to the base is made to power down the processor.
        This event is only delivered if explicitly requested using RWsSession:RequestOffEvent(). */
        switch_off,

        /** Event issued to off-event requesting windows when the off key is pressed. */
        key_switch_off,

        /** Screen size mode change event.
        This is issued when the screen size mode has changed, for instance when
        the cover on a phone that supports screen flipping is opened or closed. */
        screen_change, //20

        /** Event sent whenever the window group with focus changes.
        Requested by RWindowTreeNode::EnableFocusChangeEvents(). */
        focus_group_changed,

        /** Case opened event.
        This event is sent to those windows that have requested EEventSwitchOn
        events. Unlike with EEventSwitchOn events, the screen will not be switched
        on first. */
        case_opened,

        /** Case closed event.
        This event is sent to those windows that have requested EEventSwitchOff
        events.
        Unlike EEventSwitchOff events, which make a call to the base to power down
        the processor if for some reason the event can't be delivered (or there is
        no-one to deliver it to), failure to deliver case closed events has no repercussions. */
        case_closed,

        /** Window group list change event.
        The window group list is a list of all window groups and their z-order. This
        event indicates any change in the window group list: additions, removals and
        reorderings.
        Notification of this event is requested by calling RWindowTreeNode::EnableGroupListChangeEvents(). */
        group_list_change,

        /** The visibility of a window has changed
        This is sent to windows when they change from visible to invisible, or visa versa, usually due
        to another window obscuring them.
        Notification of this event is requested by calling RWindowTreeNode::EnableVisibilityChangeEvents(). */
        window_visibility_change,

        #ifdef SYMBIAN_PROCESS_MONITORING_AND_STARTUP
        /** Restart event.

        This is issued when an restart event is received by the window server from the 
        base. This event is also an off event, because it might power-cycle the device.

        If for some reason the event can't be delivered, or there is no-one to deliver 
        it to, then a call to the base is made to power down the processor.

        This event is only delivered if explicitly requested using RWsSession:RequestOffEvent(). */
        restart_system,
        #endif

        /** The display state or configuration has changed

        Either change of the current resolution list (state change) or current resolution/background
        (mode change) will trigger this event.

        Notification of this event is requested by calling MDisplayControl::EnableDisplayChangeEvents() 
            */
        display_changed = window_visibility_change + 2,

        //Codes for events only passed into Key Click DLL's
        /** Repeating key event.
        This is only sent to a key click plug-in DLL (if one is present) to indicate
        a repeating key event.
        @see CClickMaker */
        key_repeat=100,

        group_win_open,
        group_win_close,
        
        win_close,

        //Codes for events only passed into anim dlls
        /** Direct screen access begin
        This is only sent to anim dlls (if they register to be notified). It indicates that
        the number of direct screen access sessions has increased from zero to one.*/
        direct_access_begin=200,

        /** Direct screen access end
        This is only sent to anim dlls (if they register to be notified). It indicates that
        the number of direct screen access sessions has decreased from one to zero.*/
        direct_access_end,
        /** Event to signal the starting or stopping of the wserv heartbeat timer
        This is only sent to anim dlls (if they register to be notified). */
        heartbeat_timer_changed,

        //The range 900-999 is reserved for UI Framework events
        /** 900-909 WSERV protects with PowerMgmt */
        power_mgmt = 900,
        reserved = 910,

        //Event codes from EEventUser upwards may be used for non-wserv events.
        //No event codes below this should be defined except by the window server

        /** User defined event.
        The client can use this and all higher values to define their own
        events. These events can be sent between windows of the same client or windows
        of different clients.
        @see RWs::SendEventToWindowGroup(). */
        user=1000,
    };
    
    struct pointer_event {
        event_type evtype;
        event_modifier modifier;
        eka2l1::vec2 pos;
        eka2l1::vec2 parent_pos;
    };

    struct adv_pointer_event: public pointer_event {
        int spare1;
        int spare2;
        int pos_z;
        std::uint8_t ptr_num; // multi touch
    };

    enum pointer_filter_type {
        pointer_enter = 0x01,   ///< In/out
        pointer_move = 0x02,
        pointer_drag = 0x04,
        pointer_simulated_event = 0x08,
        all = pointer_move | pointer_simulated_event
    };

    struct event {
        event_code type;
        std::uint32_t handle;
        std::uint64_t time;

        // TODO: Should be only pointer event with epoc < 9.
        // For epoc9 there shouldnt be a pointer number, since there is no multi touch
        adv_pointer_event evt;
    };

    struct redraw_event {
        std::uint32_t handle;
        vec2 top_left;
        vec2 bottom_right;
    };
}