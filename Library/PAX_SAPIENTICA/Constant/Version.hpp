/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
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
#define PAX_SAPIENTICA_LIBRARY_VERSION (20230119L)
#endif

// PAX_SAPIENTICA 主要バージョン ( 0 がテスト版 , 1 から正式リリース版の予定 )
#ifndef PAX_SAPIENTICA_LIBRARY_MAJOR
#define PAX_SAPIENTICA_LIBRARY_MAJOR (0)
#endif

// PAX_SAPIENTICA 補助バージョン
#ifndef PAX_SAPIENTICA_LIBRARY_MINOR
#define PAX_SAPIENTICA_LIBRARY_MINOR (0)
#endif

// PAX_SAPIENTICA 微修正バージョン
#ifndef PAX_SAPIENTICA_LIBRARY_PATCHLEVEL
#define PAX_SAPIENTICA_LIBRARY_PATCHLEVEL (5)
#endif

// 正式バージョン名 ( https://github.com/AsPJT/PAX_SAPIENTICA )
#ifndef PAX_SAPIENTICA_LIBRARY_VERSION_NAME
#define PAX_SAPIENTICA_LIBRARY_VERSION_NAME "6.0.0.0.5"
#endif

/*##########################################################################################
	更新履歴
	バージョン	| 更新日		| 概要
	6.0.0.0.X	| 2023/0X/XX	| コピペ用
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

// AsDungeon 世代 ( PAX_SAPIENTICA 第六世代 )
#ifndef PAX_SAPIENTICA_LIBRARY_GENERATION
#define PAX_SAPIENTICA_LIBRARY_GENERATION (6)
#endif

// AsDungeon 世代微修正バージョン
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