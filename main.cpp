#pragma warning(disable : 4244)
#pragma warning(disable : 4530)

#include <raylib-cpp.hpp>

#include "include/Game.hpp"

int main() {
#ifdef _DEBUG
    SetTraceLogLevel(TraceLogLevel::LOG_TRACE);
#else
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
#endif

    constexpr static auto WINDOW_FLAGS = FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_BORDERLESS_WINDOWED_MODE;
    static auto window = RWindow(800, 600, "PolyformRPG 2D Game", WINDOW_FLAGS);

    window
        .SetTargetFPS(60)
        .SetExitKey(KEY_NULL);

    static Game game;
    while (!window.ShouldClose()) {
        game.update();
        game.render(window);
    }
}