# Metal Slug: OOP Project

A 2D side-scrolling run-and-gun game inspired by **Metal Slug**, written in **C++** with **SFML 2.6**.
It was made as an Object Oriented Programming course project (roll numbers **25I-0504** and **25I-0644**). The whole game is built around class hierarchies, inheritance, polymorphism and the state pattern.

![Gameplay](docs/screenshots/day.png)

## Gameplay video

[![Gameplay video](docs/screenshots/video_thumb.png)](docs/gameplay.mp4)

Click the picture to watch the gameplay video (1.5 minutes, with sound). It shows all three missions, the vehicles, the power-ups, the weather and day/night cycle, and the boss fight. The boss's health was lowered for the recording so the whole fight fits in the video.

---

## Features

- **3 missions:**
  1. **Mountain Pass:** rocky hills, then grassy plains. Reach the flag at the end.
  2. **Coastline:** plains, then the sea, with enemy submarines and the Slug Mariner. Reach the flag on the sea bed.
  3. **Rebel Base:** the final boss fight against a giant Rebel Tank at sunset going into night.

  Your score carries over from mission to mission.
- **Boss:** the Rebel Tank has a health bar, cannon bursts, grenade volleys and a ramming charge when it gets angry.
- **Score, combos and a high score** saved to `highscore.txt` and shown on the main menu. Kills in quick succession multiply the points (up to ×5).
- **Enemy waves** appear ahead of you as you advance. The boss mission sends reinforcements.
- **Pickups:**
  - killed enemies drop ammo, grenades or health
  - tied-up **prisoners (POWs)** give 1000 points and a random weapon when freed
- **4 playable characters**: Marco, Tarma, Eri and Fio. Switch between them at any time. Each has 2 lives, different stats and a power-up:
  - Marco: rapid fire
  - Tarma: 3-way spread shot
  - Eri: unlimited grenades
  - Fio: shield
- **Every game has a different random world**, built from a tile (voxel) grid.
- **Weapons:** pistol, Heavy Machine Gun, Rocket Launcher, Flame Shot and Laser Gun pickups, plus grenades and a knife.
- **Enemies with a state-machine AI** (roaming → chasing → attacking):
  - Rebel soldier
  - Bazooka soldier
  - Grenade soldier
  - Shield soldier, who blocks bullets from the front
- **Vehicles you can drive:** Metal Slug tank, Slug Flyer plane, Slug Mariner submarine and Amphibious Slug.
- **Enemy vehicles:** M-15A Bradley tank, Flying Tara bomber and an enemy submarine.
- **Day and night cycle:** the sky changes colour, the sun and moon move across it, and the level gets darker at night.
- **Weather:** clear, rain and snow on a repeating schedule. Rain and snow turn the sky grey.
- **Game feel:**
  - sound effects and background music
  - explosions with screen shake
  - enemies flash when hit and get knocked back when they die
  - muzzle flashes and walking animations
  - rocket splash damage
  - a camera that looks ahead
- **HUD and screens:** mission titles, pause, game over and win screens.

## Screenshots

### Main menu
![Main menu](docs/screenshots/menu.png)

### Missions and boss
| Mission 1: Mountain Pass | Mission 2: Coastline |
|---|---|
| ![Mission 1](docs/screenshots/mission1_title.png) | ![Mission 2](docs/screenshots/mission2_title.png) |
| **Mission 3: the Rebel Tank boss** | **Boss destroyed** |
| ![Boss fight](docs/screenshots/boss_fight.png) | ![Boss destroyed](docs/screenshots/boss_destroyed.png) |

### Score, pickups and power-ups
| Combo multiplier | Freeing a prisoner (+1000 and a weapon) |
|---|---|
| ![Combo](docs/screenshots/combo.png) | ![Prisoner rescue](docs/screenshots/pow_rescue.png) |
| **Fio's shield power-up** | **Winning the game** |
| ![Shield](docs/screenshots/powerup_shield.png) | ![You win](docs/screenshots/you_win.png) |

### Playable characters
Marco, Tarma, Eri and Fio. Press **Z** to switch.
![Characters](docs/screenshots/characters.png)

