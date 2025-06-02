# PAX SAPIENTICA [![CMake-All-Build](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-all-build.yml/badge.svg)](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-all-build.yml) [![CMake-Unit-Test](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-unit-test.yml/badge.svg)](https://github.com/AsPJT/PAX_SAPIENTICA/actions/workflows/cmake-unit-test.yml)

![PAX SAPIENTICA Logo](./Images/Logo/TitleBanner4.svg)

![Screenshot](./Images/Screenshot/Best.png)

# Overview

This software is PAX SAPIENTICA (abbreviated PAXS).
PAXS is a GIS for philological, archaeological, and anthropological data.
It has the ability to display calendars for each region in each time period and agent-based simulation.

The phenomenon of history is formed by a complex interplay of time, space, and the actions of the people who lived within them. At the core of PAX SAPIENTICA lies the objective to understand and reproduce this dynamism of history. Specifically, it aims to seamlessly integrate temporal and spatial information, which is indispensable for historical research, and furthermore, to dynamically analyze history as "an accumulation of people's actions" using the agent-based simulation method.

Traditional Geographic Information Systems (GIS), while adept at handling spatial information, have faced challenges in managing complex temporal information crucial for historical research, such as the concept of the time axis, calendars specific to different eras, and archaeological chronologies. Moreover, existing agent-based simulation software has not always been optimized for the specific needs of historical research, for instance, displaying and managing calendrical systems or chronological frameworks of particular periods. PAX SAPIENTICA is designed to overcome these challenges and meet the unique requirements faced by historical researchers.

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

![Calendar](./Images/Other/Calendar20230709.gif)

## 🏠 Agent-Based Simulation (ABS)

This is an agent-based simulation that estimates settlement and population dynamics in the Japanese archipelago from the 3rd to 7th centuries.
It was presented at the 48th Annual Meeting of the Japan Association for Archaeoinformatics on December 2, 2023.

![PAX SAPIENTICA Simulation](./Images/Animation/ABS20241024.gif)

![PAX SAPIENTICA Simulation](./Images/Animation/mtDNA-Owari.gif)

Figure. Percentage of mitochondrial DNA haplogroups in the Owari Province of Japan by year from 950 BCE to 100 BCE as output by ABS.

## [🧬 DNA Haplogroups](Data/Genomes/HomoSapiens)

This software can handle DNA haplogroup data of ancient human bones.

The following GIF animation shows the changes in East Asian mtDNA haplogroups from 1 Julian day (4713 BC) to the present.

[![PAX SAPIENTICA Mitochondrial DNA](./Images/Animation/mtDNA02.gif)](Data/Genomes/HomoSapiens)

Ancient human bone mtDNA data registered as of December 21, 2023.

[![PAX SAPIENTICA mtDNA](./Images/Screenshot/PAX%20SAPIENTICA%20v6.0.0.4.0%202023_12_25%2022_06_43.png)](Data/Genomes/HomoSapiens)

## 👤 Movement of a person

This software allows you to visualize and move historical figures.

![PAX SAPIENTICA Movement](./Images/Animation/Person01.gif)

![PAX SAPIENTICA Movement](./Images/Animation/Person02.gif)

## 📍 Distributions

### Distribution of bronze swords in NortheastAsia
![bronze swords](./Images/DistributionMaps/BronzeSwordsInNortheastAsia.png)

### Distribution of Zenpokoenfun tombs in Japan
![Distribution of Zenpokoenfun tombs](./Images/DistributionMaps/Zenpokoenfun.gif)

### Distribution of Yayoi Funkyubo tombs in Japan
![Distribution of Zenpokoenfun tombs](./Images/DistributionMaps/YayoiFunkyubo.gif)

### Distribution of bronze bells in Japan
![Distribution of bronze bells](https://upload.wikimedia.org/wikipedia/commons/8/82/DotakuDistribution.png)

### Distribution of weapon-shaped bronzes in Japan
![Distribution of weapon-shaped bronzes](https://upload.wikimedia.org/wikipedia/commons/f/f4/JapaneseWeaponShapedBronzes.png)

---

# How to Install

This software has both a GUI and a CUI.

### GUI

The GUI supports the following libraries

* SFML (CMake)
* [SFML v3.0.0 (.sln / for Windows)](Projects/MapViewer/Windows/SFML_3.0.0) - Available without installation. The Windows version of SFML in the `PAX_SAPIENTICA/Projects/MapViewer/Windows/SFML_3.0.0` directory can run in Visual Studio 2022 (compiler is MSVC).
* OpenSiv3D (tested with v0.6.13 & Visual Studio Community 2022 v17.8.3)
* DxLib (tested on Windows and Android versions)

Any of the above libraries will launch the GUI software.

### CUI

To try this library, build cmake.

> [!TIP]
> For example, clone this repository. You can open the cloned directory with VS Code and build it with CMake.

# Code Documentation

https://aspjt.github.io/PAX_SAPIENTICA/

# How to Contribute

This project welcomes issues and pull requests.
Anyone can send issues and pull requests.

You can find out how to contribute [here](CONTRIBUTING.md).

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

![Android Studio](https://raw.githubusercontent.com/AsPJT/PAX_SAPIENTICA/develop/Images/Screenshot/PAX%20SAPIENTICA%202023-09-23%2020.54.32.png)

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
