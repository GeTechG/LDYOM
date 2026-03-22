# LDYOM2 - GTA San Andreas Mission Editor Architecture

## Project Overview

LDYOM2 is a sophisticated GTA San Andreas mission editor and game scripting engine - a 32-bit ASI plugin (C++23) that hooks into the game rendering pipeline and provides a complete UI framework for mission design.

### Key Characteristics
- Type: GTA San Andreas Game Plugin
- Language: C++23
- Architecture: Modular, entity-component-objective
- UI: ImGui with DirectX 9
- Scripting: LuaJIT 2 with sol2
- Build: CMake + Conan
- Platform: Windows 32-bit

## High-Level Architecture

Layered architecture: Rendering Layer → Application Managers → Project/Scene Management → Execution Runtime → GTA API

## Entry Point Flow

1. Plugin Initialization (ldyom.cpp): Game hooks detected, LDYOM initializes on mission pack ID = 7
2. Application Init: Sequential manager initialization, component/objective registration
3. Game Loop: Application::process() each frame, Application::renderFrames() on render

## Core Managers (src/core/)

Application: ProjectsManager, ScenesManager, LuaManager, WindowManager, Hotkeys

Content: ComponentsManager, ObjectivesManager, EntitiesManager, ModelsManager, TexturesManager, AddonsManager

Execution: ProjectPlayer, TaskManager, Settings, Localization

## Data Model (src/data/)

Hierarchy:
- Project → Scenes → Scene → {Entities, Objectives}
- Entity → Components (position, rotation, scale)
- Objective → Type-erased data container

Key Classes:
- Entity: Component container, transform, JSON serializable
- Component: Base class for behaviors (editorRender, onStart, onUpdate, onReset)
- Objective: Type-erased with editor (ImGui) and executor (coroutine) callbacks

## Components (src/data/components/)

Entity Components: Actor (CPed), Vehicle (CVehicle), Object (CObject), Particle, Checkpoint, Pickup

Actor Sub-Components: Animation, Behaviour, DrivePath, MoveByPoints, SimpleTask, RandomSpawn

## Objectives (src/data/objectives/)

Core: Checkpoint, Cutscene, Clock, Countdown, DestroyVehicle, EnterToVehicle, KillActorGang, MoneyAdd/Substract, InteractWithObject, WaitSignal

Two-Callback Design: Editor callback (ImGui) + Executor callback (coroutine)

## Project/Scene Architecture

ProjectInfo: metadata, filesystem path, uuid, startSceneId

Scene: SceneInfo, SceneSettings, Entities[], Objectives[]

Manager: Load/Unload/Reset/Save from/to JSON

## Lua Integration

LuaManager: Thread-safe sol::state wrapper with StateGuard RAII locking

sol2 Bindings: Components and Objectives expose Lua types

## Rendering & UI

ImGui Hook: Hooks IDirect3DDevice9::EndScene(), uses MinHook

Window System: Registry of ImGui windows managed by WindowManager

Types: ProjectManager, MainMenu, Entities, Objectives, Popups, InGameRendering

## Project Playback

ProjectPlayer: Mission execution engine

Flow: Load scene → Spawn entities → Execute objectives sequentially → Complete/Stop

Coroutines: ktwait wrapper, TaskManager, co_await yields each frame

## Critical Design Patterns

1. Type Erasure: Template-based for objectives
2. Component Registration: Builder functions for dynamic loading
3. Dirty Flags: Track changes for GTA sync
4. Transform Callbacks: Bind entities to GTA objects
5. Thread-Safe Lua: RAII guard with shared mutex

## Build & Development

CMake: C++23, MSVC, x86 32-bit, outputs .asi plugin

Dependencies: plugin, minhook, imgui, i18ncpp, spdlog, stduuid, watcher, glm, sol2, LuaJIT

Build: conan install, cmake configure, cmake build

## Documentation Resources
- sa.json - docs for opcodes

Use this when working with low-level GTA scripting operations or implementing game-specific functionality.

## File Organization

src/ldyom.cpp | core/ (managers) | data/ (structures, components, objectives) | rendering/ (UI) | imgui_hook/ (DirectX) | lua/ | utils/ | gta_data/

## Key Insights

1. Singleton Pattern: All managers
2. Editor/Runtime Separation: ImGui vs game logic
3. Coroutine-Based Async: C++20 coroutines
4. Extensibility: Registry factories
5. Type Safety: Templates + runtime checking
6. JSON Serialization: Round-trip support

## Summary

LDYOM2 is a well-architected mission editor combining user-friendly UI editing with low-level GTA script execution through modularity, composability, extensibility, type safety, and asynchronous coroutine execution.
- always build Debug
- for learn dyom source use dyom.txt, DO NOT READ file full only grep
- localization is located in ../ingame/languages/en.json (relative to LDYOM2 project root)
- declarative Lua files for LDYOM are stored in ../ingame/definitions/core (relative to LDYOM2 project root)
- When adding a new binding or updating one for Lua, don't forget to update the declaration
- core ldyom addon stored in ../ingame/addons/core (relative to LDYOM2 project root)
- Addons have their own localization files in the ./languages folder within the addon's root directory