/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_WINDOW_HPP
#define PAX_GRAPHICA_WINDOW_HPP

/*##########################################################################################

##########################################################################################*/

#include <string>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxg {

    namespace Window {
#ifdef PAXS_USING_SFML
        sf::RenderWindow window;
#endif // PAXS_USING_SFML

        void Init(int width, int height, const std::string title){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::Resize(width, height);
            s3d::Window::SetTitle(title.s3d());
#elif defined(PAXS_USING_SFML)
            window.create(sf::VideoMode(width, height), title);
#endif
        }

        bool update(){
#if defined(PAXS_USING_SIV3D)
            return s3d::System::Update();
#elif defined(PAXS_USING_SFML)
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    return false;
            }
            return true;
#else
            return false;
#endif
        }

        void setTitle(const std::string title){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::SetTitle(title.s3d());
#elif defined(PAXS_USING_SFML)
            window.setTitle(title);
#endif
        }

        void setSize(int width, int height){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::Resize(width, height);
#elif defined(PAXS_USING_SFML)
            window.setSize(sf::Vector2u(width, height));
#endif
        }

        void setPosition(int x, int y){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::SetPos(x, y);
#elif defined(PAXS_USING_SFML)
            window.setPosition(sf::Vector2i(x, y));
#endif
        }

        void setPosition(const paxg::Vec2& pos){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::SetPos(pos.x, pos.y);
#elif defined(PAXS_USING_SFML)
            window.setPosition(pos);
#endif
        }

        void setIcon(const std::string path){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::SetIcon(path.s3d());
#elif defined(PAXS_USING_SFML)
            sf::Image icon;
            if (!icon.loadFromFile(path))
                return;
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
#endif
        }

        void setVisible(bool visible){
#if defined(PAXS_USING_SIV3D)
            s3d::Window::SetStyle(visible ? s3d::WindowStyle::Sizable : s3d::WindowStyle::None);
#elif defined(PAXS_USING_SFML)
            window.setVisible(visible);
#endif
        }

        void setVSync(bool vsync){
#if defined(PAXS_USING_SIV3D)
            s3d::Graphics::SetTargetFrameRate(vsync ? 60 : 0);
#elif defined(PAXS_USING_SFML)
            window.setVerticalSyncEnabled(vsync);
#endif
        }

        void setFPS(int fps){
#if defined(PAXS_USING_SIV3D)
            s3d::Graphics::SetTargetFrameRate(fps);
#elif defined(PAXS_USING_SFML)
            window.setFramerateLimit(fps);
#endif
        }

        void setMouseCursorVisible(bool visible){
#if defined(PAXS_USING_SIV3D)
            s3d::Cursor::SetStyle(visible ? s3d::CursorStyle::Default : s3d::CursorStyle::Hidden);
#elif defined(PAXS_USING_SFML)
            window.setMouseCursorVisible(visible);
#endif
        }

        void setMouseCursorGrabbed(bool grabbed){
#if defined(PAXS_USING_SIV3D)
            s3d::Cursor::SetStyle(grabbed ? s3d::CursorStyle::Hidden : s3d::CursorStyle::Default);
#elif defined(PAXS_USING_SFML)
            window.setMouseCursorGrabbed(grabbed);
#endif
        }

        void setMouseCursor(const std::string path){
#if defined(PAXS_USING_SIV3D)
            s3d::Cursor::RequestStyle(path.s3d());
#elif defined(PAXS_USING_SFML)
            sf::Image image;
            if (!image.loadFromFile(path))
                return;

            sf::Cursor cursor;
            cursor.loadFromPixels(image.getPixelsPtr(), image.getSize(), sf::Vector2u(0, 0));
            window.setMouseCursor(cursor);
#endif
        }

        void setMousePosition(int x, int y){
#if defined(PAXS_USING_SIV3D)
            s3d::Cursor::SetPos(x, y);
#elif defined(PAXS_USING_SFML)
            sf::Mouse::setPosition(sf::Vector2i(x, y), window);
#endif
        }

        void setMousePosition(const paxg::Vec2 pos){
#if defined(PAXS_USING_SIV3D)
            s3d::Cursor::SetPos(pos.x, pos.y);
#elif defined(PAXS_USING_SFML)
            sf::Mouse::setPosition(pos, window);
#endif
        }

        paxg::Vec2 getMousePosition(){
#if defined(PAXS_USING_SIV3D)
            return s3d::Cursor::PosF();
#elif defined(PAXS_USING_SFML)
            return sf::Mouse::getPosition(window);
#else
            return paxg::Vec2(0, 0);
#endif
        }

        void setKeyRepeat(bool repeat){
#if defined(PAXS_USING_SIV3D)
            s3d::System::SetTerminationTriggers(repeat ? s3d::UserAction::CloseButtonClicked : s3d::UserAction::ExitFocusLost);
#elif defined(PAXS_USING_SFML)
            window.setKeyRepeatEnabled(repeat);
#endif
        }

        void setBackgroundColor(const paxg::Color color){
#if defined(PAXS_USING_SIV3D)
            s3d::Graphics::SetBackground(color);
#elif defined(PAXS_USING_SFML)
            window.clear(color);
#endif
        }

        void clear(){
#if defined(PAXS_USING_SIV3D)
            s3d::Graphics::Clear();
#elif defined(PAXS_USING_SFML)
            window.clear();
#endif
        }

        void display(){
#if defined(PAXS_USING_SIV3D)
            s3d::Graphics::Flush();
#elif defined(PAXS_USING_SFML)
            window.display();
#endif
        }

        void close(){
#if defined(PAXS_USING_SIV3D)
            s3d::System::Exit();
#elif defined(PAXS_USING_SFML)
            window.close();
#endif
        }
    };
}

#endif // !PAX_GRAPHICA_WINDOW_HPP