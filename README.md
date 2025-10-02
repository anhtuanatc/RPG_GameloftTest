# RPG Gameloft Test

## 🎮 Gameplay Overview

The game is a **third-person RPG prototype** with basic combat, skills, and AI enemies.

### Player Controls

**PC (Windows):**

* `WASD` → Move character
* `C` → Toggle Walk/Run
* `1` / `2` → Switch Weapon
* `Left Mouse` → Use Skill 1
* `Right Mouse` → Use Skill 2
* `P` → Pause Menu

**Android (Mobile):**

* Virtual Joystick (bottom-left) → Move character
* Button **Skill 1** → Melee / Fireball
* Button **Skill 2** → Alternate skill
* Button **Weapon Switch 1 / 2  ** → Switch between Sword and Staff

### Gameplay Loop

* Explore the map (mountains, lakes, obstacles).
* Fight enemies (melee and ranged).
* Survive as long as possible.
* Enemies spawn at spawn points with visual effects and will chase/attack the player.
* If player HP reaches 0 → Death animation → End Menu is displayed.

---

## ⚙️ Installation & Build Instructions

### Requirements

* **Unreal Engine 5.5**
* **Visual Studio 2022** with C++ components
* **Windows 10/11 64-bit**
* (Optional) **Android SDK & NDK** if building for Android

### Setup

1. Clone or download this repository.
2. Open `RPG_GameloftTest.uproject`.
3. If prompted, right-click `.uproject` → **Generate Visual Studio project files**.
4. Open the `.sln` file in **Visual Studio 2022**.

### Build (Windows)

1. In Visual Studio, set **Configuration** = `Development Editor`, **Target** = `Win64`.
2. Build solution (`Ctrl + Shift + B`).
3. Open Unreal Editor → click **Play**.

### Build (Android)

1. In Unreal Editor, go to **Project Settings → Platforms → Android**.
2. Verify **SDK/NDK/JDK** paths.
3. From menu: **File → Package Project → Android (ASTC)**.
4. Generated `.apk` will be in `Saved/StagedBuilds/Android_ASTC/`.

---

## 📝 Notes

* First build may take time due to shader compilation.

---

