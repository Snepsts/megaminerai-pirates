#ifndef GAMES_PIRATES_AI_HPP
#define GAMES_PIRATES_AI_HPP


#include "impl/pirates.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "player.hpp"
#include "port.hpp"
#include "tile.hpp"
#include "unit.hpp"

#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/attr_wrapper.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional #includes here
// <<-- /Creer-Merge: includes -->>

namespace cpp_client
{

namespace pirates
{

/// <summary>
/// This is the header file for building your Pirates AI
/// </summary>
class AI : public Base_ai
{
public:
    /// <summary>
    /// This is a reference to the Game object itself, it contains all the information about the current game
    /// </summary>
    Game game;

    /// <summary>
    /// This is a pointer to your AI's player. This AI class is not a player, but it should command this Player.
    /// </summary>
    Player player;
    //<<-- Creer-Merge: class variables -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional class variables here.
    //<<-- /Creer-Merge: class variables -->>

    /// <summary>
    /// This returns your AI's name to the game server.
    /// Replace the string name.
    /// </summary>
    /// <returns>The name of your AI.</returns>
    virtual std::string get_name() const override;

    /// <summary>
    /// This is automatically called when the game first starts, once the game objects are created
    /// </summary>
    virtual void start() override;

    /// <summary>
    /// This is automatically called when the game ends.
    /// </summary>
    /// <param name="won">true if you won, false otherwise</param>
    /// <param name="reason">An explanation for why you either won or lost</param>
    virtual void ended(bool won, const std::string& reason) override;

    /// <summary>
    /// This is automatically called the gfind_pathame (or anything in it) updates
    /// </summary>
    virtual void game_updated() override;

    /// <summary>
    /// This is called every time it is this AI.player's turn.
    /// </summary>
    /// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
    bool run_turn();

    // <<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional methods here.
    
    void get_action();

    //
    //Finder Definitions
    //
    std::vector<Unit> get_enemy_crew();
    std::vector<Unit> get_enemy_ships();
    std::vector<Tile> get_list_of_enemy_treasure();
    Tile get_closest_enemy_treasure(Unit u);
    Tile get_closest_enemy_ship(Unit u);
    Tile get_closest_merchant_ship(Unit u);
    int get_close_enemy_ships(Unit u);
    Tile get_closest_empty_ship(Unit u);
    Tile get_closest_port(Unit u);
    Tile get_closest_tile_from_options(Unit u, std::vector<Tile> tile_options);
    std::vector<std::vector<Tile>> get_all_possible_paths_to_options(Unit u, std::vector<Tile> tile_options);

    
    Tile get_nearest_port(Unit u);

    //
    //Action Definitions
    //
    bool heal_ship(Unit u);
    bool deposit_treasure_in_home(Unit u);
    bool steal_enemy_treasure(Unit u);
    bool steal_enemy_ship(Unit u);
    bool destroy_enemy_ship(Unit u);
    bool destroy_merchant_ship(Unit u);
    bool unit_retreat_and_rest(Unit u);
    bool board_empty_ship(Unit u);
    bool crew_bury_treasure(Unit u);
    bool crew_dig_treasure(Unit u, Tile t);
    bool pickup_units_with_gold(Unit u);
    bool ship_steal_enemy_treasure(Unit u);

    //
    // Helper method delcarations
    //
    bool move_towards_enemy_treasure(Unit u);
    bool move_to_tile(Unit u, Tile t);
    bool move_next_to_tile(Unit u, Tile t);
    bool is_ship(Unit u);
    int distance_to_port(Unit u);
    int distance_to_unit(Unit u1, Unit u2);

    //
    // Properties
    //
    std::vector<Tile> buried_treasure_vec;

    // <<-- /Creer-Merge: methods -->>
    void spawn_units();
    bool run_ship_turn(Unit u);
    bool run_crew_turn(Unit u);
    bool run_ship_attack(Unit u);

    //fuzzy controllers
    bool attack_instead_of_steal(Unit u);
    bool go_deposit_ship(Unit u);

    //fuzzy stuff
    float get_ship_aggressiveness(Unit u);
    float get_ship_danger_level(Unit u);
    float get_ship_health_value(Unit u);
    float get_crew_dig_fuzzy(Unit u, Tile t);
    float closeness_to_gold(Unit u);
    float ship_richness(Unit u);
    float closeness_to_port(Unit u);

    //fuzzy helpers
    int get_distance_to_enemy_gold(Unit u);

    bool fuzzy_crew_dig(Unit u, Tile t);
    float get_enemy_ship_crew_value(Unit u, Tile enemy_ship);
    float get_enemy_ship_health_value(Unit u, Tile enemy_ship);

    bool fuzzy_steal_or_destroy_enemy_ship(Unit u);
    bool fuzzy_go_heal_ship(Unit u);
    bool fuzzy_go_heal_crew(Unit u);

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <param name="unit">the Unit that will move</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> find_path(const Tile& start, const Tile& goal, const Unit& unit);



    // ####################
    // Don't edit these!
    // ####################
    /// \cond FALSE
    virtual std::string invoke_by_name(const std::string& name,
                                       const std::unordered_map<std::string, Any>& args) override;
    virtual void set_game(Base_game* ptr) override;
    virtual void set_player(std::shared_ptr<Base_object> obj) override;
    virtual void print_win_loss_info() override;
    /// \endcond
    // ####################
    // Don't edit these!
    // ####################

};

} // PIRATES

} // cpp_client

#endif // GAMES_PIRATES_AI_HPP
