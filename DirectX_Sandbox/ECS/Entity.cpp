#include "Entity.h"

void Entity::RemoveComponent(int componentTypeID)
{
    assert(componentTypeID < MAX_COMPONENTS_PER_ENTITY);
    m_componentMask &= ~(1 << componentTypeID);
}
