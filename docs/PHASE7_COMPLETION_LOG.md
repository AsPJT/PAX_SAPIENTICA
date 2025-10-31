# フェーズ7完了レポート：バグ修正と軽量タスクの実装

**日付**: 2025-11-01
**ブランチ**: `feature/refactor-dependency`
**前回コミット**: Phase 6完了後

## フェーズ概要

Phase 7では、Phase 6完了後に発見されたバグの修正と、Phase 5完了ログで推奨されていた軽量タスクを実装しました。具体的には、MapViewportInputHandlerの重複登録バグの修正、既存の最適化の確認、そしてウィンドウイベントシステムの統合を行いました。

## 実装日

2025-11-01

## 達成した目標

### バグ修正
1. ✅ **MapViewportInputHandler重複登録バグの修正**
   - GraphicsManager::init()での重複登録を削除
   - キー入力が動作しない問題を解決

### 最適化の確認
2. ✅ **InputRouterとRenderLayerManagerのソート最適化確認**
   - 既にダーティフラグによる最適化が実装済みであることを確認
   - 毎フレームのソートが初回のみに削減されていることを検証

### 新機能の実装
3. ✅ **ウィンドウイベントシステムの統合**
   - InputEventTypeにWindowResizeとWindowFocusを追加
   - InputRouterに専用ルーティングメソッドを追加
   - 将来的な完全統合への基盤を構築

## 変更内容の詳細

### 1. MapViewportInputHandler重複登録バグの修正

#### 問題の発見

Phase 6完了後、ユーザーから「何のキーも動かない」という報告がありました。調査の結果、MapViewportInputHandlerがInputRouterに2回登録されていることが判明しました。

**原因箇所**:
- `GraphicsManager::init()` 内で条件付き登録（87-91行目）
- `GraphicsManager::setMapViewportInputHandler()` 内で登録（106行目）

**実行フロー**:
```cpp
// Main.hpp
graphics_manager.init(...);  // ここで1回目の登録（nullptrの可能性）
graphics_manager.setMapViewportInputHandler(&handler, &viewport);  // ここで2回目の登録
```

#### 修正内容

