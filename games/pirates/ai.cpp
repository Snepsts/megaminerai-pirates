// AI
// This is where you build your AI

#include "ai.hpp"

#include<map>
#include<string>
#include<algorithm>
#include<iostream>
// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
// <<-- /Creer-Merge: includes -->>


namespace cpp_client
{

namespace pirates
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Arrays Start at Zero";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
    //<<-- /Creer-Merge: ended -->>
}

bool AI::spawn_units()
{
    if (!player->port->tile->unit) {
        if (player->gold > game->ship_cost) {
            player->port->spawn("ship");
        }
    } else { //there's probably a ship maybe
        if (player->port->tile->unit->crew < 2)
            player->port->spawn("crew");
    }

    return true;
}

bool AI::run_unit_turn(Unit u)
{
    if (u->ship_health < this->game->ship_health / 2)
    {
        // Heal our unit if the ship is almost dead
        // Node: Crew also have their own health. Maybe try adding a check to see if the crew need healing?
        // Find a path to our port so we can heal
        std::vector<Tile> path = this->find_path(u->tile, player->port->tile, u);
        if (path.size() > 0)
        {
            // Move along the path if there is one
            while (path.size() > 0 && u->moves > 0) {
                u->move(path[0]);
                path.erase(path.begin());
            }
        }
        else
        {
            // Try to deposit any gold we have while we're here
            u->deposit();

            // Try to rest
            u->rest();
        }
    }
    else
    {
        // Try to attack a merchant
        // Look for a merchant ship
        Unit merchant = nullptr;
        std::vector<Unit> units = game->units;
        for (unsigned int i = 0; i < units.size(); i++)
        {
            if (units[i]->target_port != nullptr)
            {
                // Found one
                merchant = units[i];
                break;
            }
        }

        Unit enemy = nullptr;
        units = player->opponent->units;
        for (unsigned int i = 0; i < units.size(); i++)
        {
            if (units[i]->target_port != nullptr)
            {
                // Found one
                enemy = units[i];
                break;
            }
        }


        int merchant_path = 1000;
        int enemy_path = 1000;

        if (merchant)
            merchant_path = find_path(u->tile, merchant->tile, u).size();
        
        if (enemy)
            enemy_path = find_path(u->tile, enemy->tile, u).size();
        
        Unit target;

        if (enemy_path == 1000 && merchant_path == 1000)
            return false;

        if (enemy_path > merchant_path)
            target = merchant;
        else
            target = enemy;

        // If we found a target, move to it, then attack it
        if (target != nullptr)
        {
            // Find a path to this target
            std::vector<Tile> path = this->find_path(u->tile, target->tile, u);
            if (path.size() > this->game->ship_range)
            {
                while (path.size() > 0 && u->moves > 0) {
                    u->move(path[0]);
                    path.erase(path.begin());
                    if (path.size() > this->game->ship_range && u->moves > 0) {
                        u->attack(target->tile, "ship");
                        break;
                    }
                }
            }
            else
            {
                // Try to attack the merchant's ship
                u->attack(target->tile, "ship");
            }
        }
    }

    // <<-- /Creer-Merge: runTurn -->>
    return true;
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    // <<-- Creer-Merge: runTurn -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
 
    spawn_units();

    auto my_units = player->units;

    for (auto unit : my_units) {
        run_unit_turn(unit);
    }

    // <<-- /Creer-Merge: runTurn -->>
    return true;
}

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <param name="unit">the Unit that will move</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> AI::find_path(const Tile& start, const Tile& goal, const Unit& unit)
{
    // no need to make a path to here...
    if(start == goal)
    {
        return {};
    }

    // the tiles that will have their neighbors searched for 'goal'
    std::queue<Tile> fringe;

    // How we got to each tile that went into the fringe.
    std::unordered_map<Tile,Tile> came_from;

    // Enqueue start as the first tile to have its neighbors searched.
    fringe.push(start);

    // keep exploring neighbors of neighbors... until there are no more.
    while(fringe.size() > 0)
    {
        // the tile we are currently exploring.
        Tile inspect = fringe.front();
        fringe.pop();

        // Note, we are using the `auto` keyword here
        //   The compiler can discern that this is a `std::vector<Tile>` based on the return type of `getNeighbors()`
        auto neighbors = inspect->get_neighbors();

        // cycle through the tile's neighbors.
        for(unsigned i = 0; i < neighbors.size(); i++)
        {
            Tile neighbor = neighbors[i];

            // If we found the goal we've found the path!
            if(neighbor == goal)
            {
                // Follow the path backward starting at the goal and return it.
                std::deque<Tile> path;
                path.push_front(goal);

                // Starting at the tile we are currently at, insert them retracing our steps till we get to the starting tile
                for(Tile step = inspect; step != start; step = came_from[step])
                {
                    path.push_front(step);
                }

                // we want to return a vector as that's what we use for all containers in C++
                // (and they don't have push_front like we need)
                // So construct the vector-ized path here
                std::vector<Tile> vector_path;
                for(auto& tile : path)
                {
                    vector_path.push_back(tile);
                }
                return vector_path;
            }

            // if the tile exists, has not been explored or added to the fringe yet, and it is pathable
            if(neighbor && came_from.count(neighbor) == 0 && neighbor->is_pathable(unit))
            {
                // add it to the tiles to be explored and add where it came from.
                fringe.push(neighbor);
                came_from[neighbor] = inspect;
            }

        } // for each neighbor

    } // while fringe not empty

    // if you're here, that means that there was not a path to get to where you want to go.
    //   in that case, we'll just return an empty path.
    return {};
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.

bool is_ship(Unit u)
{
    return u->ship_health > 0;
}

//<<-- /Creer-Merge: methods -->>

} // pirates

} // cpp_client
