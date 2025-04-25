# LDYOM (Lua Design Your Own Mission)

A mission-making platform for Grand Theft Auto: San Andreas. Created as a spiritual successor of [DYOM](https://dyom.gtagames.nl/), it uses C++/Lua external plug-ins and technically is modular in contrast to DYOM's hardcoded features all done in its main script.

Making use of visual scripting with some basic programming and node systems, it allows designers to make non-linear missions (here referred as 'Scenes'/'Projects' starting from 0.8) and is overall very flexible and rich with a wide library of nodes covering a plethora of things, from camera settings and audio control to memory read/write functions, to logical operations.

## Features

- 🎮 Visual scripting system for creating missions
- 📝 Non-linear mission design capabilities
- 🔌 Modular plug-in architecture
- 🎥 Advanced camera control
- 🎵 Audio management
- 🔧 Extensive node library for various operations
- 🌐 Multi-language support

## Work with

- Grand Theft Auto: San Andreas (Hoodlum/Compat)

## Building from Source

### Prerequisites

- CMake 3.28 or higher
- Conan package manager
- Visual Studio 2022 with C++ workload
- Git

### Build Steps

1. Clone the repository:
```bash
git clone https://github.com/GeTechG/LDYOM.git
cd LDYOM
```

2. Install dependencies using Conan:
```bash
cd LDYOM2
conan install . --build=missing
```

3. Configure and build using CMake:
```bash
cmake --preset conan-default
cmake --build --preset conan-release
```

## Development

The project structure:
- `tools/` - Various utility tools
- `resources/` - Project resources
- `LDYOM2/` - Core project files

## Community and Support

* [Documentation](https://getechg.github.io/LDYOM/docs/category/getting-started)
* [Website](https://getechg.github.io/LDYOM/)
* [Telegram Chat](https://t.me/ldyom)
* [Discord Server](https://discord.gg/SqmeF377XV)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Special thanks to:
- The original DYOM team for inspiration
- All contributors and community members
- [Add other acknowledgments]
