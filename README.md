# PETIT PROGRAM

PAC-MAN meets Pong meets Super Smash Bros. ... with a lot of imagination.

A small C++ SDL2 arcade-like game, most batteries included:

![Petit Program Screenshot](media/petit_program.png)

- Levels, Sprites and Font defined as inline code
- Amazing AI - behold Skynet
- Only depends on SDL2 & C++ 17
- Super-Mini unit test definition included (enable via `WITH_TESTS` CMake Option)
- Super-Mini EC(S) included (using OOP, not DOD)
- Tuple-based Service-Locator
- In source SDL build

# Building

Requirements:

- Build-System: CMake 3.17 or newer
- Compiler: gcc / clang / msvc (with C++17 support)

## Linux & MacOS

```shell
mkdir build
cd build
cmake ..
cmake --build .
```

## Windows

```shell
md build
cd build
cmake ..
cmake --build .
```

# How to play?

## Keys

- Move player via `Left`,`Top`,`Bottom`,`Right` keys.
- Quit via `Escape`
- Toggle Fullscreen- and Window-Mode via `ALT+RETURN`

## Rules

- Your goal is to collect more points than the opponent.
- You may not touch the opponent during the collection phase.
- If no points are left on the screen either:
  - you will be eaten if it collected more or equal points
  - you can eat the opponent and move to the next level, if you collected more points

# Modding

## Adding Levels 
Open up `levels.h`. Change the `LEVEL_COUNT`,
copy paste a level, and change the `#` and ` ` characters.
You just added a new level. Just make sure to always
keep each level's size 8x4 characters.

## Changing Sprites

All the characters live in the `resources.h`.
They work similar to the levels: each character represents an
entry in either of the palettes (`palette1` & `palette2`).

Please note that the palettes are not dynamic. The sprite
textures are created at the start of the game.

Characters and numbers are also just sprites following
the same pattern.

# TODO

- Main Menu
- GamePad support
- 2 Player Mode
- Cleanup
  - Pull sprite animation state up to component
  - Pull position into component
  - Cache component access
- Sound  

# License

[Compare LICENSE.md](LICENSE.md)