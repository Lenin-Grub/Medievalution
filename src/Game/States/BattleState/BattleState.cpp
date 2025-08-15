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

    int unitCount = 0;
    for (int row = 0; row < 4 && unitCount < 5; ++row) 
    {
        for (int col = 0; col < 5 && unitCount < 20; ++col) 
        {
            Entity spearman = factory.createSpearman(sf::Vector2f(col /** 32*/, row /** 16*/));
            unitCount++;
        }
    }


    uint32_t squad_id = 1;
    for (int i = 0; i < 10; ++i) 
    {
        sf::Vector2f start_pos(-320 + i * 32, 320);

        Entity spearman = factory.createSpearman(start_pos);
        entt::entity entity = spearman.getEntity();

        auto& squad_comp = registry.getRegistry().emplace_or_replace<Components::Formation>(entity);
        squad_comp.squad_id = squad_id;
        squad_comp.formation_index = i;
        squad_comp.is_leader = (i == 0);
        squad_comp.formation_type = Components::FormationType::Circle;
    }

    uint32_t squad2_id = 2;

    for (int i = 0; i < 10; ++i) 
    {
        sf::Vector2f start_pos(320 + i * 32, 640);

        Entity spearman = factory.createSpearman(start_pos);
        entt::entity entity = spearman.getEntity();

        auto& squad_comp = registry.getRegistry().emplace_or_replace<Components::Formation>(entity);
        squad_comp.squad_id = squad2_id;
        squad_comp.formation_index = i;
        squad_comp.is_leader = (i == 0);
        squad_comp.formation_type = Components::FormationType::Line;
    }
   
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

    renderUI();

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

std::vector<uint32_t> BattleState::getSelectedSquads()
{
    std::vector<uint32_t> squads;
    std::unordered_set<uint32_t> unique_squads;

    auto selectable_view = registry.getRegistry().view<Components::Selectable, Components::Formation>();
    for (auto entity : selectable_view)
    {
        auto& selectable = selectable_view.get<Components::Selectable>(entity);
        if (selectable.is_selected)
        {
            auto& squad = selectable_view.get<Components::Formation>(entity);
            if (squad.squad_id != 0)
            {
                unique_squads.insert(squad.squad_id);
            }
        }
    }

    squads.assign(unique_squads.begin(), unique_squads.end());
    return squads;
}

void BattleState::changeSquadFormation(uint32_t squad_id, Components::FormationType new_type)
{
    auto squad_view = registry.getRegistry().view<Components::Formation>();
    for (auto entity : squad_view)
    {
        auto& squad = registry.getRegistry().get<Components::Formation>(entity);
        if (squad.squad_id == squad_id)
        {
            squad.formation_type = new_type;
        }
    }
}

void BattleState::renderUI()
{
    if (!show_imgui) return;

    ImGui::Begin("Battle Control", &show_imgui);

    selected_squads = getSelectedSquads();

    if (selected_squads.empty())
    {
        ImGui::Text("No squads selected");
    }
    else
    {
        ImGui::Text("Selected squads: %zu", selected_squads.size());

        const char* formation_names[] = { "Line", "Square", "Circle"/*, "Diamond", "Triangle" */};

        for (size_t i = 0; i < selected_squads.size(); ++i)
        {
            uint32_t squad_id = selected_squads[i];
            ImGui::PushID(squad_id);

            ImGui::Text("Squad ID: %u", squad_id);

            Components::FormationType current_type = Components::FormationType::Line;
            auto squad_view = registry.getRegistry().view<Components::Formation>();
            for (auto entity : squad_view)
            {
                auto& squad = registry.getRegistry().get<Components::Formation>(entity);
                if (squad.squad_id == squad_id)
                {
                    current_type = squad.formation_type;
                    break;
                }
            }

            int current_selection = static_cast<int>(current_type);

            if (ImGui::Combo(("Formation##" + std::to_string(squad_id)).c_str(),
                &current_selection, formation_names, IM_ARRAYSIZE(formation_names)))
            {
                Components::FormationType new_type = static_cast<Components::FormationType>(current_selection);
                changeSquadFormation(squad_id, new_type);
            }

            ImGui::PopID();
        }
    }

    ImGui::Separator();

    ImGui::Text("Squads Info:");
    auto squad_view = registry.getRegistry().view<Components::Formation>();
    std::unordered_set<uint32_t> squad_ids;
    for (auto entity : squad_view)
    {
        auto& squad = registry.getRegistry().get<Components::Formation>(entity);
        if (squad.squad_id != 0)
        {
            squad_ids.insert(squad.squad_id);
        }
    }

    const char* formation_names[] = { "Line", "Square", "Circle", "Diamond", "Triangle" };

    for (uint32_t squad_id : squad_ids)
    {
        int unit_count = 0;
        bool is_selected = false;
        Components::FormationType formation_type = Components::FormationType::Line;

        for (auto entity : squad_view)
        {
            auto& squad = registry.getRegistry().get<Components::Formation>(entity);
            if (squad.squad_id == squad_id)
            {
                unit_count++;
                if (squad.is_leader)
                {
                    formation_type = squad.formation_type;
                }
                if (registry.getRegistry().all_of<Components::Selectable>(entity))
                {
                    auto& selectable = registry.getRegistry().get<Components::Selectable>(entity);
                    if (selectable.is_selected)
                    {
                        is_selected = true;
                    }
                }
            }
        }

        ImGui::Text("ID %u: %d units [%s] %s",
            squad_id, unit_count,
            formation_names[static_cast<int>(formation_type)],
            is_selected ? "(selected)" : "");
    }

    ImGui::End();
}