AnimFSM = {
    currentState = "Idle",
    attackAnimations = {
        "Contents/Horizontal1/Armature|Horizontal1",
        -- "Contents/Horizontal2/Armature|Horizontal2",
        -- "Contents/Vertical1/Armature|Vertical1",
    },
    reactionAnimation = "Contents/Enemy_Impact/Armature|Enemy_Impact",
    attackCooldown = 3.0,
    isAttacking = false,
    isReacting = false,
    lastAttackTime = 0,
    reactionEndTime = 0,

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
}

-- 플레이어 블록 성공 시뮬레이션
-- function TestPlayerBlock()
--     AnimFSM:TriggerReaction()
-- end

return AnimFSM
