/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    ToggleButton Example / Usage Demo
    トグルボタン使用例

##########################################################################################*/

/*
    This is a usage example for PAX_GRAPHICA ToggleButton component
    このファイルはPAX_GRAPHICAのToggleButtonコンポーネントの使用例です

    To use this in your project / プロジェクトで使用するには:
    
    1. Include the header / ヘッダーをインクルード:
       #include <PAX_GRAPHICA/ToggleButton.hpp>
    
    2. Create instances / インスタンスを作成:
       paxg::ToggleButton toggle(100, 100);  // position x=100, y=100
       paxg::ToggleButton toggle2(100, 150, 60, 35);  // with custom size
    
    3. In your main loop / メインループ内で:
       while (paxg::Window::update()) {
           toggle.update();  // Handle input and animation
           toggle.draw();    // Render the button
           
           if (toggle.isOn()) {
               // Do something when ON
           }
       }

    ========================================================================
    Complete Example Code / 完全な使用例コード:
    ========================================================================
*/

#if 0  // Set to 1 to enable this example / 1に設定すると有効化

#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/ToggleButton.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Color.hpp>

int main() {
    // Initialize window / ウィンドウを初期化
    paxg::Window::init(800, 600, "ToggleButton Example");

    // Create toggle buttons / トグルボタンを作成
    paxg::ToggleButton toggle1(100, 100);           // Default size
    paxg::ToggleButton toggle2(100, 150, 60, 35);   // Custom size
    paxg::ToggleButton toggle3(100, 200, 80, 40);   // Larger size

    // Optionally set initial state / 初期状態を設定（オプション）
    toggle2.setOn(true);  // Start with ON state

    // Main loop / メインループ
    while (paxg::Window::update()) {
        // Update mouse state / マウス状態を更新
        paxg::Mouse::getInstance()->calledFirstEveryFrame();

        // Update all toggles / 全てのトグルを更新
        toggle1.update();
        toggle2.update();
        toggle3.update();

        // Clear screen / 画面をクリア
        paxg::Window::clear();

        // Draw all toggles / 全てのトグルを描画
        toggle1.draw();
        toggle2.draw();
        toggle3.draw();

        // Example: Use toggle state / 例：トグル状態を使用
        if (toggle1.isOn()) {
            // Draw something when toggle1 is ON
            // toggle1がONの時に何かを描画
        }

        if (toggle2.isOn() && toggle3.isOn()) {
            // Both toggle2 and toggle3 are ON
            // toggle2とtoggle3が両方ONの時
        }
    }

    return 0;
}

#endif // Example code

/*
    ========================================================================
    API Reference / API リファレンス:
    ========================================================================

    Constructors / コンストラクタ:
    -------------------------------
    ToggleButton()                                 - Default (x=0, y=0, size=50x30)
    ToggleButton(float x, float y)                 - With position
    ToggleButton(float x, float y, float w, float h) - With position and size

    Methods / メソッド:
    -------------------
    void setPosition(float x, float y)    - Set position / 位置を設定
    void setSize(float w, float h)        - Set size / サイズを設定
    void setOn(bool on)                   - Set state / 状態を設定
    bool isOn() const                     - Get state / 状態を取得
    void toggle()                         - Toggle state / 状態を反転
    bool mouseOver() const                - Check if mouse over / マウスオーバー判定
    void update()                         - Update (call every frame) / 更新（毎フレーム呼ぶ）
    void draw() const                     - Draw the button / ボタンを描画

    Getters / ゲッター:
    -------------------
    float x() const       - Get X position / X座標を取得
    float y() const       - Get Y position / Y座標を取得
    float width() const   - Get width / 幅を取得
    float height() const  - Get height / 高さを取得

    ========================================================================
    Design Details / デザイン詳細:
    ========================================================================

    - Background: Rounded rectangle (pill shape) / 背景：角丸長方形（ピル型）
    - Colors / 色:
      * ON:  Green (#00de00, RGB: 0, 222, 0)
      * OFF: Light Gray (RGB: 240, 240, 240)
      * Knob: White (RGB: 255, 255, 255)
      * Shadow: Gray (#bbbbbb, RGB: 187, 187, 187)
    
    - Animation: Smooth sliding transition (20% per frame)
                スムーズなスライドアニメーション（1フレームあたり20%）
    
    - Knob: White circle that slides left (OFF) or right (ON)
            白い円が左（OFF）または右（ON）にスライド
    
    - Shadow: Soft shadow beneath the knob for depth
              つまみの下に柔らかい影を表示して奥行きを演出

    ========================================================================
*/
