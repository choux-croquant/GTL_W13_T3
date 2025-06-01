AnimFSM = {
    current = "Idle",
    Update = function(self, dt)

        -- self.current = "Contents/Fbx/Capoeira.fbx"
        self.current = "Contents/Enemy_Idle/Armature|Enemy_Idle"
        -- self.current = "Contents/Combo_RLU/Armature|Combo_RLU"

        return {
            anim = self.current,
            blend = 5.0
        }
    end
}

return AnimFSM