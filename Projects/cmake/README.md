# グラフィックスライブラリの CMake ビルドサポート

このディレクトリには、SFML、DxLib、OpenSiv3D を CMake でビルドするためのモジュールが含まれています。

## CMake モジュール

- **FindSFMLCustom.cmake** - SFMLを検出するカスタムモジュール
- **FindDxLib.cmake** - DxLibを検出するモジュール
- **FindSiv3D.cmake** - OpenSiv3Dを検出するモジュール

## 必要な設定

### SFML のビルド

SFML は以下の優先順位で検出されます:

1. **プロジェクトローカルのSFML（デフォルト）**
   - Windows: `Projects/MapViewer/Windows/SFML_3.0.0`
   - macOS: `Projects/MapViewer/macOS/SFML`
   - Linux: `Projects/MapViewer/Ubuntu/SFML`

2. **CMake変数で指定**
   ```bash
   cmake ../Projects -DSFML_ROOT_DIR="C:/SFML"
   ```

3. **環境変数で指定**
   ```bash
   set SFML_DIR=C:\SFML
   cmake ../Projects
   ```

4. **システムワイドインストール**
   - システムにインストール済みのSFMLを自動検出

### DxLib のビルド

DxLib を使用するには、以下のいずれかの方法でDxLibのインストール先を指定する必要があります:

#### 方法1: CMake変数で指定（推奨）
```bash
cmake ../Projects -DDXLIB_ROOT_DIR="C:/DxLib"
```

#### 方法2: 環境変数で指定
```bash
set DXLIB_DIR=C:\DxLib
cmake ../Projects
```

#### 方法3: デフォルト位置にインストール
DxLib を `C:\DxLib` にインストールする

### Siv3D のビルド

OpenSiv3D を使用するには、以下のいずれかの方法でSiv3Dのインストール先を指定する必要があります:

#### 方法1: CMake変数で指定（推奨）
```bash
cmake ../Projects -DSIV3D_ROOT_DIR="C:/OpenSiv3D_0.6.13"
```

#### 方法2: 環境変数で指定（Siv3Dの標準方法）
```bash
set SIV3D_0_6_13=C:\OpenSiv3D_0.6.13
cmake ../Projects
```

または
```bash
set SIV3D_DIR=C:\OpenSiv3D_0.6.13
cmake ../Projects
```

## ビルド例

### SFMLのみビルド（デフォルト）
```bash
# プロジェクトローカルのSFMLを使用（追加設定不要）
cmake ../Projects -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug
```

### SFML + DxLib + Siv3D を全てビルド
```bash
# 3つのライブラリ全てを使用
cmake ../Projects -G "Visual Studio 17 2022" -A x64 ^
  -DDXLIB_ROOT_DIR="C:/DxLib" ^
  -DSIV3D_ROOT_DIR="C:/OpenSiv3D_0.6.13"

# ビルド
cmake --build . --config Debug
```

### カスタムSFMLを使用
```bash
# システムワイドまたはカスタムパスのSFMLを使用
cmake ../Projects -G "Visual Studio 17 2022" -A x64 ^
  -DSFML_ROOT_DIR="C:/custom/SFML"
cmake --build . --config Debug
```

## 生成されるターゲット

設定されたライブラリに応じて、以下のターゲットが生成されます:

- `SFMLMapViewer` - SFML版（常に生成、Windows/macOS/Linux）
- `DxLibMapViewer` - DxLib版（Windows、DxLibが見つかった場合）
- `Siv3DMapViewer` - Siv3D版（Windows、Siv3Dが見つかった場合）

## トラブルシューティング

### DxLib が見つからない
```
-- DxLib not found. Set DXLIB_ROOT_DIR to enable DxLib support.
```
このメッセージが表示される場合は、上記の方法でDxLibのパスを指定してください。

### Siv3D が見つからない
```
-- Siv3D not found. Set SIV3D_ROOT_DIR or SIV3D_0_6_13 environment variable to enable Siv3D support.
```
このメッセージが表示される場合は、上記の方法でSiv3Dのパスを指定してください。

### プリコンパイル済みヘッダーのエラー (Siv3D)
Siv3D では `stdafx.h` をプリコンパイル済みヘッダーとして使用します。
CMake が自動的に設定しますが、問題が発生した場合は手動でビルドしてください。

## ライブラリの配置

### DxLib
DxLib は以下のような構造で配置してください:
```
C:/DxLib/
  ├── DxLib.h
  └── lib/
      └── DxLib_x64.lib (または DxLib.lib)
```

### Siv3D
Siv3D は以下のような構造で配置してください:
```
C:/OpenSiv3D_0.6.13/
  ├── include/
  │   ├── Siv3D.hpp
  │   └── ThirdParty/
  └── lib/
      └── Windows/
          ├── Siv3D.lib
          └── Siv3D_d.lib
```

## 注意事項

- Siv3D のサポートは現在 Windows のみです
- SFML は Windows/macOS/Linux でサポートされています
- ライブラリが見つからない場合は警告が表示されますが、ビルドは継続されます
