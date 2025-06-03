AnimFSM = {
    --애니메이션 이름 바꾸기 귀찮아서 애니메이션은 막을수 있는 적공격 패리라고 명명돼있음
    currentState = "Idle",

    AnimMap = {
        VerticalFastParry = "Contents/Player/Armature|VerticalFastParry",
        VerticalHardParry = "Contents/Player/Armature|VerticalHardParry",
        HorizontalFastParry = "Contents/Player/Armature|HorizontalFastParry",
        HorizontalHardParry = "Contents/Player/Armature|HorizontalHardParry",
        DamageReact = "Contents/Player/Armature|DamageReact",
        Die = "Contents/Player/Armature|Die",
        Idle = "Contents/Player/Armature|Idle",
    },
    isAttacking = false,
    lastAttackTime = 0,
    isParrying = false,
    isReacting = false,
    AnimStartTime = 0,
    parrySpeed = 1.0,
    timer = 0,

    GetAnimName = function(self, InState)
        return self.AnimMap[InState]
    end,
    
    TransitionToState = function(self, newState)
        self.currentState = newState
    end,

    Update = function(self, dt)

        self.timer = self.timer + dt

        if self.currentState == "Die" then
            return self:HandleDieState()
        end

        self:ParryEnd()
        self:ParryCheck()

        if self.currentState == "DamageReact" then
            return self:HandleDamageReactionState()
        elseif self.currentState == "VerticalFastParry" then
            return self:HandleVerticalFastParryState() 
        elseif self.currentState == "VerticalHardParry" then
            return self:HandleVerticalHardParryState() 
        elseif self.currentState == "HorizontalFastParry" then
            return self:HandleHorizontalFastParryState() 
        elseif self.currentState == "HorizontalHardParry" then
            return self:HandleHorizontalHardParryState() 
        else
            return self:HandleIdleState()
        end
    end,

    HandleIdleState = function(self)

        return {
            anim = self.AnimMap["Idle"],
            blend = 0.0,
            loop = true, 
            rate_scale = 1.0,
            state = self.currentState,
        }
    end,

    ChangeParryState = function(self, bIsParrying)
        self.isParrying = bIsParrying
    end,

    ParryEnd = function(self)
        if self.isParrying then
            if self.timer >= self.AnimStartTime + (self.CurrentAnimDuration / self.parrySpeed) then
                self:TransitionToState("Idle")

                print("ParryEnd")

                if self.isReacting then
                    isReacting = false
                    self:ChangeParryState(false)
                end
            end
        end
    end,

    ParryCheck = function(self)
        if self.isParrying then
            if self.timer > self.AnimStartTime + (self.CurrentAnimDuration / self.parrySpeed) * 2 then
                self:ChangeParryState(false)
                self.lastAttackTime = self.timer
            end
        end
    end,    

    HandleVerticalFastParryState = function(self)
        if self.isParrying then
            return
        end

        self:ChangeParryState(true)
        self.AnimStartTime = self.timer
        self.parrySpeed = 1.2

        return {
            anim = self.AnimMap["VerticalFastParry"],
            blend = 0.1,
            loop = false,
            rate_scale = self.parrySpeed,
            state = self.currentState,
        }
    end,

    HandleVerticalHardParryState = function(self)
        if self.isParrying then
            return
        end

        self:ChangeParryState(true)
        self.AnimStartTime = self.timer
        self.parrySpeed = 1.2

        return {
            anim = self.AnimMap["VerticalHardParry"],
            blend = 0.1,
            loop = false,
            rate_scale = self.parrySpeed,
            state = self.currentState,
       }
    end,

    HandleHorizontalFastParryState = function(self)
        if self.isParrying then
            return
        end

        self:ChangeParryState(true)
        self.AnimStartTime = self.timer
        self.parrySpeed = 1.2

        return {
            anim = self.AnimMap["HorizontalFastParry"],
            blend = 0.1,
            loop = false,
            rate_scale = self.parrySpeed,
            state = self.currentState,
      }
    end,

    HandleHorizontalHardParryState = function(self)
        if self.isParrying then
            return
        end

        self:ChangeParryState(true)
        self.AnimStartTime = self.timer
        self.parrySpeed = 1.2

        return {
            anim = self.AnimMap["HorizontalHardParry"],
            blend = 0.1,
            loop = false,
            rate_scale = self.parrySpeed,
            state = self.currentState,
    }
    end,

    HandleDieState = function(self)

        self.isParrying = false
        self.parrySpeed = 1.0

        return {
            anim = self.AnimMap["Die"],
            blend = 0.3,
            loop = false,
            rate_scale = self.parrySpeed,
            state = self.currentState,
        }
    end,

    HandleDamageReactionState = function(self)
        if self.isParrying then
            return
        end
        
        self:ChangeParryState(true)
        self.AnimStartTime = self.timer
        self.parrySpeed = 1.0
        self.isReacting = true

        return {
            anim = self.AnimMap["DamageReact"],
            blend = 0.3,
            loop = false,
            rate_scale = 1.0,
            state = self.currentState,
        }
    end,
}

return AnimFSM