**ファイル**: [Library/PAX_MAHOROBA/Rendering/GraphicsManager.hpp:87-88](../Library/PAX_MAHOROBA/Rendering/GraphicsManager.hpp#L87-L88)

```cpp
// 修正前
if (map_viewport_input_handler_ != nullptr) {
    input_router_.registerHandler(map_viewport_input_handler_);
}

// 修正後
// MapViewportInputHandlerは setMapViewportInputHandler() で登録される
// MapViewportInputHandler will be registered via setMapViewportInputHandler()
```

**効果**: 重複登録が解消され、キー入力が正常に動作するようになりました。

### 2. 最適化の確認

Phase 5完了ログの「オプション2: パフォーマンス最適化」として、InputRouterとRenderLayerManagerのソート最適化を確認しました。

#### InputRouterの既存最適化

**ファイル**: [Library/PAX_MAHOROBA/Input/InputRouter.hpp](../Library/PAX_MAHOROBA/Input/InputRouter.hpp)

```cpp
private:
    std::vector<IInputHandler*> handlers_;
    bool is_sorted_ = false;  // ソート済みフラグ

public:
    void registerHandler(IInputHandler* handler) {
        handlers_.push_back(handler);
        is_sorted_ = false;  // 新規登録時にリセット
    }

    void sort() {
        std::stable_sort(handlers_.begin(), handlers_.end(), ...);
        is_sorted_ = true;
    }

    bool routeKeyboardInput(...) {
        if (!is_sorted_) {
            sort();  // ソートされていない場合のみソート
        }
        // ... ルーティング処理
    }
```

**最適化内容**:
- ✅ ダーティフラグパターンの実装
- ✅ 登録時のみソートフラグをリセット
- ✅ ルーティング時に未ソートの場合のみソート実行

**効果**: 毎フレーム3回のソート（キーボード、マウスホイール、マウス）→ 初回1回のみに削減

#### RenderLayerManagerの既存最適化

**ファイル**: [Library/PAX_MAHOROBA/Rendering/RenderLayerManager.hpp](../Library/PAX_MAHOROBA/Rendering/RenderLayerManager.hpp)

同様のダーティフラグパターンが実装済みで、毎フレームのソートが初回のみに削減されています。

### 3. ウィンドウイベントシステムの統合

Phase 5完了ログの「オプション4: 完全なイベントシステム統合」の一部として、ウィンドウイベントのサポートを追加しました。

#### InputEventTypeの拡張

**ファイル**: [Library/PAX_MAHOROBA/Input/IInputHandler.hpp:22-28](../Library/PAX_MAHOROBA/Input/IInputHandler.hpp#L22-L28)

```cpp
enum class InputEventType {
    Mouse,         ///< マウス/タッチ入力 / Mouse/Touch input
    Keyboard,      ///< キーボード入力 / Keyboard input
    MouseWheel,    ///< マウスホイール入力 / Mouse wheel input
    WindowResize,  ///< ウィンドウリサイズ / Window resize
    WindowFocus    ///< ウィンドウフォーカス / Window focus
};
```

#### InputEvent構造体の拡張

**ファイル**: [Library/PAX_MAHOROBA/Input/IInputHandler.hpp:47-57](../Library/PAX_MAHOROBA/Input/IInputHandler.hpp#L47-L57)

```cpp
struct InputEvent {
    // ... 既存フィールド ...

    /// @brief ウィンドウの幅（WindowResizeイベント用）
    int window_width = 0;

    /// @brief ウィンドウの高さ（WindowResizeイベント用）
    int window_height = 0;

    /// @brief フォーカス状態（WindowFocusイベント用）
    bool has_focus = true;

    // コンストラクタ（ウィンドウリサイズイベント用）
    InputEvent(int width, int height)
        : type(InputEventType::WindowResize), input_state_manager(nullptr),
          x(0), y(0), window_width(width), window_height(height) {}

    // コンストラクタ（ウィンドウフォーカスイベント用）
    static InputEvent createFocusEvent(bool focus) {
        InputEvent event;
        event.type = InputEventType::WindowFocus;
        event.has_focus = focus;
        return event;
    }
};
```

#### InputRouterへのルーティングメソッド追加

**ファイル**: [Library/PAX_MAHOROBA/Input/InputRouter.hpp:264-337](../Library/PAX_MAHOROBA/Input/InputRouter.hpp#L264-L337)

```cpp
/// @brief ウィンドウリサイズイベントをルーティング
bool routeWindowResizeEvent(int width, int height) {
    InputEvent event(width, height);

    if (!is_sorted_) {
        sort();
    }

    // 前面から順にイベントを通知（ヒットテストなし）
    for (IInputHandler* handler : handlers_) {
        if (handler == nullptr || !handler->isEnabled()) continue;

        if (handler->handleInput(event)) {
            return true;  // イベント消費済み
        }
    }

    return false;
}

/// @brief ウィンドウフォーカスイベントをルーティング
bool routeWindowFocusEvent(bool has_focus) {
    InputEvent event = InputEvent::createFocusEvent(has_focus);

    if (!is_sorted_) {
        sort();
    }

    // 前面から順にイベントを通知（ヒットテストなし）
    for (IInputHandler* handler : handlers_) {
        if (handler == nullptr || !handler->isEnabled()) continue;

        if (handler->handleInput(event)) {
            return true;  // イベント消費済み
        }
    }

    return false;
}
```

**設計**: キーボード・マウスホイールと同様に、座標非依存型イベントとしてヒットテストをスキップします。

#### UILayerの更新

**ファイル**: [Library/PAX_MAHOROBA/UI/UILayer.hpp:416-423](../Library/PAX_MAHOROBA/UI/UILayer.hpp#L416-L423)

```cpp
bool handleInput(const InputEvent& event) override {
    if (!enabled_ || !visible_) return false;

    // 座標に依存しないイベント（キーボード、マウスホイール、ウィンドウイベント）はスキップ
    // Skip coordinate-independent events (Keyboard, MouseWheel, Window events)
    if (event.type == InputEventType::Keyboard ||
        event.type == InputEventType::MouseWheel ||
        event.type == InputEventType::WindowResize ||
        event.type == InputEventType::WindowFocus) {
        return false;
    }

    // マウス/タッチイベントのみ処理
    // ...
}
```

**理由**: ウィンドウイベントはUIウィジェットではなく、アプリケーション全体で処理すべきため、UILayerではスキップします。

## システムアーキテクチャの現状

### 入力イベントフロー

```
Main.hpp (メインループ)
  ↓
InputRouter::routeKeyboardInput()      // キーボード入力
InputRouter::routeMouseWheelInput()    // マウスホイール入力
InputRouter::routeInput()              // マウス/タッチ入力
InputRouter::routeWindowResizeEvent()  // ウィンドウリサイズ（新規）
InputRouter::routeWindowFocusEvent()   // ウィンドウフォーカス（新規）
  ↓
優先順位に基づいて各ハンドラーに配信:
  1. UILayer (優先度: 400)
  2. MapController (優先度: 200)
  3. MapViewportInputHandler (優先度: 0)
```

### イベントタイプの分類

**座標依存型** (hitTest実行):
- Mouse/Touch

**座標非依存型** (hitTestスキップ):
- Keyboard
- MouseWheel
- WindowResize (新規)
- WindowFocus (新規)

## 変更したファイル

### バグ修正
- `Library/PAX_MAHOROBA/Rendering/GraphicsManager.hpp` - 重複登録の削除

### ウィンドウイベント統合
- `Library/PAX_MAHOROBA/Input/IInputHandler.hpp` - InputEventTypeとInputEventの拡張
- `Library/PAX_MAHOROBA/Input/InputRouter.hpp` - ルーティングメソッドの追加
- `Library/PAX_MAHOROBA/UI/UILayer.hpp` - ウィンドウイベントのスキップ処理

## ビルド・テスト結果

**ビルド**: ✅ 全39ファイルのコンパイル成功、警告なし
**テスト**: ✅ 27/28テストパス（1件の失敗は本フェーズとは無関係）
**動作確認**: ✅ ユーザーによる「動きました」の確認

## 学んだ教訓

### 1. デバッグの重要性

「何のキーも動かない」という症状から、システム的な問題（重複登録）を特定しました：
- InputRouterのハンドラーリストを確認
- 登録フローを追跡
- 重複が問題の原因であることを特定

### 2. 既存の最適化の価値

Phase 5-6で実装されたInputRouterとRenderLayerManagerは既に最適化されており、追加作業は不要でした：
- ダーティフラグパターンの実装
- 安定ソートの使用
- 早期リターンによる無駄な処理の回避

### 3. 段階的な統合アプローチ

ウィンドウイベントシステムは、完全な統合ではなく、まずインフラストラクチャを整備しました：
- イベントタイプの追加
- ルーティングメソッドの追加
- 既存の`handleWindowResize()`は維持
- 将来的な移行を容易にする設計

## 次のフェーズへの推奨事項

Phase 7で軽量タスクが完了しました。Phase 5完了ログに記載された次の推奨作業に基づき、以下のオプションを提案します：

### オプション1: 運用検証とデバッグ（推奨）
**目的**: 新しいInputRouterとRenderLayerManagerシステムの安定性確認
- より複雑なシナリオでのテスト
- エッジケースの発見と修正
- パフォーマンス測定

### オプション2: InputEventの拡張
**目的**: より詳細な入力イベント情報の提供
- マウスボタンの識別（左、右、中）
- 修飾キー（Shift、Ctrl、Alt）
- ドラッグ&ドロップイベント

### オプション3: ウィンドウイベントの完全統合
**目的**: GraphicsManager::handleWindowResize()をInputRouter経由に移行
- Main.hppでrouteWindowResizeEvent()を使用
- MapViewportとUILayerにウィンドウイベントハンドラーを実装
- 既存のhandleWindowResize()を段階的に置き換え

### オプション4: 後方互換性レイヤーの削除
**目的**: コードベースのさらなる簡素化
- 使用されていない後方互換性メソッドの削除
- APIバージョンの明確化
- 外部プロジェクトへの移行ガイド提供

**推奨**: まずオプション1（運用検証）を実施し、システムの安定性を確認してから、他のオプションに進むことをお勧めします。

## 関連ドキュメント

- [Phase 5完了ログ](PHASE5_COMPLETION_LOG.md) - InputRouterの初期実装
- [Phase 6完了ログ](PHASE6_COMPLETION_LOG.md) - 座標非依存型入力の対応
- [IInputHandlerインターフェース](../Library/PAX_MAHOROBA/Input/IInputHandler.hpp)
- [InputRouter実装](../Library/PAX_MAHOROBA/Input/InputRouter.hpp)
- [RenderLayerManager実装](../Library/PAX_MAHOROBA/Rendering/RenderLayerManager.hpp)
- [GraphicsManager実装](../Library/PAX_MAHOROBA/Rendering/GraphicsManager.hpp)

---

**Phase 7ステータス**: ✅ 完了
**日付**: 2025-11-01
**重要な成果**:
- ✅ MapViewportInputHandler重複登録バグの修正
- ✅ 既存のソート最適化の確認
- ✅ ウィンドウイベントシステムの基盤構築
