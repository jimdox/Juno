#include <vector>
#include "Entity.h"




/* handles creation/deletion of entities into scenes */
namespace bbx {
class EntityManager
{
public:
    EntityManager();
    ~EntityManager();                               /* delete all entities when deleted */
    void loadEntity(std::string& filepath);         /* construct entity at loc &filepath */
    Entity& getEntity(unsigned int n);
    void removeEntity(unsigned int n);
    

private:
    std::vector<Entity> entities;


};
}