[![CMake-All-Build](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-all-build.yml/badge.svg)](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-all-build.yml) [![CMake-Unit-Test](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-unit-test.yml/badge.svg)](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-unit-test.yml)

![PAX SAPIENTICA Logo](./Image/Logo/TitleBanner4.svg)

![Screenshot](./Image/Screenshot/Best.png)

# Overview

This software is PAX SAPIENTICA (abbreviated PAXS).
PAXS is a GIS for philological, archaeological, and anthropological data.
It has the ability to display calendars for each region in each time period and agent-based simulation.

# How to Install

This software has both a GUI and a CUI.

### GUI

The GUI supports the following libraries

* OpenSiv3D (tested with v0.6.10)
* DxLib (tested on Windows and Android versions)
* SFML

Any of the above libraries will launch the GUI software.

### CUI

To try this library, build cmake.

> [!TIP]
> For example, clone this repository. You can open the cloned directory with VS Code and build it with CMake.

# Code Documentation

https://aspjt.github.io/PAX_SAPIENTICA/

# List of elements supported by this software.

## 📅 Calendar

|Available|Calendar Name|
|:---:|:---|
|✅|Julian calendar|
|✅|Gregorian calendar|
|✅|Japanese calendar|
|✅|Chinese calendar|
|✅|Islamic calendar|
|✅|Julian day number|

![Calendar](./Image/Other/Calendar20230709.gif)

## 🏠 Agent-Based Simulation

This is an agent-based simulation that estimates settlement and population dynamics in the Japanese archipelago from the 3rd to 7th centuries.
It was presented at the 48th Annual Meeting of the Japan Association for Archaeoinformatics on December 2, 2023.

![PAX SAPIENTICA Simulation](./Image/Animation/ABS01.gif)

## 🧬 DNA haplogroup

This software can handle DNA haplogroup data of ancient human bones.

The following GIF animation shows the changes in East Asian mtDNA haplogroups from 1 Julian day (4713 BC) to the present.

![PAX SAPIENTICA Mitochondrial DNA](./Image/Animation/mtDNA02.gif)

Ancient human bone mtDNA data registered as of December 21, 2023.

![PAX SAPIENTICA mtDNA](./Image/Screenshot/Screenshot%202023-12-21%2022.00.17.png)

## 👤 Movement of a person

This software allows you to visualize and move historical figures.

![PAX SAPIENTICA Movement](./Image/Animation/Person01.gif)

---

# Supported Environment

### Compilers

* GNU Compiler Collection (GCC)
* Clang
* Microsoft Visual C++ (MSVC)

### Language Version

C++17 or later

### Build Tools

* CMake (e.g. Visual Studio Code)
* .sln / Microsoft Visual Studio
* Android Studio

### Android Studio

This library has been tested to work in a mobile environment.

> [!IMPORTANT]
> The Android version of DxLib, a wrapper library for drawing APIs, is used.
> Drawing APIs such as OpenGL and Vulkan are not directly supported.

![Android Studio](https://raw.githubusercontent.com/AsPJT/PAX_SAPIENTICA/develop/Image/Screenshot/PAX%20SAPIENTICA%202023-09-23%2020.54.32.png)

---

# Main Developers

|Icon|Name|Main Work|
|:---|:---|:---|
|<a href="https://github.com/AsPJT"><img src="https://avatars.githubusercontent.com/u/30593725" style="width: 96px;"></a>|As Project|● Planning<br>● Design Concepts<br>● Specification Document Preparation|
|<a href="https://github.com/guinpen98"><img src="https://avatars.githubusercontent.com/u/83969826" style="width: 96px;"></a>|guinpen98|● Programming<br>● Simulation<br>● GitHub Actions / CI&CD|

---

# License

These codes in the Library directory are licensed under **CC0 (PUBLIC DOMAIN)**.

Third party content is copyrighted by its respective owners.

[![CC0](https://mirrors.creativecommons.org/presskit/buttons/88x31/svg/cc-zero.svg "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.en)
