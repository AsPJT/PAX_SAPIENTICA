/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TIME_CONTROLLER_HPP
#define PAX_SAPIENTICA_TIME_CONTROLLER_HPP

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>

namespace paxs {

    // 時間制御ロジック専用クラス
    class TimeController {
    public:
        // 再生状態
        enum class PlaybackState {
            Paused,      // 停止
            Playing,     // 再生（未来へ）
            Reversing    // 逆再生（過去へ）
        };

        // 初期化
        TimeController() = default;

        // 再生制御
        void play() {
            playback_state_ = PlaybackState::Playing;
        }

        void pause() {
            playback_state_ = PlaybackState::Paused;
        }

        void reverse() {
            playback_state_ = PlaybackState::Reversing;
        }

        // 状態取得
        bool isPlaying() const {
            return playback_state_ == PlaybackState::Playing;
        }

        bool isPaused() const {
            return playback_state_ == PlaybackState::Paused;
        }

        bool isReversing() const {
            return playback_state_ == PlaybackState::Reversing;
        }

        PlaybackState getPlaybackState() const {
            return playback_state_;
        }

        // 速度制御
        void setSpeed(double speed) {
            if (speed >= min_speed_ && speed <= max_speed_) {
                speed_ = speed;
            }
        }

        double getSpeed() const {
            return speed_;
        }

        void speedUp() {
            setSpeed(speed_ * 2.0);
        }

        void slowDown() {
            setSpeed(speed_ / 2.0);
        }

        void resetSpeed() {
            speed_ = 1.0;
        }

        // 速度範囲の取得
        double getMinSpeed() const { return min_speed_; }
        double getMaxSpeed() const { return max_speed_; }

    private:
        PlaybackState playback_state_ = PlaybackState::Paused;
        double speed_ = 1.0;
        const double min_speed_ = 0.1;
        const double max_speed_ = 100.0;
    };

}

#endif // !PAX_SAPIENTICA_TIME_CONTROLLER_HPP
