-- AnimFSM = {
--     current = "Idle",
--     Update = function(self, dt)

--         -- self.current = "Contents/Enemy_Idle/Armature|Enemy_Idle"
--         -- self.current = "Contents/Enemy_Impact/Armature|Enemy_Impact"
--         -- self.current = "Contents/Combo_RLU/Armature|Combo_RLU"
--         self.current = "Contents/GameJamEnemy/Armature|Combo_RLR"

--         return {
--             anim = self.current,
--             blend = 5.0
--         }
--     end
-- }

-- return AnimFSM

AnimFSM = {
    currentState = "Idle",
    attackAnimations = {
        "Contents/Combo_RLU/Armature|Combo_RLU",
        -- "Contents/GameJamEnemy/Armature|Combo_RLR",
    },
    reactionAnimation = "Contents/Enemy_Impact/Armature|Enemy_Impact",
    attackCooldown = 3.0,
    isAttacking = false,
    lastAttackTime = 0,

    TransitionToState = function(self, newState)
        self.currentState = newState
    end,

    Update = function(self, dt)
        if self.currentState == "Reacting" then
            return self:HandleReactionState()
        elseif self.currentState == "Attacking" then
            return self:HandleAttackState()
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
            anim = "Contents/Enemy_Idle/Armature|Enemy_Idle",
            blend = 0.2,
            loop = false,
            rate_scale = 1.0,
        }
    end,

    HandleAttackState = function(self)
        if not self.isAttacking then
            self.isAttacking = true
            self.selectedAttack = self.attackAnimations[math.random(#self.attackAnimations)]
            self.attackEndTime = os.clock() + 2
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
            rate_scale = 1.0,
        }
    end,

    HandleReactionState = function(self)
        local blend = 0.3
        if os.clock() > self.reactionEndTime then
            self:TransitionToState("Idle")
        end

        return {
            anim = self.reactionAnimation,
            blend = blend,
            loop = false,
            rate_scale = 1.0,
        }
    end,
}

-- 플레이어 블록 성공 시뮬레이션
-- function TestPlayerBlock()
--     AnimFSM:TriggerReaction()
-- end

return AnimFSM
