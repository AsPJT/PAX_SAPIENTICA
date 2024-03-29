# 歴史シミュレータ＆地理情報システム
この解説は2023年12月26日のものです。

## 操作方法
* 矢印 or WASD：地図の上下左右の移動
* Q：拡大
* E：縮小
* メニューバーとボタンはマウス左クリック

最大化ボタンで最大化できます
端をつかんで自由な大きさにできます

## メニューバー
* 言語選択：右上の English を左クリックで変更可能
* View （表示）からカレンダーや地図の表示・非表示可能
* その他のメニューバーは試験運用（実装中）

## シミュレーション
### Sim Init (CUI) を左クリックでシミュレーション初期化処理（CUIに進捗表示）
地図データを読み込むため、初期化処理はPCスペックによりますが１分～数分かかります
Randomize settlements... が100％になると初期化処理は終了です

### Sim Start：シミュレーションを開始するボタン
右上のカレンダーの日付が再生されている＆初期化処理後に実行してください
集落動態エージェントベースシミュレーションが始まります

## カレンダー（開始時は再生モード）
* ▶：再生
* ■：停止
* ◀：逆再生
再生ボタンの下にあるボタンはそれぞれ
１日、１ヶ月、１年、10年、100年、1000年、1万年 進める or 戻す ボタンです

## その他の表示
* カレンダーボタンの下の数字は拡大率です
* pop：シミュレーションの人口数
* set：シミュレーションの集落数
* torai：シミュレーションの渡来数
