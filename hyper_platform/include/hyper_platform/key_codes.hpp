/*
 * Copyright (c) 2024, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

namespace hyper_engine
{
    enum class KeyCode : uint32_t
    {
        Unknown = 0x00000000u,
        Return = 0x0000000du,
        Escape = 0x0000001bu,
        Backspace = 0x00000008u,
        Tab = 0x00000009u,
        Space = 0x00000020u,
        Exclaim = 0x00000021u,
        DblApostrophe = 0x00000022u,
        Hash = 0x00000023u,
        Dollar = 0x00000024u,
        Percent = 0x00000025u,
        Ampersand = 0x00000026u,
        Apostrophe = 0x00000027u,
        LeftParen = 0x00000028u,
        RightParen = 0x00000029u,
        Asterisk = 0x0000002au,
        Plus = 0x0000002bu,
        Comma = 0x0000002cu,
        Minus = 0x0000002du,
        Period = 0x0000002eu,
        Slash = 0x0000002fu,
        K0 = 0x00000030u,
        K1 = 0x00000031u,
        K2 = 0x00000032u,
        K3 = 0x00000033u,
        K4 = 0x00000034u,
        K5 = 0x00000035u,
        K6 = 0x00000036u,
        K7 = 0x00000037u,
        K8 = 0x00000038u,
        K9 = 0x00000039u,
        Colon = 0x0000003au,
        Semicolon = 0x0000003bu,
        Less = 0x0000003cu,
        Equals = 0x0000003du,
        Greater = 0x0000003eu,
        Question = 0x0000003fu,
        At = 0x00000040u,
        LeftBracket = 0x0000005bu,
        Backslash = 0x0000005cu,
        RightBracket = 0x0000005du,
        Caret = 0x0000005eu,
        Underscore = 0x0000005fu,
        Grave = 0x00000060u,
        A = 0x00000061u,
        B = 0x00000062u,
        C = 0x00000063u,
        D = 0x00000064u,
        E = 0x00000065u,
        F = 0x00000066u,
        G = 0x00000067u,
        H = 0x00000068u,
        I = 0x00000069u,
        J = 0x0000006au,
        K = 0x0000006bu,
        L = 0x0000006cu,
        M = 0x0000006du,
        N = 0x0000006eu,
        O = 0x0000006fu,
        P = 0x00000070u,
        Q = 0x00000071u,
        R = 0x00000072u,
        S = 0x00000073u,
        T = 0x00000074u,
        U = 0x00000075u,
        V = 0x00000076u,
        W = 0x00000077u,
        X = 0x00000078u,
        Y = 0x00000079u,
        Z = 0x0000007au,
        LeftBrace = 0x0000007bu,
        Pipe = 0x0000007cu,
        RightBrace = 0x0000007du,
        Tilde = 0x0000007eu,
        Delete = 0x0000007fu,
        PlusMinus = 0x000000b1u,
        CapsLock = 0x40000039u,
        F1 = 0x4000003au,
        F2 = 0x4000003bu,
        F3 = 0x4000003cu,
        F4 = 0x4000003du,
        F5 = 0x4000003eu,
        F6 = 0x4000003fu,
        F7 = 0x40000040u,
        F8 = 0x40000041u,
        F9 = 0x40000042u,
        F10 = 0x40000043u,
        F11 = 0x40000044u,
        F12 = 0x40000045u,
        PrintScreen = 0x40000046u,
        ScrollLock = 0x40000047u,
        Pause = 0x40000048u,
        Insert = 0x40000049u,
        Home = 0x4000004au,
        PageUp = 0x4000004bu,
        End = 0x4000004du,
        PageDown = 0x4000004eu,
        Right = 0x4000004fu,
        Left = 0x40000050u,
        Down = 0x40000051u,
        Up = 0x40000052u,
        NumLockClear = 0x40000053u,
        KpDivide = 0x40000054u,
        KpMultiply = 0x40000055u,
        KpMinus = 0x40000056u,
        KpPlus = 0x40000057u,
        KpEnter = 0x40000058u,
        Kp1 = 0x40000059u,
        Kp2 = 0x4000005au,
        Kp3 = 0x4000005bu,
        Kp4 = 0x4000005cu,
        Kp5 = 0x4000005du,
        Kp6 = 0x4000005eu,
        Kp7 = 0x4000005fu,
        Kp8 = 0x40000060u,
        Kp9 = 0x40000061u,
        Kp0 = 0x40000062u,
        KpPeriod = 0x40000063u,
        Application = 0x40000065u,
        Power = 0x40000066u,
        KpEquals = 0x40000067u,
        F13 = 0x40000068u,
        F14 = 0x40000069u,
        F15 = 0x4000006au,
        F16 = 0x4000006bu,
        F17 = 0x4000006cu,
        F18 = 0x4000006du,
        F19 = 0x4000006eu,
        F20 = 0x4000006fu,
        F21 = 0x40000070u,
        F22 = 0x40000071u,
        F23 = 0x40000072u,
        F24 = 0x40000073u,
        Execute = 0x40000074u,
        Help = 0x40000075u,
        Menu = 0x40000076u,
        Select = 0x40000077u,
        Stop = 0x40000078u,
        Again = 0x40000079u,
        Undo = 0x4000007au,
        Cut = 0x4000007bu,
        Copy = 0x4000007cu,
        Paste = 0x4000007du,
        Find = 0x4000007eu,
        Mute = 0x4000007fu,
        VolumeUp = 0x40000080u,
        VolumeDown = 0x40000081u,
        KpComma = 0x40000085u,
        KpEqualsAs400 = 0x40000086u,
        AltErase = 0x40000099u,
        SysReq = 0x4000009au,
        Cancel = 0x4000009bu,
        Clear = 0x4000009cu,
        Prior = 0x4000009du,
        Return2 = 0x4000009eu,
        Separator = 0x4000009fu,
        Out = 0x400000a0u,
        Oper = 0x400000a1u,
        ClearAgain = 0x400000a2u,
        CrSel = 0x400000a3u,
        ExSel = 0x400000a4u,
        Kp00 = 0x400000b0u,
        Kp000 = 0x400000b1u,
        ThousandsSeparator = 0x400000b2u,
        DecimalSeparator = 0x400000b3u,
        CurrencyUnit = 0x400000b4u,
        CurrencySubunit = 0x400000b5u,
        KpLeftParen = 0x400000b6u,
        KpRightParen = 0x400000b7u,
        KpLeftBrace = 0x400000b8u,
        KpRightBrace = 0x400000b9u,
        KpTab = 0x400000bau,
        KpBackspace = 0x400000bbu,
        KpA = 0x400000bcu,
        KpB = 0x400000bdu,
        KpC = 0x400000beu,
        KpD = 0x400000bfu,
        KpE = 0x400000c0u,
        KpF = 0x400000c1u,
        KpXor = 0x400000c2u,
        KpPower = 0x400000c3u,
        KpPercent = 0x400000c4u,
        KpLess = 0x400000c5u,
        KpGreater = 0x400000c6u,
        KpAmpersand = 0x400000c7u,
        KpDblAmpersand = 0x400000c8u,
        KpVerticalBar = 0x400000c9u,
        KpDblVerticalBar = 0x400000cau,
        KpColon = 0x400000cbu,
        KpHash = 0x400000ccu,
        KpSpace = 0x400000cdu,
        KpAt = 0x400000ceu,
        KpExclam = 0x400000cfu,
        KpMemStore = 0x400000d0u,
        KpMemRecall = 0x400000d1u,
        KpMemClear = 0x400000d2u,
        KpMemAdd = 0x400000d3u,
        KpMemSubtract = 0x400000d4u,
        KpMemMultiply = 0x400000d5u,
        KpMemDivide = 0x400000d6u,
        KpPlusMinus = 0x400000d7u,
        KpClear = 0x400000d8u,
        KpClearEntry = 0x400000d9u,
        KpBinary = 0x400000dau,
        KpOctal = 0x400000dbu,
        KpDecimal = 0x400000dcu,
        KpHexadecimal = 0x400000ddu,
        LCtrl = 0x400000e0u,
        LShift = 0x400000e1u,
        LAlt = 0x400000e2u,
        LGui = 0x400000e3u,
        RCtrl = 0x400000e4u,
        RShift = 0x400000e5u,
        RAlt = 0x400000e6u,
        RGui = 0x400000e7u,
        Mode = 0x40000101u,
        Sleep = 0x40000102u,
        Wake = 0x40000103u,
        ChannelIncrement = 0x40000104u,
        ChannelDecrement = 0x40000105u,
        MediaPlay = 0x40000106u,
        MediaPause = 0x40000107u,
        MediaRecord = 0x40000108u,
        MediaFastForward = 0x40000109u,
        MediaRewind = 0x4000010au,
        MediaNextTrack = 0x4000010bu,
        MediaPreviousTrack = 0x4000010cu,
        MediaStop = 0x4000010du,
        MediaEject = 0x4000010eu,
        MediaPlayPause = 0x4000010fu,
        MediaSelect = 0x40000110u,
        AcNew = 0x40000111u,
        AcOpen = 0x40000112u,
        AcClose = 0x40000113u,
        AcExit = 0x40000114u,
        AcSave = 0x40000115u,
        AcPrint = 0x40000116u,
        AcProperties = 0x40000117u,
        AcSearch = 0x40000118u,
        AcHome = 0x40000119u,
        AcBack = 0x4000011au,
        AcForward = 0x4000011bu,
        AcStop = 0x4000011cu,
        AcRefresh = 0x4000011du,
        AcBookmarks = 0x4000011eu,
        SoftLeft = 0x4000011fu,
        SoftRight = 0x40000120u,
        Call = 0x40000121u,
        EndCall = 0x40000122u
    };
} // namespace hyper_engine
