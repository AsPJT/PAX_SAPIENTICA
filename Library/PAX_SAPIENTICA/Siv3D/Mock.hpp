#include <array>
#include <string>
#include <vector>

namespace s3d
{
    using String = std::u32string;
    using int32 = int;
    using FilePath = String;
    using URL = String;

    struct Vec2;

    struct Point
    {
        std::size_t x, y;
        Point() : x(0), y(0) {}
        Point(std::size_t _x, std::size_t _y) : x(_x), y(_y) {}
        Point operator+(const Point& p) const { return Point(); }
        Point operator-(const Point& p) const { return Point(); }
        Point operator*(double d) const { return Point(); }
        Point movedBy(int x, int y) const { return Point(); }
        Point movedBy(const Point& p) const { return Point(); }
        Point operator +(const Vec2& p) const { return Point(); }
    };

    enum class Palette
    {
        White
        , Black
        , Orange
        , Gray
        , Skyblue
    };

    enum class BlendState
    {
        MaxAlpha
    };

    using Size = Vec2;

    struct Vec2
    {
        double x, y;
        Vec2() {}
        Vec2(int, int) {}
        Vec2(double _x, double _y) : x(_x), y(_y) {}
        Vec2 operator*(double d) const { return Vec2(); }
        Point movedBy(int x, int y) const { return Point(); }
        Vec2 operator +(const Size& p) const { return Vec2(); }
    };

    std::array<std::size_t, 1> step(std::size_t v) { return std::array<std::size_t, 1>{v}; }

    namespace Arg {
        Vec2 bottomCenter;
        Vec2 topCenter;
        Vec2 topRight;
        Vec2 bottomLeft;
    }

    class ColorF
    {
    public:
        double r, g, b, a;
        ColorF removeSRGBCurve() const { return *this; }
    };

    class Color
    {
    public:
        unsigned char r, g, b, a;
        Color() : r(0), g(0), b(0), a(255) {}
        Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
        operator ColorF() const { return ColorF(); }
    };

    struct Rect
    {
        Rect() {}
        Rect(int _x, int _y, int _w, int _h) {}
        Rect(Point, int, double) {}
        Rect(int, int, int) {}
        Rect(Point, std::size_t, std::size_t) {}
        Rect(Point, Size) {}
        Point pos;
        Size size;
        std::size_t x, y, w, h;
        static int value_type(std::size_t x) { return 0; }
        bool leftClicked() const { return false; }
        void draw() const {}
        void draw(Palette p) const {}
        void draw(const Color& color) const {}
        void drawFrame(int x, int y, Palette p) const {}
        void drawFrame(int x, int y, ColorF p) const {}
        bool mouseOver() const { return false; }
        Rect drawShadow(Point p, int x, int y) const { return Rect(); }
        void setPos(Point p) {}
    };

    class Line
    {
    public:
        int x, y, w, h;
        Line() : x(0), y(0), w(0), h(0) {}
        Line(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
        void draw(double w, ColorF c) const {}
    };

    enum class TextureDesc
    {
        MippedSRGB
    };

    class Model
    {
    public:
        String path;
        Model() {}
        Model(const String& path) {}
        static void RegisterDiffuseTextures(Model model, TextureDesc t) {}
    };

    class Image
    {
    public:
        String path;
        Image() {}
        Image(const String& path) {}
        String mirror() const { return path; }
    };

    class Graphics
    {
    public:
        static void SetBackground(const ColorF& color) {}
        static void Set3DRenderState() {}
    };

    class Vec3
    {
    public:
        float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
        Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    };

    class DebugCamera3D
    {
    public:
        Size size;
        double d;
        Vec3 eye;
    };

    class Graphics3D
    {
    public:
        static void SetGlobalAmbientColor(const ColorF& color) {}
        static void SetSunColor(const ColorF& color) {}
        static void SetCameraTransform(DebugCamera3D camera) {}
        static Size GetRenderTargetSize() { return Size(); }
        static void Flush() {}
    };

    class Scene
    {
    public:
        static int Width() { return 0; }
        static int Height() { return 0; }
        static Vec2 Center() { return Vec2(); }
        static s3d::Size Size() { return s3d::Size(); }
        static void SetBackground(s3d::Color color) {}
        static void SetLetterbox(s3d::Color color) {}
    };

    class Key
    {
    public:
        static bool Down() { return false; }
        static bool Press() { return false; }
        static bool Up() { return false; }
    };

    class Input
    {
    public:
        static bool pressed() { return false; }
    };

    Input input;

    String ToString(int n) { return String(); }

    Input KeyW = Input();
    Input KeyA = Input();
    Input KeyS = Input();
    Input KeyD = Input();
    Input KeyQ = Input();
    Input KeyE = Input();
    Input KeyLeft = Input();
    Input KeyRight = Input();
    Input KeyUp = Input();
    Input KeyDown = Input();

    enum WindowStyle
    {
        Frameless
        , Sizable
    };

    class Window
    {
    public:
        static void SetTitle(const String& title) {}
        static void Resize(const int w, const int h) {}
        static void SetStyle(const WindowStyle style) {}
    };

    class System
    {
    public:
        static bool Update() { return true; }
        static void LaunchBrowser(String) {}
    };

    struct Circle
    {
        Vec2 center;
        int radius;
        Circle() : center(), radius(0) {}
        Circle(Vec2 _center, int _radius) : center(_center), radius(_radius) {}
    };

    int Max(int a, int b) { return 0; }

    class FontMethod
    {
    public:
        FontMethod() {}
        static FontMethod SDF;
    };
    FontMethod FontMethod::SDF = FontMethod();

