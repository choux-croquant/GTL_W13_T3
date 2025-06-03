// FTimerManager.cpp
#include "TimerManager.h"

#include "Container/Array.h"

void FTimerManager::AddTimer(float Delay, std::function<void()> Callback) {
    Timers.push_back({ Delay, Callback }); // Delay를 TimeLeft로 초기화
}

void FTimerManager::Update(float DeltaTime) {

    TArray<int32> PendingDestroyTimer;

    for (int i=0;i<Timers.size();i++)
    {
        Timers[i].TimeLeft -= DeltaTime;

        if (Timers[i].TimeLeft <= 0)
        {
            if (Timers[i].Callback)
            {
                Timers[i].Callback();
            }

            PendingDestroyTimer.Add(i);
        }
    }

    for (int32 t : PendingDestroyTimer)
    {
        Timers.erase(Timers.begin() + t);
    }
}
