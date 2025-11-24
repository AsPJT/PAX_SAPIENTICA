# SFML 3.0.0 MapViewer for Linux (Ubuntu)

## 概要 (Overview)

このディレクトリには、Linux（Ubuntu）環境でSFML 3.0.0を使用してMapViewerをビルドするためのファイルが含まれています。

This directory contains files for building MapViewer with SFML 3.0.0 on Linux (Ubuntu).

## 必要な環境 (Requirements)

- Ubuntu 20.04 以降 (Ubuntu 20.04 or later)
- CMake 3.16 以降 (CMake 3.16 or later)
- C++20 対応コンパイラ (C++20 compatible compiler)
- Git

## インストール手順 (Installation)

### ステップ1: vcpkgのセットアップ (Step 1: Setup vcpkg)

プロジェクトルートディレクトリから、vcpkgセットアップスクリプトを実行します：

From the project root directory, run the vcpkg setup script:

```bash
cd /path/to/PAX_SAPIENTICA
./Scripts/setup_vcpkg_linux.sh
```

このスクリプトは以下を自動的に実行します：
- 必要なシステムパッケージのインストール
- vcpkgのクローンとブートストラップ
- SFML 3.0.0のインストール

This script will automatically:
- Install required system packages
- Clone and bootstrap vcpkg
- Install SFML 3.0.0

### ステップ2: プロジェクトのビルド (Step 2: Build the project)

```bash
mkdir -p Projects/build
cd Projects/build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

### ステップ3: 実行 (Step 3: Run)

```bash
./MapViewer/SFMLMapViewer
```

## vcpkgを使用しない場合 (Without vcpkg)

vcpkgを使用せずにSFML 3.0.0をソースからビルドする場合：

If you want to build SFML 3.0.0 from source without vcpkg:

### 依存関係のインストール (Install dependencies)

```bash
sudo apt update
sudo apt install -y \
    cmake \
    ninja-build \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev
```

### SFML 3.0.0のビルドとインストール (Build and install SFML 3.0.0)

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

## トラブルシューティング (Troubleshooting)

### SFML が見つからない (SFML not found)

CMake実行時に以下のようなメッセージが表示される場合：

```
SFML not found - MapViewer will not be built
```

解決方法：
1. vcpkgのセットアップスクリプトが正常に完了したか確認
2. CMake実行時に `-DCMAKE_TOOLCHAIN_FILE` オプションを指定しているか確認
3. vcpkgディレクトリが正しい場所に存在するか確認

### ビルドエラー (Build errors)

システムの依存関係が不足している場合、以下を実行：

```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config
```

## 参考リンク (References)

- [SFML 3.0 公式ドキュメント](https://www.sfml-dev.org/tutorials/3.0/)
- [vcpkg 公式サイト](https://vcpkg.io/)
- [PAX_SAPIENTICA メインREADME](../../../../README.md)