### Weapons and combat
| Heavy Machine Gun pickup | Grenade explosion |
|---|---|
| ![Heavy Machine Gun](docs/screenshots/weapon_hmg.png) | ![Grenade explosion](docs/screenshots/grenade_explosion.png) |

### Enemies
| Shield soldier and bazooka soldier | Grenade soldier |
|---|---|
| ![Shield and bazooka soldiers](docs/screenshots/enemies_shield_bazooka.png) | ![Grenade soldier](docs/screenshots/enemy_grenade.png) |
| **Flying Tara bomber** | **M-15A Bradley tank** |
| ![Flying Tara](docs/screenshots/enemy_flying_tara.png) | ![Bradley tank](docs/screenshots/enemy_bradley_tank.png) |

### Vehicles
| Metal Slug tank | Slug Flyer plane |
|---|---|
| ![Metal Slug](docs/screenshots/vehicle_metal_slug.png) | ![Slug Flyer](docs/screenshots/vehicle_slug_flyer.png) |
| **Slug Mariner vs. enemy submarine** | |
| ![Slug Mariner](docs/screenshots/vehicle_slug_mariner.png) | |

### Day and night cycle
| Sunset | Night |
|---|---|
| ![Sunset](docs/screenshots/sunset.png) | ![Night with moon](docs/screenshots/night_moon.png) |

### Weather
| Rain (at night) | Snow |
|---|---|
| ![Rain](docs/screenshots/weather_rain_night.png) | ![Snow](docs/screenshots/weather_snow.png) |

### End screens
| Mission complete | Game over |
|---|---|
| ![Mission complete](docs/screenshots/mission_complete.png) | ![Game over](docs/screenshots/game_over.png) |

## Controls

| Key | Action |
|---|---|
| **A / D** | Move left / right |
| **W** | Jump |
| **Space** | Fire |
| **Up / Down** | Aim up / down |
| **T** | Throw grenade |
| **R** | Knife |
| **Q** | Character power-up (different for each character, 20 s cooldown) |
| **Z** | Switch character |
| **E** | Enter a vehicle (stand next to it) |
| **U** | Exit the vehicle |
| **P** | Pause |
| **M** | Mute / unmute sound |
| **Esc** | Back to the menu (quits from the menu) |
| **Enter / Up / Down** | Menu navigation |

**Inside vehicles:**
- Metal Slug: A/D to drive, W to jump, Space for the cannon.
- Flyer, Mariner and Amphibious Slug: arrow keys to move.
- Slug Flyer: R fires missiles.
- Slug Mariner: D, W and A fire missiles forward, up and backward.

## Building and running

### Windows (Visual Studio)
1. Install **SFML 2.6.x** (the Visual C++ 64-bit build) to `C:\SFML-2.6.1`. Or change the include and library folders in the project properties to wherever you put it.
2. Open `OOP_PROJECT.slnx` in Visual Studio (the project uses toolset v145).
3. Select **Debug | x64** and build.
4. Copy these DLLs from `SFML\bin` next to the `.exe`: `sfml-graphics-d-2.dll`, `sfml-window-d-2.dll`, `sfml-system-d-2.dll`, `sfml-audio-d-2.dll` and `openal32.dll` (needed for sound).
5. Run it from Visual Studio. The working directory has to be the `OOP_PROJECT/OOP_PROJECT` folder, because the assets are loaded with relative paths.

### Linux
```bash
sudo apt install libsfml-dev
cd OOP_PROJECT/OOP_PROJECT
g++ -std=c++17 *.cpp -o metalslug -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./metalslug
```

## Project structure

```
OOP_PROJECT/
├── OOP_PROJECT.slnx               Visual Studio solution
└── OOP_PROJECT/
    ├── *.h / *.cpp                game source (one class or class family per file)
    ├── 25I-0504_25I-0644_Assets/  sprites, tiles, menu images, sounds/ (effects and music)
    ├── TEXT/font1.ttf             HUD font
    └── SPRITE/                    original sprite sheets the assets were cut from
```

### Main classes

