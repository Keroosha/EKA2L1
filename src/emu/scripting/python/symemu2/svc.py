# Copyright (c) 2018 EKA2L1 Team.
# 
# This file is part of EKA2L1 project 
# (see bentokun.github.com/EKA2L1).
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from enum import Enum

class Epoc9Svc(Enum):
    WaitForAnyRequest = 0x00800000
    Heap = 0x00800001
    HeapSwitch = 0x00800002
    ActiveScheduler = 0x00800005
    SetActiveScheduler = 0x00800006
    TrapHandler = 0x00800008
    SetTrapHandler = 0x00800009
    DebugMask = 0x0080000C
    DebugMaskIndex = 0x0080000D
    UserSvrRomHeaderAddress = 0x00800013
    UserSvrRomRootDirAddress = 0x00800014
    SafeInc = 0x00800015
    UTCOffset = 0x00800019
    GetGlobalUserData = 0x0080001A
    ObjectNext = 0x00
    ChunkBase = 0x01
    ChunkMaxSize = 0x03
    LibraryLookup = 0x0E
    ProcessFilename = 0x16
    ProcessSetPriority = 0x1C
    ProcessSetFlags = 0x1E
    ServerReceive = 0x22
    SetSessionPtr = 0x24
    SessionShare = 0x27
    ThreadResume = 0x28
    ThreadSetFlags = 0x2F
    HandleName = 0x3C
    MessageComplete = 0x42
    SessionSendSync = 0x4D 
    DllTls = 0x4E
    HalFunction = 0x4F
    ProcessCmdLineLength = 0x52
    DebugPrint = 0x56
    ThreadContext = 0x5E
    ProcessGetMemoryInfo = 0x5F
    HandleClose = 0x6A
    ProcessType = 0x64
    ThreadCreate = 0x68
    ChunkCreate = 0x6B
    ChunkAdjust = 0x6C
    HandleOpenObject = 0x6D
    HandleDuplicate = 0x6E
    SemaphoreCreate = 0x70
    ThreadKill = 0x73
    ThreadLogon = 0x74
    ThreadLogonCancel = 0x75
    DllSetTls = 0x76
    DllFreeTLS = 0x77
    ThreadRename = 0x78
    ProcessResume = 0x7A
    ProcessLogon = 0x7B
    ProcessLogonCancel = 0x7C
    ThreadProcess = 0x7D
    ServerCreate = 0x7E
    SessionCreate = 0x7F
    TimerCreate = 0x84
    ChangeNotifierCreate = 0x87
    WaitDllLock = 0x9C
    ReleaseDllLock = 0x9D
    LibraryAttach = 0x9E
    LibraryAttached = 0x9F
    StaticCallList = 0xA0
    LastThreadHandle = 0xA3
    ThreadRendezvous = 0xA4
    ProcessRendezvous = 0xA5
    MessageGetDesLength = 0xA7
    MessageIpcCopy = 0xA8
    MessageKill = 0xAC
    PropertyDefine = 0xBC
    PropertyAttach = 0xBE
    PropertySubscribe = 0xBF
    PropertyCancel = 0xC0
    PropertyGetInt = 0xC1
    PropertyGetBin = 0xC2
    PropertySetInt = 0xC3
    PropertySetBin = 0xC4
    PropertyFindGetInt = 0xC5
    PropertyFindGetBin = 0xC6
    PropertyFindSetInt = 0xC7
    PropertyFindSetBin = 0xC8
    ProcessGetDataParameter = 0xD1
    ProcessDataParameterLength = 0xD2
    LeaveStart = 0xDF
    LeaveEnd = 0xE0

    def __int__(self):
        return self.value
