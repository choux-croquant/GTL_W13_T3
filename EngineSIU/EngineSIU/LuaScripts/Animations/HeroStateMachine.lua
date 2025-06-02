AnimFSM = {
    currentState = "Idle",
    verticalFastParryAnimation = "Contents/Player/Armature|VerticalFastParry",
    verticalHardParryAnimation = "Contents/Player/Armature|VerticalHardParry",
    HorizontalFastParryAnimation = "Contents/Player/Armature|HorizontalFastParry",
    HorizontalHardParryAnimation = "Contents/Player/Armature|HorizontalHardParry",
    
    reactionAnimation = "Contents/Player/Armature|DamageReact",
    dieAnimation = "Contents/Player/Armature|Die",
    idleAnimation = "Contents/Player/Armature|Idle",
    isAttacking = false,
    lastAttackTime = 0,
    reactionEndTime = 0,
    isParrying = false,
    AnimStartTime = 0,
    
    
    TransitionToState = function(self, newState)
        self.currentState = newState
    end,

    Update = function(self, dt)

        self:ParryEnd()

        if self.currentState == "React" then
            return self:HandleReactionState()
        elseif self.currentState == "VerticalFastParry" then
            return self:HandleVerticalFastParryState() 
        elseif self.currentState == "VerticalHardParry" then
            return self:HandleVerticalHardParryState() 
        elseif self.currentState == "HorizontalFastParry" then
            return self:HandleHorizontalFastParryState() 
        elseif self.currentState == "HorizontalHardParry" then
            return self:HandleHorizontalHardParryState() 
        elseif self.currentState == "Die" then
            return self:HandleDieState()
        else
            return self:HandleIdleState()
        end
    end,

    HandleIdleState = function(self)
        -- 공격 쿨타임 체크
        -- 키입력으로 체크 쿨다운도 체크해야하긴함

        return {
            anim = self.idleAnimation,
            blend = 0.2,
            loop = true, 
            rate_scale = 1.0,
            state = self.currentState,
        }
    end,


    ParryEnd = function(self)
        if self.isParrying then
            if os.clock() > self.AnimStartTime + self.CurrentAnimDuration then
                print("Hello")
                self.isParrying = false
                self.lastAttackTime = os.clock()
                self:TransitionToState("Idle")
            end
        end
    end,

    HandleVerticalFastParryState = function(self)
        if self.isParrying then
            return
        end

        self.isParrying = true
        self.AnimStartTime = os.clock()

        return {
            anim = self.verticalFastParryAnimation,
            blend = 0.1,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState,
        }
    end,

    HandleVerticalHardParryState = function(self)
        if self.isParrying then
            return
        end

        self.isParrying = true
        self.AnimStartTime = os.clock()

        return {
            anim = self.verticalHardParryAnimation,
            blend = 0.1,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState,
       }
    end,

    HandleHorizontalFastParryState = function(self)

        if self.isParrying then
            return
        end

        self.isParrying = true
        self.AnimStartTime = os.clock()

        return {
            anim = self.HorizontalFastParryAnimation,
            blend = 0.1,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState,
      }
    end,

    HandleHorizontalHardParryState = function(self)
        if self.isParrying then
            return
        end

        self.isParrying = true
        self.AnimStartTime = os.clock()

        return {
            anim = self.HorizontalHardParryAnimation,
            blend = 0.1,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState,
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
            state = self.currentState,
      }
    end,
}

return AnimFSM