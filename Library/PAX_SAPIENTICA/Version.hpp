/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CONSTANT_VERSION_HPP
#define PAX_SAPIENTICA_CONSTANT_VERSION_HPP

/*##########################################################################################
	Define version macro.
	バージョンマクロを定義する。
##########################################################################################*/

// 正式バージョン数値
#ifndef PAX_SAPIENTICA_LIBRARY_VERSION
#define PAX_SAPIENTICA_LIBRARY_VERSION (20240108L)
#endif

// PAX_SAPIENTICA 主要バージョン ( 0 がテスト版 , 1 から正式リリース版 ) [vX.a.a]
#ifndef PAX_SAPIENTICA_LIBRARY_MAJOR
#define PAX_SAPIENTICA_LIBRARY_MAJOR (1)
#endif

// PAX_SAPIENTICA 補助バージョン  [v6.0.a.X.a]
#ifndef PAX_SAPIENTICA_LIBRARY_MINOR
#define PAX_SAPIENTICA_LIBRARY_MINOR (0)
#endif

// PAX_SAPIENTICA 微修正バージョン [v6.0.a.a.X]
#ifndef PAX_SAPIENTICA_LIBRARY_PATCHLEVEL
#define PAX_SAPIENTICA_LIBRARY_PATCHLEVEL (0)
#endif

// バージョン名 ( https://github.com/AsPJT/PAX_SAPIENTICA )
#ifndef PAX_SAPIENTICA_LIBRARY_VERSION_NAME
#define PAX_SAPIENTICA_LIBRARY_VERSION_NAME "1.0.0 MESOPOTAMIA"
#endif

//char name[
//	10
//		+ ((PAX_SAPIENTICA_LIBRARY_PATCHLEVEL >= 10) ? 1 : 0)
//];

