/*
 * EntityMap.cpp
 */

#include "EntityMap.hpp"

/*
 * Insert entity in optimization matix.
 *
 * Does not check for collisions. To check
 * for collisions, use EntityMap::place(...).
 */
void OptimizationMatrix::insert(EntityAABB *e)
{
    PtI bl = matrixBottomLeft(e->rect), tr = matrixTopRight(e->rect);
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            mat.at(x,y).insert(e);
}

/*
 * Remove entity from optimization matix.
 */
void OptimizationMatrix::erase(EntityAABB *e)
{
    PtI bl = matrixBottomLeft(e->rect), tr = matrixTopRight(e->rect);
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            mat.at(x,y).erase(e);
}

/*
 * Get all the entities that fall within a certain (rectangular) region.
 */
set<EntityAABB *> OptimizationMatrix::getEntities(Rect region)
{
    set<EntityAABB *> entities;

    PtI bl = matrixBottomLeft(region), tr = matrixTopRight(region);
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            if(!mat.at(x,y).empty())
                for(auto e : mat.at(x,y))
                    entities.insert(e);

    return entities;
}

/*
 * Computes which entities in the map intersect `EntityAABB *e`
 *
 * Returns `false` if there is a collision.
 */
bool EntityMap::computeEntityCollisions(const EntityAABB *e, set<EntityAABB *> &collidingEntities)
{
    if( collidingEntities.size() != 0 )
        throw logic_error("EntityMap::computeEntityCollisions -- non-empty EntityCollision collidingEntities set");

    if( !isInsideMap(*e) )
        return false;

    set<EntityAABB *> entities = optmat.getEntities(e->rect);
    for(auto m_e : entities)
        if( e->rect.doesIntersect((m_e->rect)) )
            collidingEntities.insert(m_e);

    if( !collidingEntities.empty() )
        return false;

    return true;
}

/*
 * Place an entity on the map
 */
bool EntityMap::place(EntityAABB *e,  set<EntityAABB *> &collidingEntities)
{
    if( entities.find(e) != entities.end() )
        throw logic_error("EntityMap::place -- attempt to place an existing entity on the map");

    if( !computeEntityCollisions(e, collidingEntities) )
        return false;

    optmat.insert(e);
    entities.insert(e);

    return true;
}

/*
 * Remove an entity from the map
 */
void EntityMap::remove(EntityAABB *e)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::remove -- attempt to remove an entity that does not exist on the map");

    optmat.erase(e);
    entities.erase(e);
}

/*
 * Move an existing entity `e` to a new position
 */
bool EntityMap::move(EntityAABB *e, Pt newPos,  set<EntityAABB *> &collidingEntities)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::move -- attempt to move an entity that does not exist on the map");

    if(e->rect.pos == newPos)
        return true;

    remove(e);

    Pt oldPos = e->rect.pos;
    e->rect.pos = newPos;

    if( !place(e, collidingEntities) ) {
        e->rect.pos = oldPos;
        set<EntityAABB *> temp;
        if( !place(e, temp) ) // place it back
            throw logic_error("EntityMap::move -- unexpected fatal error: trouble placing an entity back at the same position it was moved from.");
        return false;
    }

    return true;
}

/*
 * Move entity *e as close as possible by `distance` until there until there is a collision.
 *
 * First it attempts a naïve move. If there is no collision, good.
 * When there's a collision, `moveBy` moves the entity by small steps
 * defined by `divisor` (derived from `distance`) until there is a collision.
 */
bool EntityMap::moveBy(EntityAABB *e, Pt distance,  set<EntityAABB *> &collidingEntities)
{
    if( !move(e, e->rect.pos + distance, collidingEntities) )
    {
        collidingEntities.clear();

        float divisor = min( abs(distance.x), abs(distance.y) );
        if(divisor == 0) // then get the other value:
            divisor = max( abs(distance.x), abs(distance.y) ) * 10;

        Pt short_distance =  distance / divisor;
        while( move(e, e->rect.pos + short_distance, collidingEntities) ) {}

        return false;
    }
    return true;
}

/*
 * Invoke step() on each entity.
 */
void EntityMap::step()
{
    for(auto e : entities)
        e->step();
}