| Area | Classes |
|---|---|
| Game loop and screens | `Game`, `GameStateManager`, `GameState` → `MenuState`, `PlayState` |
| World and missions | `World` (one layout per mission), `Voxel`, `Camera`, `Level` (spawns and waves), `LevelManager` |
| Entities | `Entity` → `DamagableEntity` → `Soldier` → `PlayerSoldier`; `Player` (the 4 characters) |
| Enemies | `Enemy` → `RebelSoldier`, `ShieldedSoldier`, `BazookaSoldier`, `GrenadeSoldier`, `BossTank` |
| Pickups | `ItemDrop`, `Prisoner` (both inherit `Entity`), `WeaponCollectible` |
| Enemy AI | `EnemyAiState` → `RoamingAround`, `runningState`, `AttackingState`, `Descending` |
| Character states | `TransformativeState` → `NormalState`, `UndeadState`, `MummyState` |
| Vehicles | `Vehicle` → `GroundVehicle` (`MetalSlug`, `M15Bradley`, `AmphibiousSlug`), `AerialVehicle` (`SlugFlyer`, `FlyingTara`), `AquaticVehicle` (`SlugMariner`, `EnemySub`) |
| Weapons | `Weapon` → `ProjectileWeapon` → `Pistol`, `HeavyMachineGun`, `RocketLauncher`, `FlameShot`, `LaserGun`; `WeaponCollectible` |
| Projectiles | `Projectile` → `StraightProjectile` (`Bullet`, `Rocket`, `FireStream`, `LaserBeam`), `BallisticProjectile` (`NormalGrenade`, `FireBombGrenade`) |
| Effects and game systems | `EntityManager`, `Explosion`, `Score`, `SoundManager`, `DayNightCycle`, `Weather` → `ClearWeather`, `Rain`, `Snow`; `WeatherSystem` |

```mermaid
classDiagram
    Entity <|-- DamagableEntity
    Entity <|-- Projectile
    Entity <|-- ItemDrop
    Entity <|-- Prisoner
    DamagableEntity <|-- Soldier
    DamagableEntity <|-- Enemy
    DamagableEntity <|-- Vehicle
    Soldier <|-- PlayerSoldier
    Enemy <|-- RebelSoldier
    Enemy <|-- ShieldedSoldier
    Enemy <|-- BazookaSoldier
    Enemy <|-- GrenadeSoldier
    Enemy <|-- BossTank
    Vehicle <|-- GroundVehicle
    Vehicle <|-- AerialVehicle
    Vehicle <|-- AquaticVehicle
    Projectile <|-- StraightProjectile
    Projectile <|-- BallisticProjectile
    Weapon <|-- ProjectileWeapon
    GameState <|-- MenuState
    GameState <|-- PlayState
    Weather <|-- Rain
    Weather <|-- Snow
    Weather <|-- ClearWeather
```

### OOP concepts used
- **Inheritance:** every object in the game is an `Entity`. Anything that can be hurt is a `DamagableEntity`, which owns the HP, gravity and tile collision code.
- **Polymorphism:** `EntityManager` keeps arrays of base-class pointers (`Enemy*`, `Vehicle*`, `Projectile*`) and calls virtual `update()`, `render()` and `applyDamage()` on them. The same applies to `Weather*` and `GameState*`.
- **Abstract classes:** `Entity`, `Projectile`, `Weapon`, `Vehicle`, `GameState`, `EnemyAiState` and `Weather` have pure virtual functions.
- **State pattern:**
  - screens: `GameState`
  - enemy AI: `EnemyAiState`
  - character transformations: `TransformativeState`
- **Encapsulation and composition:** `Player` owns 4 `PlayerSoldier`s, `Soldier` owns its `Weapon`s, `PlayState` owns the `World`, `EntityManager`, `Score`, `DayNightCycle` and `WeatherSystem`.
- **Overriding:** for example `BossTank` replaces `update`, `render`, `die` and `throwProjectile`, while the base `Enemy` code still handles its damage, score and collisions.
- **File handling:** the high score is read and written with `ifstream` / `ofstream`.

## Credits

- Metal Slug characters, enemies, vehicles and effects are © SNK. The sprites come from fan-ripped Metal Slug sprite sheets (rippers are credited inside the sheets in the `SPRITE` folder).
- Sound effects and music were generated specifically for this game.
- This is a non-commercial student project made only for learning.