/*##########################################################################################
	更新履歴
	バージョン	| 更新日		| 概要
	6.0.1.0.X	| 2024/0X/XX	| コピペ用
	6.0.1.0.0	| 2024/01/08	| リリース版
	6.0.0.4.20	| 2024/01/08	| マウス対応
	6.0.0.4.19	| 2024/01/07	| シミュレーション定数のTSV入力対応 #90
	6.0.0.4.18	| 2024/01/06	| データの設定ファイルを追加 #89
	6.0.0.4.17	| 2024/01/06	| シミュレーション対象地域のTSV化 #88
	6.0.0.4.16	| 2024/01/04	| Androidの地図機能を修正 #87
	6.0.0.4.15	| 2024/01/04	| バイナリデータの読み込み処理を変更 #86
	6.0.0.4.14	| 2024/01/04	| Android対応（安定化のための修正※完全ではない） #85
	6.0.0.4.13	| 2024/01/03	| GUIの起動時処理を新しい仕様へ更新 #84
	6.0.0.4.12	| 2024/01/03	| SFMLリサイズバグ修正・ホイール機能追加・プルダウンメニュー改善 #83
	6.0.0.4.11	| 2024/01/03	| 旧ルートパスの取得方法を削除 #82
	6.0.0.4.10	| 2024/01/03	| シミュレーション側のPath.tsv対応 #81
	6.0.0.4.9	| 2024/01/02	| KeyValue形式のTSV読み込みクラスを追加 #80
	6.0.0.4.8	| 2024/01/02	| Path.tsvを追加 #79
	6.0.0.4.7	| 2024/01/01	| SFMLの描画不具合を修正 #76
	6.0.0.4.6	| 2023/12/31	| シミュレータの例外を修正 #75
	6.0.0.4.5	| 2023/12/31	| シミュレーションのGUI対応 #74
	6.0.0.4.4	| 2023/12/29	| BOM付きファイルを問題なく読み込めるように対応 #73
	6.0.0.4.3	| 2023/12/04	| ログをとるマクロを追加 #72
	6.0.0.4.2	| 2023/12/04	| シミュレーションの手直し #71
	6.0.0.4.1	| 2023/11/26	| シミュレーションの高速化 #70
	6.0.0.4.0	| 2023/11/25	| 渡来を除くAgent-based Simulationの修正と改善
	6.0.0.3.23	| 2023/11/16	| 集落によるシミュレーションを追加
	6.0.0.3.22	| 2023/11/13	| Font管理クラスを改善
	6.0.0.3.21	| 2023/11/09	| プルダウンメニューをunordered_mapでの管理へ変更
	6.0.0.3.20	| 2023/11/05	| XYZタイルをText Fileから読み込みできるように改善
	6.0.0.3.19	| 2023/10/31	| 地理情報のバイナリデータの入出力機能を追加
	6.0.0.3.18	| 2023/10/29	| XYZタイルデータ：傾斜のバイナリ入出力に対応
	6.0.0.3.17	| 2023/10/28	| 地方区分追加
	6.0.0.3.16	| 2023/10/24	| enumをunordered_mapに置き換え
	6.0.0.3.15	| 2023/10/21	| 地名表示の改良/File読み込み不具合修正/OpenMP有効化
	6.0.0.3.14	| 2023/10/18	| MurMur3の追加及び既存の文字列比較の置換対応
	6.0.0.3.13	| 2023/10/11	| SFMLのUI表示を改善
	6.0.0.3.12	| 2023/10/09	| 主にGUIにおけるデータ読み込みの安全性と汎用性を向上
	6.0.0.3.11	| 2023/10/02	| 入力ファイルを読み込むクラスを作成
	6.0.0.3.10	| 2023/10/01	| GUI描画ライブラリ「まほろば」を作成
	6.0.0.3.9	| 2023/10/01	| DxLibで今まで未対応だった描画機能を追加
	6.0.0.3.8	| 2023/09/28	| upd: 描画ライブラリの順番を統一
	6.0.0.3.7	| 2023/09/27	| upd: SFMLでのMapviewerの表示を修正
	6.0.0.3.6	| 2023/09/26	| fix: SFML関連のエラーを修正
	6.0.0.3.5	| 2023/09/25	| テクスチャの読み込み速度の向上
    6.0.0.3.4	| 2023/09/23	| Android Studioによるアプリ開発機能をサポート
    6.0.0.3.3	| 2023/09/22	| PAXS_GRAPHICA のマルチプラットフォーム対応
    6.0.0.3.2	| 2023/09/20	| 日本の令制国を大幅に追加
    6.0.0.3.1	| 2023/09/18	| 日本の令制国の国境線を追加
    6.0.0.3.0	| 2023/09/18	| 集落エージェントを追加
    6.0.0.2.18	| 2023/08/19	| Doxygen のドキュメントを生成する workflow を追加
    6.0.0.2.17	| 2023/08/13	| シミュレーションの機能を更新
    6.0.0.2.16	| 2023/07/22	| CMake や Test の更新
    6.0.0.2.15	| 2023/07/17	| PAXS_GRAPHICA の基本機能を追加
    6.0.0.2.14	| 2023/07/14	| シミュレーションのデータ構造の見直し
    6.0.0.2.13	| 2023/07/10	| 暦クラスの分割
    6.0.0.2.12	| 2023/07/09	| GUI 関連のファイルを細分化
    6.0.0.2.11	| 2023/07/08	| ヒジュラ暦を追加
    6.0.0.2.10	| 2023/07/01	| ライブラリのコードを整理
    6.0.0.2.9	| 2023/06/27	| 進行状態を記録する出力バーを追加
    6.0.0.2.8	| 2023/06/24	| 地図投影法クラスを追加
    6.0.0.2.7	| 2023/06/22	| 人間エージェントクラスを追加
    6.0.0.2.6	| 2023/06/10	| シミュレーション用の地図のデータ構造を変更
    6.0.0.2.5	| 2023/05/29	| 360 度写真を表示する機能を追加
    6.0.0.2.4	| 2023/05/15	| 傾斜と緯度ごとのセルの大きさのデータを追加
    6.0.0.2.3	| 2023/05/13	| 仮の時間管理アイコンを追加
    6.0.0.2.2	| 2023/05/09	| XYZ タイル管理クラスを追加
    6.0.0.2.1	| 2023/05/08	| エージェントの描画を追加
    6.0.0.2.0	| 2023/05/05	| ImageResize の追加
	6.0.0.2.0	| 2023/05/05	| シミュレータの機能が追加
	6.0.0.1.14	| 2023/05/04	| Environment を追加
	6.0.0.1.13	| 2023/05/02	| AgentSimulator を追加
	6.0.0.1.12	| 2023/05/02	| メニューバーを追加
	6.0.0.1.11	| 2023/05/01	| SFML を追加
	6.0.0.1.10	| 2023/04/30	| Agent を追加
	6.0.0.1.9	| 2023/04/30	| LandAndWaterConverter を追加
	6.0.0.1.8	| 2023/04/29	| Submodule を追加
	6.0.0.1.7	| 2023/04/22	| AreaCalculate を追加
	6.0.0.1.6	| 2023/04/21	| ColorCount の整理
	6.0.0.1.5	| 2023/04/15	| ImageSegment を追加
	6.0.0.1.4	| 2023/04/09	| ImageCombiner を追加
	6.0.0.1.3	| 2023/02/27	| OpenCV を追加
	6.0.0.1.2	| 2023/02/26	| CMake / GitHub Actions に対応
	6.0.0.1.1	| 2023/02/23	| 複数の XYZ Tiles の描画に対応
	6.0.0.1.0	| 2023/02/18	| guinpen98 が参加
	6.0.0.0.9	| 2023/01/26	| シミュレータ開始時のロゴの描画を関数化
	6.0.0.0.8	| 2023/01/25	| 和暦の読み込みに対応
	6.0.0.0.7	| 2023/01/21	| 沖縄の地名を追加
	6.0.0.0.6	| 2023/01/20	| XYZ Tiles の地図の読み込みに対応
	6.0.0.0.5	| 2023/01/18	| 2,162 件の和名類聚抄の地名データを追加
	6.0.0.0.4	| 2023/01/17	| 66 件の古事記の地名データを追加
	6.0.0.0.3	| 2023/01/16	| 主な日本の旧国名を追加
	6.0.0.0.3	| 2023/01/16	| 英語表示の暦を追加
	6.0.0.0.2	| 2023/01/15	| 集落遺跡のピンを表示する機能を追加
	6.0.0.0.2	| 2023/01/15	| 日本国外の地図を追加
	6.0.0.0.1	| 2023/01/13	| 地名を表示する機能を追加
	6.0.0.0.1	| 2023/01/13	| 和暦の計算機能を追加
	6.0.0.0.0	| 2023/01/11	| ユリウス暦とグレゴリオ暦の計算機能を追加
	6.0.0.0.0	| 2023/01/11	| PAX_SAPIENTICA をライブラリ化
	6.0.0.0.z	| 2023/01/11	| PAX_SAPIENTICA のロゴを作成
	6.0.0.0.z	| 2023/01/11	| 中国暦のデータの対応を検討
	6.0.0.0.z	| 2023/01/10	| 和暦のデータの対応を開始
	6.0.0.0.a	| 2020/09/21	| PAX SAPIENTICA の名称を考案
##########################################################################################*/

