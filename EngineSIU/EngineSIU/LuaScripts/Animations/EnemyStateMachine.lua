AnimFSM = {
    currentState = "Idle",
    idleAnimation = "Contents/Enemy_Idle/Armature|Enemy_Idle",
    attackAnimations = {
        "Contents/Horizontal1/Armature|Horizontal1",
        -- "Contents/Horizontal2/Armature|Horizontal2",
        -- "Contents/Vertical1/Armature|Vertical1",
    },
    reactionAnimation = "Contents/Enemy_Impact/Armature|Enemy_Impact",
    kneelAnimation = "Contents/Kneel/Armature|Kneel",
    kneelIdleAnimation = "Contents/Kneel_Idle/Armature|Kneel_Idle",
    attackCooldown = 3.0,
    isAttacking = false,
    isReacting = false,
    isDefeated = false,
    lastAttackTime = 0,
    reactionEndTime = 0,
    defeatStartTime = 0,

    TransitionToState = function(self, newState)
        self.currentState = newState
    end,

    Update = function(self, dt)
        if self.currentState == "Reacting" then
            return self:HandleReactionState()
        elseif self.currentState == "Attacking" then
            return self:HandleAttackState()
        elseif self.currentState == "Defeat" then
            return self:HandleDefeatState()
        else
            return self:HandleIdleState(dt)
        end
    end,

    HandleIdleState = function(self, dt)
        -- 공격 쿨타임 체크
        if os.clock() - self.lastAttackTime > self.attackCooldown then
            self:TransitionToState("Attacking")
        end

        return {
            anim = idleAnimation,
            blend = 0.2,
            loop = true,
            rate_scale = 1.0,
            state = self.currentState
        }
    end,

    HandleAttackState = function(self)
        if not self.isAttacking then
            self.isAttacking = true
            self.selectedAttack = self.attackAnimations[math.random(#self.attackAnimations)]
            self.attackEndTime = os.clock() + self.CurrentAnimDuration * 1.25;
        end

        if os.clock() > self.attackEndTime then
            self.isAttacking = false
            self.lastAttackTime = os.clock()
            self:TransitionToState("Idle")
        end

        return {
            anim = self.selectedAttack,
            blend = 0.1,
            loop = false,
            rate_scale = 0.8,
            state = self.currentState
        }
    end,

    HandleReactionState = function(self)
        if not self.isReacting then
            self.isReacting = true
            self.isAttacking = false
            self.lastAttackTime = os.clock()
            self.reactionEndTime = os.clock() + self.CurrentAnimDuration;
        end

        if self.reactionEndTime and os.clock() > self.reactionEndTime then
            self.isReacting = false
            self:TransitionToState("Idle")
        end

        return {
            anim = self.reactionAnimation,
            blend = 0.1,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState
        }
    end,

    HandleDefeatState = function(self)
        if not self.isDefeated then
            -- 최초 진입 시 Kneel 애니메이션 실행
            self.isDefeated = true
            self.defeatStartTime = os.clock()
            return {
                anim = self.kneelAnimation,
                blend = 0.3,
                loop = false,
                rate_scale = 1.0,
                state = self.currentState
            }
        else
            local kneelDuration = self.CurrentAnimDuration
            -- local kneelDuration = 0.4 
            -- if os.clock() > self.defeatStartTime + kneelDuration then
            --     return {
            --         anim = self.kneelIdleAnimation,
            --         blend = 4.0,
            --         loop = true,
            --         rate_scale = 1.0,
            --         state = self.currentState
            --     }
            if os.clock() > self.defeatStartTime + kneelDuration then
                return {
                    anim = self.kneelAnimation,
                    blend = 0.0,
                    loop = false,
                    rate_scale = 1.0,
                    state = self.currentState
                }
            -- else
            --     return {
            --     anim = self.kneelAnimation,
            --     blend = 0.0,
            --     loop = false,
            --     rate_scale = 1.0,
            --     state = self.currentState
            -- }
            end
        end
    end,
}

-- 플레이어 블록 성공 시뮬레이션
-- function TestPlayerBlock()
--     AnimFSM:TriggerReaction()
-- end

return AnimFSM
