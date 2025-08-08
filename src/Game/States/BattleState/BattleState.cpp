#include "BattleState.hpp"
#include "../StateMachine/StateMachine.hpp"
#include "../SettingsState/SettingsState.hpp"

BattleState::BattleState(StateData& data, StateMachine& machine, sf::RenderWindow& window, const bool replace)
: State { data, machine, window, replace }
{
    state_machine.is_init = true;
}

void BattleState::init()
{
    data.camera.setDefaultView();
    data.camera.reset();

    battle_map.init();
    battle_map.loadMap("map_save.json");

    texture = ResourceLoader::instance().getTexture("Swordman.png");

    GridSystem::generateGrid(pathfinding, 64, 64);

    UnitFactory factory(registry, texture);

    Entity spearman1 = factory.createSpearman(sf::Vector2f(0,0));
    Entity spearman2 = factory.createSpearman(sf::Vector2f(64, 64));
    Entity spearman3 = factory.createSpearman(sf::Vector2f(128, 64));
    Entity spearman4 = factory.createSpearman(sf::Vector2f(128, 128));
   
    LOG_INFO("State Battle\t Init");
}

void BattleState::onDeactivate()
{
    LOG_INFO("State Battle\t Deactivate");
}

void BattleState::onActivate()
{
    LOG_INFO("State Battle\t Activate");
}

void BattleState::updateEvents()
{
    if (Input::isKeyPressed(sf::Keyboard::Key::Escape))
        state_machine.lastState();

    battle_map.updatePreview(common::mouse_pos_view);

    PathfindingInputSystem::handleInput(pathfinding);

    data.camera.scroll();
    data.camera.zoom();
}

void BattleState::updateImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::BeginMainMenuBar();

    if (ImGui::Button(SET_ICON(Icon::SETTINGS)))
        next_state = StateMachine::build<SettingsState>(data, state_machine, window, false);

    if (ImGui::Button(SET_ICON(Icon::ON_OFF)))
        state_machine.lastState();

    ImGui::EndMainMenuBar();
}

void BattleState::update(const float& dtime)
{
    updateMousePositions();
    registry.update(registry.getRegistry(), dtime, pathfinding, window);
    data.camera.update(dtime);
}

void BattleState::draw(sf::RenderTarget* target)
{
    beginView(target);

    battle_map.draw(*target, sf::RenderStates::Default);
    PathfindingRenderer::render(pathfinding, window);
    registry.draw(registry.getRegistry(), window);

    endView(target);
    ImGui::SFML::Render(window);
}


void BattleState::endView(sf::RenderTarget* target)
{
    target->setView(window.getDefaultView());
    target->setView(common::view);
}

void BattleState::beginView(sf::RenderTarget*& target)
{
    if (!target)
        target = &window;
    target->setView(common::view);
}