    class Font
    {
    public:
        FontMethod method;
        int w;
        String path;
        Font operator()(String path) const { return Font(); }
        Font region() const { return Font(); }
        int height() const { return 0; }
        void drawAt(Vec2 p, String s) const {}
        void drawAt(Vec2 p, double a, double b, Palette pa) const {}
        void draw(Vec2 p, String s, Palette pa) const {}
        void draw(Vec2 p, Vec2 s, Palette pa) const {}
        void draw(Vec2 p, Palette pa) const {}
        void draw(Point p, Palette pa) const {}
        void draw(Vec2 p, double a, double b, ColorF c) const {}
        void setBufferThickness(int n) {}
        Font operator ()(String s1, std::size_t n1, String s2, std::size_t n2, String s3, std::size_t n3) const { return Font(); }
    };

    class Mouse
    {
    public:
        static Vec2 Pos() { return Vec2(); }
    };

    class Texture
    {
    public:
        String path;
        Texture() {}
        Texture(const String& path) {}
        Texture resized(const int w, const int h) const { return Texture(); }
        Texture resized(const double s) const { return Texture(); }
        void draw(Vec2 p) const {}
        void draw(int n, double d) const {}
        void drawAt(Vec2 p) const {}
        void drawAt(double x, double y) const {}
        int width() const { return 0; }
        int height() const { return 0; }
        bool operator !() const { return true; }
        operator bool() const { return true; }
    };

    using URL = String;

    struct Triangle
    {
        double p1;
        double p2;
        double p3;
        double p4;
        void draw(Palette p) const {}
    };

    struct Ellipse
    {
        Vec2 center;
        int a;
        int b;
        Ellipse() : center(), a(0), b(0) {}
        Ellipse(Vec2 _center, int _a, int _b) : center(_center), a(_a), b(_b) {}
    };

    class Sound
    {
    public:
        Sound() {}
        Sound(const std::string& path) {}
        bool play() { return true; }
        bool stop() { return true; }
        bool isPlaying() { return false; }
    };

    class Unicode
    {
    public:
        static String FromUTF8(const std::string& utf8) { return String(); }
    };

    enum class TextureFormat
    {
        R8G8B8A8_Unorm_SRGB
    };

    enum class HasDepth
    {
        Yes
    };

    class Quaternion
    {
    public:
        double x, y, z, w;
        static Quaternion RotateX(double angle) { return Quaternion(); }
        static Quaternion RotateY(double angle) { return Quaternion(); }
        Quaternion operator *(const Quaternion& q) const { return Quaternion(); }
    };

    class Sphere
    {
    public:
        double x, y, z, r;
        void draw(Quaternion q, Texture t) {}
    };

    class MSRenderTexture
    {
    public:
        Size size;
        TextureFormat format;
        HasDepth hasDepth;

        MSRenderTexture clear(const ColorF& color) const { return MSRenderTexture(); }
        void resolve() {}
        void draw(double x, double y) {}
    };

    class ScopedRenderTarget3D
    {
    public:
        MSRenderTexture texture;
    };

    enum class RasterizerState
    {
        SolidCullNone
    };

    class ScopedRenderStates3D
    {
    public:
        RasterizerState rasterizerState;
    };

    enum class SamplerState
    {
        ClampNearest
        , ClampLinear
    };

    class RenderTexture
    {
    public:
        RenderTexture() {}
        RenderTexture(int) {}
        RenderTexture(const Size&, ColorF) {}
        int size() const { return 0; }
        RenderTexture clear(ColorF) { return RenderTexture(); }
        void draw(ColorF) {}
    };

    class ScopedRenderStates2D
    {
    public:
        SamplerState samplerState;
        ScopedRenderStates2D() {}
        ScopedRenderStates2D(SamplerState) {}
        ScopedRenderStates2D(BlendState) {}
    };

    class ScopedRenderTarget2D
    {
    public:
        ScopedRenderTarget2D() {}
        ScopedRenderTarget2D(RenderTexture) {}
    };

    class TextStyle
    {
    public:
        static Vec2 Outline(int n, double d, Palette p) { return Vec2(); }
        static Vec2 Outline(int n, double d, ColorF p) { return Vec2(); }
    };

    class SimpleHTTP
    {
    public:
        static SimpleHTTP Save(URL url, Texture texture) { return SimpleHTTP(); }
        bool isOK() { return true; }
    };

    template <class Type>
    class Array
    {
    public:
        Type& operator [](int n) { return dummyData; }  // non-const version
        const Type& operator [](int n) const { return dummyData; }
        int size() const { return 0; }
        template <class... Args>
        void emplace_back(Args&&... args) {}
    private:
        Type dummyData;

    };

    class Mat3x2
    {
    public:
        static Mat3x2 Translate(int, int) { return Mat3x2(); }
    };

    class Transformer2D
    {
    public:
        Transformer2D() {}
        Transformer2D(const Mat3x2&) {}
    };

    class RoundRect : public Rect
    {
    public:
        RoundRect() {}
        RoundRect(int, int, int, int, int) {}
        void draw(ColorF) {}
        void draw() {}
    };

    class Shader
    {
    public:
        static void GaussianBlur(RenderTexture, RenderTexture, RenderTexture) {}
    };

    class SimpleGUI
    {
    public:
        static bool Button(String, Vec2) { return false; }
    };

    namespace detail
    {
        class Console_impl
        {
        public:
            void open() {}
        };
    }

    class Spline2D
    {
    public:
        Spline2D() {}
        Spline2D(const Array<Vec2>&) {}
        void draw(int, Color) {}
    };
}
