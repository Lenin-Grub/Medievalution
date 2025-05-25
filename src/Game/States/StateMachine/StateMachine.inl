#pragma once

template <typename T>
std::unique_ptr<T> StateMachine::build(StateData& data, StateMachine& machine, sf::RenderWindow& window, bool replace)
{
    auto new_state = std::unique_ptr<T>{ nullptr };

    try
    {
        new_state = std::make_unique<T>(data, machine, window, replace);
    }
    catch (std::runtime_error& exception)
    {
        LOG_CRITICAL("Creation of new state was unsuccessful\t{0}", exception.what());
    }

    return new_state;
}