/*
 * EntityMap.cpp
 */

#include "stdinclude.hpp"
#include "GeometricPrimitives.hpp"
#include "Elements.hpp"
#include "PlatformInterface.hpp"
#include "EntityMap.hpp"

/*
 * Computes which entities in the map intersect `EntityAABB *e`
 *
 * Returns `false` if there is a collision.
 */
bool EntityMap::computeEntityCollisions(const Entity *e, set<Entity *> &collidingEntities)
{
    if( collidingEntities.size() != 0 )
        throw logic_error("EntityMap::computeEntityCollisions -- \
non-empty EntityCollision collidingEntities set");

    if( !isInsideMap(*e) )
        return false;

    set<Entity *> entities = optmat.getEntities( Rect(e->pos, e->d->getSize()) );
    for(auto m_e : entities)
        if( Rect(e->pos, e->d->getSize()).
                doesIntersect( Rect(m_e->pos, m_e->d->getSize()) ) )
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
        throw logic_error("EntityMap::place -- \
attempt to place an existing entity on the map");

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
        throw logic_error("EntityMap::remove -- \
attempt to remove an entity that does not exist on the map");

    optmat.erase(e);
    entities.erase(e);
}

/*
 * Move an existing entity `e` to a new position
 */
bool EntityMap::move(Entity *e, xy newPos, set<Entity *> &collidingEntities)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::move -- \
attempt to move an entity that does not exist on the map");

    // If `e` is already at `newPos`, don't do anything
    if(e->pos == newPos)
        return true;

    // First remove `e` from the map
    remove(e);

    xy oldPos = e->pos;
    e->pos = newPos;

    // Attempt to place `e` at `newPos`
    if( !place(e, collidingEntities) ) {
        // If the above failed, then restore `e` to its old location
        e->pos = oldPos;
        set<Entity *> temp;
        if( !place(e, temp) ) // place it back
            throw logic_error("EntityMap::move -- \
unexpected fatal error: trouble placing an entity back at the same position it was moved from.");
        return false;
    }

    return true;
}

/*
 * Test whether a move will be possible (use sparingly)
 *
 * Note: This function should only be used when the move is never
 * intended to be made. For intended moves, use `EntityMap::move`
 */
bool EntityMap::moveTest(Entity *e, xy newPos)
{
    if( entities.find(e) == entities.end() )
        throw logic_error("EntityMap::move -- \
attempt to move an entity that does not exist on the map");

    remove(e);

    xy oldPos = e->pos;
    e->pos = newPos;

    set<Entity *> collidingEntities;
    bool success = place(e, collidingEntities);

    if(success)
        remove(e);
    else
        collidingEntities.clear();

    e->pos = oldPos;
    place(e, collidingEntities);

    return success;
}

static int non_zero_abs_min(int a, int b)
{
    int r = min(abs(a), abs(b));
    if( r == 0 )
        r = max(abs(a), abs(b));
    return r;
}

/*
 * Move entity *e as close as possible by `distance` until there until there is a collision.
 *
 * First it attempts a naive move. If there are no collisions, then it is successful.
 * If there is a collision, this function moves the entity slowly in small steps.
 * In each step, the entity is moved by `step_dist`, until there is a collision.
 */
bool EntityMap::moveBy(Entity *e, xy distance,  set<Entity *> &collidingEntities)
{
    if( move(e, e->pos + distance, collidingEntities) )
        return true;

    collidingEntities.clear();

    int min_dist = non_zero_abs_min( abs(distance.x), abs(distance.y) );
    xy step_dist = distance / min_dist;

    if( move(e, e->pos + step_dist, collidingEntities) )
        // tail recursive call:
        return moveBy(e, distance - step_dist, collidingEntities);

    return false;
}

/*
 * Insert an entity in the optimization matrix.
 *
 * Note: This function does not check for collisions.
 *       To check for collisions, use EntityMap::place().
 */
void EntityMap::OptimizationMatrix::insert(Entity *e)
{
    xy bl = optMatPos(e->pos), tr = optMatPos(e->pos + e->d->getSize());
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            matrix[x][y].insert(e);
}

/*
 * Remove entity from optimization matrix.
 */
void EntityMap::OptimizationMatrix::erase(Entity *e)
{
    xy bl = optMatPos(e->pos), tr = optMatPos(e->pos + e->d->getSize());
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            matrix[x][y].erase(e);
}

/*
 * Get all the entities that fall within a certain (rectangular) region.
 */
set<Entity *> EntityMap::OptimizationMatrix::getEntities(Rect region)
{
    set<Entity *> entities;

    xy bl = optMatPos(region.pos), tr = optMatPos(region.pos + region.size);
    for(int x = bl.x ; x <= tr.x ; x++)
        for(int y = bl.y ; y <= tr.y ; y++)
            if(!matrix[x][y].empty())
                for(auto e : matrix.at(x, y))
                    entities.insert(e);

    return entities;
}
