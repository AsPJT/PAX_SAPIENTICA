# Ubuntu Build Environment

このディレクトリには、Ubuntu環境（Docker コンテナ）でPAX_SAPIENTICAをビルドするためのスクリプトとツールが含まれています。

This directory contains scripts and tools for building PAX_SAPIENTICA in Ubuntu environment (Docker container).

## ファイル構成 (File Structure)

```
Scripts/UbuntuBuild/
├── README.md              # このファイル (This file)
├── docker-compose.yml     # Docker Compose設定 (Docker Compose configuration)
├── Build.sh              # メインビルドスクリプト (Main build script)
└── SetupVcpkg.sh         # vcpkgセットアップスクリプト (vcpkg setup script)
```

## 必要な環境 (Requirements)

- Docker
- Docker Compose

## 使用方法 (Usage)

### 方法1: Docker Composeを使用 (Method 1: Using Docker Compose)

プロジェクトルートディレクトリから以下のコマンドを実行します：

From the project root directory, run:

```bash
cd Scripts/UbuntuBuild
docker-compose up
```

このコマンドは以下を自動的に実行します：
- Ubuntu 24.04コンテナの起動
- システムパッケージのインストール
- vcpkgのクローンとブートストラップ
- SFMLとその依存関係のインストール
- プロジェクトのビルド
- DEBパッケージの作成

This command will automatically:
- Start Ubuntu 24.04 container
- Install system packages
- Clone and bootstrap vcpkg
- Install SFML and its dependencies
- Build the project
- Create DEB package

### 方法2: 手動実行 (Method 2: Manual Execution)

Dockerコンテナ内で手動で実行する場合：

To run manually inside a Docker container:

```bash
# コンテナを起動 (Start container)
docker run -it -v $(pwd)/../..:/workspace ubuntu:24.04 /bin/bash

# コンテナ内で実行 (Inside the container)
cd /workspace
chmod +x Scripts/UbuntuBuild/Build.sh
./Scripts/UbuntuBuild/Build.sh
```

## スクリプトの説明 (Script Descriptions)

### SetupVcpkg.sh

vcpkgのセットアップとSFMLのインストールを行います。

Sets up vcpkg and installs SFML.

**機能 (Features)**:
- システム依存パッケージのインストール
- vcpkgのクローンとブートストラップ
- SFML 3.0+のインストール（vcpkg.jsonマニフェストモードを使用）

**実行例 (Example)**:
```bash
./Scripts/UbuntuBuild/SetupVcpkg.sh
```

### Build.sh

プロジェクト全体のビルドプロセスを管理します。

Manages the entire project build process.

**機能 (Features)**:
1. vcpkgのセットアップ（SetupVcpkg.shを呼び出し）
2. CMakeでのプロジェクト設定
3. ビルドの実行
4. DEBパッケージの作成

**実行例 (Example)**:
```bash
./Scripts/UbuntuBuild/Build.sh
```

## ビルド成果物 (Build Artifacts)

ビルドが完了すると、以下の場所に成果物が生成されます：

After the build completes, artifacts will be generated at:

```
/workspace/ProductionBuild/
├── SFMLMapViewer              # 実行可能ファイル (Executable)
├── *.deb                      # DEBパッケージ (DEB package)
└── ...                        # その他のビルド成果物 (Other build artifacts)
```

## トラブルシューティング (Troubleshooting)

### ビルドが失敗する (Build fails)

**問題**: vcpkgのブートストラップでエラーが発生する

**解決方法**:
1. インターネット接続を確認
2. vcpkgディレクトリを削除して再試行
   ```bash
   rm -rf /workspace/vcpkg
   ./Scripts/UbuntuBuild/Build.sh
   ```

### DEBパッケージが作成されない (DEB package not created)

**問題**: cpackがDEBパッケージを生成しない

**解決方法**:
1. CMakeLists.txtにCPack設定が含まれているか確認
2. 手動でcpackを実行:
   ```bash
   cd /workspace/ProductionBuild
   cpack -G DEB
   ```

### メモリ不足エラー (Out of memory error)

**問題**: ビルド中にメモリ不足でエラーが発生

**解決方法**:
- Dockerに割り当てるメモリを増やす（Docker Desktop設定）
- 並列ビルドジョブ数を減らす:
  ```bash
  cmake --build ProductionBuild -j2  # 2並列に制限
  ```

## 環境変数 (Environment Variables)

以下の環境変数をdocker-compose.ymlで設定できます：

The following environment variables can be set in docker-compose.yml:

| 変数名 | 説明 | デフォルト値 |
|:---|:---|:---|
| `DEBIAN_FRONTEND` | Debianパッケージマネージャーのフロントエンド | `noninteractive` |
| `WORKSPACE_ROOT` | プロジェクトルートディレクトリ | `/workspace` |

## 参考リンク (References)

- [メインREADME](../../README.md)
- [Linux SFML セットアップガイド](../../Projects/MapViewer/Linux/SFML/README.md)
- [vcpkg公式サイト](https://vcpkg.io/)
- [SFML 3.0 ドキュメント](https://www.sfml-dev.org/tutorials/3.0/)
- [Docker公式サイト](https://www.docker.com/)

## 注意事項 (Notes)

- このビルド環境はUbuntu 24.04をベースにしています
- SFMLは静的ライブラリとしてリンクされます（`-DSFML_STATIC_LIBRARIES=TRUE`）
- vcpkgは初回実行時にクローンされ、2回目以降は既存のものを使用します
- ビルドには時間がかかる場合があります（特にvcpkgの初回セットアップ）
