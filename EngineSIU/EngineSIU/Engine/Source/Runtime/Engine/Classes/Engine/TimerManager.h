#pragma once
#include <functional>

#include "Container/Array.h"

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
        Timers.Empty();
    }

private:
    FTimerManager() = default; // 생성자 private/protected
    struct FTimer {
        float TimeLeft;
        std::function<void()> Callback;
    };
    TArray<FTimer> Timers;
};