// AsDungeon 世代 ( PAX_SAPIENTICA 第六世代 ) [vX.0.a.a.a]
#ifndef PAX_SAPIENTICA_LIBRARY_GENERATION
#define PAX_SAPIENTICA_LIBRARY_GENERATION (6)
#endif

// AsDungeon 世代微修正バージョン [v6.X.a.a.a]
#ifndef PAX_SAPIENTICA_LIBRARY_GENERATION_PATCHLEVEL
#define PAX_SAPIENTICA_LIBRARY_GENERATION_PATCHLEVEL (0)
#endif

/*##########################################################################################
	AsDungeon 世代とは？
	As Project が開発する地形に関わるシステムの管理番号である。
	第一世代： AsRogueLike_C (First Generation AsDungeon)			| https://github.com/AsPJT/Roguelike ( C++ 版のみ公開 )
	第二世代： AsRogueLike_C++ (Second Generation AsDungeon)		| https://github.com/AsPJT/Roguelike
	第三世代： AsLib (Third Generation AsDungeon)					| https://github.com/AsPJT/AsLib
	第四世代： DungeonTemplateLibrary (Fourth Generation AsDungeon)	| https://github.com/AsPJT/DungeonTemplateLibrary
	第五世代： Crafterra (Fifth Generation AsDungeon)				| https://github.com/AsPJT/Crafterra
	第六世代： PAX_SAPIENTICA (Sixth Generation AsDungeon)			| https://github.com/AsPJT/PAX_SAPIENTICA
	詳しくはこちら
	https://github.com/AsPJT/DungeonTemplateLibrary/wiki/Version-History
##########################################################################################*/

namespace PAX_SAPIENTICA {

    namespace Version {
        // using 定義
        constexpr long pax_sapientica_library_version				= (PAX_SAPIENTICA_LIBRARY_VERSION);
        constexpr int  pax_sapientica_library_generation			= (PAX_SAPIENTICA_LIBRARY_GENERATION);
        constexpr int  pax_sapientica_library_generation_patchlevel	= (PAX_SAPIENTICA_LIBRARY_GENERATION_PATCHLEVEL);
        constexpr int  pax_sapientica_library_major					= (PAX_SAPIENTICA_LIBRARY_MAJOR);
        constexpr int  pax_sapientica_library_minor					= (PAX_SAPIENTICA_LIBRARY_MINOR);
        constexpr int  pax_sapientica_library_patchlevel			= (PAX_SAPIENTICA_LIBRARY_PATCHLEVEL);
    }

}

// PAX_SAPIENTICA 宣言マクロ
#ifndef PAX_SAPIENTICA
#define PAX_SAPIENTICA
#endif

// PAX_SAPIENTICA ライブラリ宣言マクロ
#ifndef PAX_SAPIENTICA_LIBRARY
#define PAX_SAPIENTICA_LIBRARY
#endif

// PAXSAPIENTICA 宣言マクロ
#ifndef PAXSAPIENTICA
#define PAXSAPIENTICA
#endif

#endif // !PAX_SAPIENTICA_CONSTANT_VERSION_HPP
