// FTimerManager.cpp
#include "TimerManager.h"

void FTimerManager::AddTimer(float Delay, std::function<void()> Callback) {
    Timers.push_back({ Delay, Callback }); // Delay를 TimeLeft로 초기화
}

void FTimerManager::Update(float DeltaTime) {
    for (auto it = Timers.begin(); it != Timers.end();) {
        it->TimeLeft -= DeltaTime;
        
        if (it->TimeLeft <= 0.0f) {
            // 1. 콜백 실행
            if (it->Callback) {
                it->Callback(); 
            }
            
            // 2. 타이머 제거 (erase는 다음 요소를 가리키는 iterator 반환)
            it = Timers.erase(it); 
        } else {
            // 3. 다음 타이머로 이동
            ++it; 
        }
    }
}
