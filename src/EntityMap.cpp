/*
 * EntityMap.cpp
 */

#include "EntityMap.hpp"

/*
 * Insert an entity in the optimization matrix.
 *
 * Note: This function does not check for collisions.
 *       To check for collisions, use EntityMap::place().
 */
void OptimizationMatrix::insert(Entity *e)
{
    PtI bl = matrixBottomLeft( Rect(e->pos, e->d->getSize()) ), tr = matrixTopRight( Rect(e->pos, e->d->getSize()) );
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            mat.at(x,y).insert(e);
}

/*
 * Remove entity from optimization matrix.
 */
void OptimizationMatrix::erase(Entity *e)
{
    PtI bl = matrixBottomLeft( Rect(e->pos, e->d->getSize()) ), tr = matrixTopRight( Rect(e->pos, e->d->getSize()) );
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            mat.at(x,y).erase(e);
}

/*
 * Get all the entities that fall within a certain (rectangular) region.
 */
set<Entity *> OptimizationMatrix::getEntities(Rect region)
{
    set<Entity *> entities;

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
bool EntityMap::computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities)
{
    if( collidingEntities.size() != 0 )
        throw logic_error("EntityMap::computeEntityCollisions -- non-empty EntityCollision collidingEntities set");

    if( !isInsideMap(*e) )
        return false;

    set<Entity *> entities = optmat.getEntities( Rect(e->pos, e->d->getSize()) );
    for(auto m_e : entities)
        if( Rect(e->pos, e->d->getSize()).doesIntersect( Rect(m_e->pos, m_e->d->getSize()) ) )
            collidingEntities.insert(m_e);

    if( !collidingEntities.empty() )
        return false;

    return true;
}

/*
 * Place an entity on the map
 */
bool EntityMap::place(Entity *e,  set<Entity *> &collidingEntities)
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
void EntityMap::remove(Entity *e)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::remove -- attempt to remove an entity that does not exist on the map");

    optmat.erase(e);
    entities.erase(e);
}

/*
 * Move an existing entity `e` to a new position
 */
bool EntityMap::move(Entity *e, Pt newPos,  set<Entity *> &collidingEntities)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::move -- attempt to move an entity that does not exist on the map");

    if(e->pos == newPos)
        return true;

    remove(e);

    Pt oldPos = e->pos;
    e->pos = newPos;

    if( !place(e, collidingEntities) ) {
        e->pos = oldPos;
        set<Entity *> temp;
        if( !place(e, temp) ) // place it back
            throw logic_error("EntityMap::move -- unexpected fatal error: trouble placing an entity back at the same position it was moved from.");
        return false;
    }

    return true;
}

/*
 * Move entity *e as close as possible by `distance` until there until there is a collision.
 *
 * First it attempts a naïve move. If there are no collisions, then it is successful.
 * If there is a collision, this function moves the entity slowly in small steps.
 * In each step, the entity is moved by a `short_distance`, until there is a collision.
 */
bool EntityMap::moveBy(Entity *e, Pt distance,  set<Entity *> &collidingEntities)
{
    if( !move(e, e->pos + distance, collidingEntities) )
    {
        collidingEntities.clear();

        float divisor = min( abs(distance.x), abs(distance.y) );
        if(divisor == 0) // then get the other value:
            divisor = max( abs(distance.x), abs(distance.y) ) * 10;

        Pt short_distance =  distance / divisor;
        while( move(e, e->pos + short_distance, collidingEntities) ) {}

        return false;
    }
    return true;
}
