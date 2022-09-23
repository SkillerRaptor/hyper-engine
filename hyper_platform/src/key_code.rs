/*
 * Copyright (c) 2022, SkillerRaptor <skillerraptor@protonmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

use glfw::Key;

#[derive(Debug)]
pub enum KeyCode {
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,

    Num0 = 48,
    Num1 = 49,
    Num2 = 50,
    Num3 = 51,
    Num4 = 52,
    Num5 = 53,
    Num6 = 54,
    Num7 = 55,
    Num8 = 56,
    Num9 = 57,

    Semicolon = 59,
    Equal = 61,

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,

    World1 = 161,
    World2 = 162,

    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,

    KeyPad0 = 320,
    KeyPad1 = 321,
    KeyPad2 = 322,
    KeyPad3 = 323,
    KeyPad4 = 324,
    KeyPad5 = 325,
    KeyPad6 = 326,
    KeyPad7 = 327,
    KeyPad8 = 328,
    KeyPad9 = 329,
    KeyPadDecimal = 330,
    KeyPadDivide = 331,
    KeyPadMultiply = 332,
    KeyPadSubtract = 333,
    KeyPadAdd = 334,
    KeyPadEnter = 335,
    KeyPadEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,

    Unknown = -1,
}

impl From<Key> for KeyCode {
    fn from(key: Key) -> Self {
        match key {
            Key::Space => KeyCode::Space,
            Key::Apostrophe => KeyCode::Apostrophe,
            Key::Comma => KeyCode::Comma,
            Key::Minus => KeyCode::Minus,
            Key::Period => KeyCode::Period,
            Key::Slash => KeyCode::Slash,
            Key::Num0 => KeyCode::Num0,
            Key::Num1 => KeyCode::Num1,
            Key::Num2 => KeyCode::Num2,
            Key::Num3 => KeyCode::Num3,
            Key::Num4 => KeyCode::Num4,
            Key::Num5 => KeyCode::Num5,
            Key::Num6 => KeyCode::Num6,
            Key::Num7 => KeyCode::Num7,
            Key::Num8 => KeyCode::Num8,
            Key::Num9 => KeyCode::Num9,
            Key::Semicolon => KeyCode::Semicolon,
            Key::Equal => KeyCode::Equal,
            Key::A => KeyCode::A,
            Key::B => KeyCode::B,
            Key::C => KeyCode::C,
            Key::D => KeyCode::D,
            Key::E => KeyCode::E,
            Key::F => KeyCode::F,
            Key::G => KeyCode::G,
            Key::H => KeyCode::H,
            Key::I => KeyCode::I,
            Key::J => KeyCode::J,
            Key::K => KeyCode::K,
            Key::L => KeyCode::L,
            Key::M => KeyCode::M,
            Key::N => KeyCode::N,
            Key::O => KeyCode::O,
            Key::P => KeyCode::P,
            Key::Q => KeyCode::Q,
            Key::R => KeyCode::R,
            Key::S => KeyCode::S,
            Key::T => KeyCode::T,
            Key::U => KeyCode::U,
            Key::V => KeyCode::V,
            Key::W => KeyCode::W,
            Key::X => KeyCode::X,
            Key::Y => KeyCode::Y,
            Key::Z => KeyCode::Z,
            Key::LeftBracket => KeyCode::LeftBracket,
            Key::Backslash => KeyCode::Backslash,
            Key::RightBracket => KeyCode::RightBracket,
            Key::GraveAccent => KeyCode::GraveAccent,
            Key::World1 => KeyCode::World1,
            Key::World2 => KeyCode::World2,
            Key::Escape => KeyCode::Escape,
            Key::Enter => KeyCode::Enter,
            Key::Tab => KeyCode::Tab,
            Key::Backspace => KeyCode::Backspace,
            Key::Insert => KeyCode::Insert,
            Key::Delete => KeyCode::Delete,
            Key::Right => KeyCode::Right,
            Key::Left => KeyCode::Left,
            Key::Down => KeyCode::Down,
            Key::Up => KeyCode::Up,
            Key::PageUp => KeyCode::PageUp,
            Key::PageDown => KeyCode::PageDown,
            Key::Home => KeyCode::Home,
            Key::End => KeyCode::End,
            Key::CapsLock => KeyCode::CapsLock,
            Key::ScrollLock => KeyCode::ScrollLock,
            Key::NumLock => KeyCode::NumLock,
            Key::PrintScreen => KeyCode::PrintScreen,
            Key::Pause => KeyCode::Pause,
            Key::F1 => KeyCode::F1,
            Key::F2 => KeyCode::F2,
            Key::F3 => KeyCode::F3,
            Key::F4 => KeyCode::F4,
            Key::F5 => KeyCode::F5,
            Key::F6 => KeyCode::F6,
            Key::F7 => KeyCode::F7,
            Key::F8 => KeyCode::F8,
            Key::F9 => KeyCode::F9,
            Key::F10 => KeyCode::F10,
            Key::F11 => KeyCode::F11,
            Key::F12 => KeyCode::F12,
            Key::F13 => KeyCode::F13,
            Key::F14 => KeyCode::F14,
            Key::F15 => KeyCode::F15,
            Key::F16 => KeyCode::F16,
            Key::F17 => KeyCode::F17,
            Key::F18 => KeyCode::F18,
            Key::F19 => KeyCode::F19,
            Key::F20 => KeyCode::F20,
            Key::F21 => KeyCode::F21,
            Key::F22 => KeyCode::F22,
            Key::F23 => KeyCode::F23,
            Key::F24 => KeyCode::F24,
            Key::F25 => KeyCode::F25,
            Key::Kp0 => KeyCode::KeyPad0,
            Key::Kp1 => KeyCode::KeyPad1,
            Key::Kp2 => KeyCode::KeyPad2,
            Key::Kp3 => KeyCode::KeyPad3,
            Key::Kp4 => KeyCode::KeyPad4,
            Key::Kp5 => KeyCode::KeyPad5,
            Key::Kp6 => KeyCode::KeyPad6,
            Key::Kp7 => KeyCode::KeyPad7,
            Key::Kp8 => KeyCode::KeyPad8,
            Key::Kp9 => KeyCode::KeyPad9,
            Key::KpDecimal => KeyCode::KeyPadDecimal,
            Key::KpDivide => KeyCode::KeyPadDivide,
            Key::KpMultiply => KeyCode::KeyPadMultiply,
            Key::KpSubtract => KeyCode::KeyPadSubtract,
            Key::KpAdd => KeyCode::KeyPadAdd,
            Key::KpEnter => KeyCode::KeyPadEnter,
            Key::KpEqual => KeyCode::KeyPadEqual,
            Key::LeftShift => KeyCode::LeftShift,
            Key::LeftControl => KeyCode::LeftControl,
            Key::LeftAlt => KeyCode::LeftAlt,
            Key::LeftSuper => KeyCode::LeftSuper,
            Key::RightShift => KeyCode::RightShift,
            Key::RightControl => KeyCode::RightControl,
            Key::RightAlt => KeyCode::RightAlt,
            Key::RightSuper => KeyCode::RightSuper,
            Key::Menu => KeyCode::Menu,
            Key::Unknown => KeyCode::Unknown,
        }
    }
}
