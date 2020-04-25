#pragma once

namespace Juno
{
class PhysicsManager
{
public:
    static PhysicsManager& Get()
    {	
        static PhysicsManager s_instance;
        return s_instance;
    }
    PhysicsManager(const PhysicsManager& physicsManager) = delete;





private:
    PhysicsManager();

};
}