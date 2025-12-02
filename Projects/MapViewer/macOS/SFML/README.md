# SFML MapViewer for macOS

## 概要 (Overview)

このディレクトリには、macOS環境でSFMLを使用してMapViewerをビルドするためのファイルが含まれています。

This directory contains files for building MapViewer with SFML on macOS.

## 必要な環境 (Requirements)

- macOS 11.0 (Big Sur) 以降 (macOS 11.0 or later)
- Homebrew
- CMake 3.16 以降 (CMake 3.16 or later)
- C++20 対応コンパイラ (C++20 compatible compiler)
- Git

## インストール手順 (Installation)

### ステップ1: vcpkgのセットアップ (Step 1: Setup vcpkg)

プロジェクトルートディレクトリから、vcpkgセットアップスクリプトを実行します：

From the project root directory, run the vcpkg setup script:

```bash
cd /path/to/PAX_SAPIENTICA
./Scripts/SetupVcpkgMac.sh
```

このスクリプトは以下を自動的に実行します：
- Homebrewを使用した必要なシステムパッケージのインストール
- vcpkgのクローンとブートストラップ
- カスタムtriplet（arm64-osx-static / x64-osx-static）を使用したSFMLの静的リンクインストール
- アーキテクチャの自動検出（ARM64 / Intel x64）

This script will automatically:
- Install required system packages using Homebrew
- Clone and bootstrap vcpkg
- Install SFML with static linking using custom triplets (arm64-osx-static / x64-osx-static)
- Auto-detect architecture (ARM64 / Intel x64)

### ステップ2: プロジェクトのビルド (Step 2: Build the project)

#### 方法1: ビルドスクリプトを使用（推奨 / Recommended）

**開発ビルド（Development Build）**:
```bash
cd /path/to/PAX_SAPIENTICA
./Scripts/DevelopmentBuild.sh
./DevelopmentBuild/SFMLMapViewer
```

**本番ビルド（Production Build）**:
```bash
cd /path/to/PAX_SAPIENTICA
./Scripts/ProductionBuild.sh
./ProductionBuild/SFMLMapViewer.app/Contents/MacOS/SFMLMapViewer
```

#### 方法2: CMakeを直接使用

```bash
cd /path/to/PAX_SAPIENTICA

# ARM64 Mac (Apple Silicon)の場合
cmake -B build -S Projects \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DSFML_STATIC_LIBRARIES=TRUE \
  -DVCPKG_INSTALLED_DIR=vcpkg_installed \
  -DVCPKG_OVERLAY_TRIPLETS=Projects/cmake \
  -DVCPKG_TARGET_TRIPLET=arm64-osx-static

# Intel Macの場合
cmake -B build -S Projects \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DSFML_STATIC_LIBRARIES=TRUE \
  -DVCPKG_INSTALLED_DIR=vcpkg_installed \
  -DVCPKG_OVERLAY_TRIPLETS=Projects/cmake \
  -DVCPKG_TARGET_TRIPLET=x64-osx-static

cmake --build build
```

**重要なオプション (Important options)**:
- `-DSFML_STATIC_LIBRARIES=TRUE`: SFMLの静的リンクを使用（必須 / required for static linking）
- `-DVCPKG_OVERLAY_TRIPLETS=Projects/cmake`: カスタムtripletの場所を指定（Custom triplet location）
- `-DVCPKG_TARGET_TRIPLET`: アーキテクチャ別の静的リンク設定を指定（Static linking configuration per architecture）
  - `arm64-osx-static`: Apple Silicon (M1/M2/M3) Mac用
  - `x64-osx-static`: Intel Mac用

### ステップ3: 実行 (Step 3: Run)

```bash
./build/MapViewer/SFMLMapViewer
```

## vcpkgを使用しない場合 (Without vcpkg)

vcpkgを使用せずにSFMLをソースからビルドする場合：

If you want to build SFML from source without vcpkg:

### 依存関係のインストール (Install dependencies)

```bash
brew install cmake ninja pkg-config
```

### SFMLのビルドとインストール (Build and install SFML)

```bash
# SFML 3.0.0をクローン
git clone https://github.com/SFML/SFML.git
cd SFML
git checkout 3.0.0

# ビルド
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# インストール
sudo cmake --install .
```

### プロジェクトのビルド (Build the project)

```bash
cd /path/to/PAX_SAPIENTICA/Projects
mkdir -p build && cd build
cmake ..
cmake --build .
```

## Apple Siliconについて (About Apple Silicon)

このプロジェクトではカスタムtripletを使用して静的リンクを行います：
- **Apple Silicon (M1/M2/M3)**: `arm64-osx-static` を使用
- **Intel Mac**: `x64-osx-static` を使用

セットアップスクリプトとビルドスクリプトはアーキテクチャを自動検出し、適切なtripletを選択します。

This project uses custom triplets for static linking:
- **Apple Silicon (M1/M2/M3)**: Uses `arm64-osx-static`
- **Intel Mac**: Uses `x64-osx-static`

Setup and build scripts automatically detect the architecture and select the appropriate triplet.

### カスタムtripletについて (About Custom Triplets)

カスタムtriplet設定ファイルは `Projects/cmake/` に配置されています：
- `arm64-osx-static.cmake`: ARM64 Mac用の静的リンク設定
- `x64-osx-static.cmake`: Intel Mac用の静的リンク設定

これらのtripletは以下を設定します：
- 静的リンク（`VCPKG_LIBRARY_LINKAGE=static`）
- X11パスの除外（macOSネイティブのOpenGLフレームワークを使用）

Custom triplet configuration files are located in `Projects/cmake/`:
- `arm64-osx-static.cmake`: Static linking configuration for ARM64 Macs
- `x64-osx-static.cmake`: Static linking configuration for Intel Macs

These triplets configure:
- Static linking (`VCPKG_LIBRARY_LINKAGE=static`)
- Exclusion of X11 paths (to use macOS native OpenGL framework)

## トラブルシューティング (Troubleshooting)

### SFML が見つからない (SFML not found)

CMake実行時に以下のようなメッセージが表示される場合：

```
SFML not found - MapViewer will not be built
```

解決方法：
1. vcpkgのセットアップスクリプトが正常に完了したか確認
2. CMake実行時に以下のオプションを指定しているか確認：
   - `-DCMAKE_TOOLCHAIN_FILE`
   - `-DVCPKG_INSTALLED_DIR`
   - `-DVCPKG_OVERLAY_TRIPLETS`
   - `-DVCPKG_TARGET_TRIPLET`
3. vcpkgディレクトリが正しい場所に存在するか確認
4. `vcpkg_installed/` ディレクトリがプロジェクトルートに存在するか確認

### Homebrewがインストールされていない (Homebrew not installed)

Homebrewがインストールされていない場合、以下のコマンドでインストールしてください：

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### ビルドエラー (Build errors)

Xcodeコマンドラインツールが不足している場合、以下を実行：

```bash
xcode-select --install
```

## 参考リンク (References)

- [SFML 3.0 公式ドキュメント](https://www.sfml-dev.org/tutorials/3.0/)
- [vcpkg 公式サイト](https://vcpkg.io/)
- [Homebrew 公式サイト](https://brew.sh)
- [PAX_SAPIENTICA メインREADME](../../../../README.md)
