#pragma once
#include <functional>

class FTimerManager {
public:
    static FTimerManager& GetInstance() {
        static FTimerManager Instance;
        return Instance;
    }
    void AddTimer(float Delay, std::function<void()> Callback);
    void Update(float DeltaTime);
    void ClearAllTimers()
    {
        Timers.clear();
    }

private:
    FTimerManager() = default; // 생성자 private/protected
    struct FTimer {
        float TimeLeft;
        std::function<void()> Callback;
    };
    std::vector<FTimer> Timers;
};
