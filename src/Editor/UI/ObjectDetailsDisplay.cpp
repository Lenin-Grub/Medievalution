#include "ObjectDetailsDisplay.hpp"
#include <imgui.h>
// Полные названия направлений в порядке СЗ, С, СВ, В, ЮВ, Ю, ЮЗ, З (по часовой стрелке от Севера)
const char* CompassWidget::direction_names[8] = {
    "North-West", "North", "North-East", "East",
    "South-East", "South", "South-West", "West"
};

// Сокращенные названия направлений
const char* CompassWidget::direction_names_short[8] = {
    "NW", "N", "NE", "E", "SE", "S", "SW", "W"
};

CompassWidget::CompassWidget(float size, int num_directions)
    : size(size), num_directions(num_directions), current_direction(0.0f), is_dragging(false) {
    radius = size * 0.5f;
    inner_radius = radius * 0.7f;
}

bool CompassWidget::draw(const char* label) {
    ImGui::PushID(label);

    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size(size, size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Резервируем пространство под виджет
    ImGui::InvisibleButton("compass_canvas", canvas_size);
    bool value_changed = false;

    ImVec2 center(canvas_pos.x + radius, canvas_pos.y + radius);

    // Обработка ввода
    bool is_hovered = ImGui::IsItemHovered();

    if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        is_dragging = true;
    }

    if (is_dragging) {
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 local_pos = screenToCircle(mouse_pos, center);
            float angle = vectorToAngle(local_pos);
            current_direction = angle;
            value_changed = true;
        }
        else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            is_dragging = false;
            // При отпускании "прищелкиваем" к ближайшему направлению
            current_direction = snapToNearestDirection(current_direction);
            value_changed = true;
        }
    }

    // Рисуем компас
    drawCompass(draw_list, center);

    // Отображаем название текущего направления
    int dir_index = getDirectionIndex();
    ImVec2 text_size = ImGui::CalcTextSize(direction_names[dir_index]);
    ImVec2 text_pos(
        center.x - text_size.x * 0.5f,
        canvas_pos.y + size + 5.0f
    );
    draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), direction_names[dir_index]);

    ImGui::PopID();

    // Продвигаем курсор вниз, чтобы учесть текст
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + size + 25.0f);

    return value_changed;
}

void CompassWidget::drawCompass(ImDrawList* draw_list, const ImVec2& center) {
    // Рисуем внешний круг
    draw_list->AddCircleFilled(center, radius, IM_COL32(50, 50, 50, 255));
    draw_list->AddCircle(center, radius, IM_COL32(100, 100, 100, 255), 0, 2.0f);

    // Рисуем внутренний круг
    draw_list->AddCircleFilled(center, inner_radius, IM_COL32(30, 30, 30, 255));

    // Рисуем направления
    // Определяем фиксированные углы для 8 направлений в правильном порядке
    float angles[8] = { 315.0f, 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f };

    for (int i = 0; i < num_directions; ++i) {
        float angle = angles[i];
        // Корректируем угол для системы координат ImGui (Y вниз, 0° вправо)
        float corrected_angle = angle - 90.0f; // Смещаем, чтобы 0° был вверху
        float rad = corrected_angle * M_PI / 180.0f;

        ImVec2 dir(cosf(rad), sinf(rad));
        ImVec2 outer_point(
            center.x + dir.x * radius,
            center.y + dir.y * radius
        );
        ImVec2 inner_point(
            center.x + dir.x * inner_radius,
            center.y + dir.y * inner_radius
        );

        // Все направления рисуем одинаково
        ImU32 color = IM_COL32(200, 200, 200, 255);
        float thickness = 1.5f;

        draw_list->AddLine(inner_point, outer_point, color, thickness);

        // Подписи направлений
        ImVec2 text_center(
            center.x + dir.x * (radius + 15.0f),
            center.y + dir.y * (radius + 15.0f)
        );

        ImVec2 text_size = ImGui::CalcTextSize(direction_names_short[i]);
        ImVec2 text_pos(
            text_center.x - text_size.x * 0.5f,
            text_center.y - text_size.y * 0.5f
        );

        draw_list->AddText(text_pos, color, direction_names_short[i]);
    }

    // Рисуем стрелку (корректируем угол стрелки)
    float corrected_current_angle = current_direction - 90.0f;
    ImVec2 arrow_dir = angleToVector(corrected_current_angle);
    ImVec2 arrow_tip(
        center.x + arrow_dir.x * (inner_radius - 5.0f),
        center.y + arrow_dir.y * (inner_radius - 5.0f)
    );

    // Рисуем треугольник стрелки
    ImVec2 perp(-arrow_dir.y, arrow_dir.x); // Перпендикулярный вектор
    float head_size = 10.0f;

    ImVec2 p1(
        center.x + perp.x * head_size * 0.5f,
        center.y + perp.y * head_size * 0.5f
    );
    ImVec2 p2(
        center.x - perp.x * head_size * 0.5f,
        center.y - perp.y * head_size * 0.5f
    );
    ImVec2 p3(
        arrow_tip.x,
        arrow_tip.y
    );

    // Тень стрелки
    ImVec2 shadow_offset(1.0f, 1.0f);
    draw_list->AddTriangleFilled(
        ImVec2(p1.x + shadow_offset.x, p1.y + shadow_offset.y),
        ImVec2(p2.x + shadow_offset.x, p2.y + shadow_offset.y),
        ImVec2(p3.x + shadow_offset.x, p3.y + shadow_offset.y),
        IM_COL32(0, 0, 0, 100)
    );

    // Основная стрелка
    draw_list->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 50, 50, 255));
    draw_list->AddTriangle(p1, p2, p3, IM_COL32(200, 0, 0, 255), 1.5f);

    // Центральный круг
    draw_list->AddCircleFilled(center, 5.0f, IM_COL32(100, 100, 100, 255));
    draw_list->AddCircle(center, 5.0f, IM_COL32(200, 200, 200, 255), 0, 1.5f);
}

ImVec2 CompassWidget::screenToCircle(const ImVec2& screen_pos, const ImVec2& center) const {
    return ImVec2(screen_pos.x - center.x, screen_pos.y - center.y);
}

float CompassWidget::vectorToAngle(const ImVec2& vec) const {
    // В ImGui система координат такая, что Y направлен вниз.
    // Чтобы 0° был вверху (Север), нужно скорректировать вычисление угла.
    float angle = atan2f(vec.y, vec.x) * 180.0f / M_PI;
    // Корректируем: прибавляем 90° и нормализуем
    angle += 90.0f;
    if (angle < 0) angle += 360.0f;
    if (angle >= 360.0f) angle -= 360.0f;
    return angle;
}

ImVec2 CompassWidget::angleToVector(float angle) const {
    // Корректируем угол для преобразования в вектор
    // В стандартной системе 0° направо, нам нужно 0° вверх
    float corrected_angle = angle - 90.0f;
    float rad = corrected_angle * M_PI / 180.0f;
    return ImVec2(cosf(rad), sinf(rad));
}

float CompassWidget::snapToNearestDirection(float angle) const {
    // Фиксированные углы для 8 направлений
    float angles[8] = { 315.0f, 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f };

    float min_diff = 360.0f;
    float closest_angle = 0.0f;

    for (int i = 0; i < 8; ++i) {
        float diff = fabs(angle - angles[i]);
        // Учитываем переход через 360°
        if (diff > 180.0f) diff = 360.0f - diff;

        if (diff < min_diff) {
            min_diff = diff;
            closest_angle = angles[i];
        }
    }

    return closest_angle;
}

int CompassWidget::getDirectionIndex() const {
    // Фиксированные углы для 8 направлений
    float angles[8] = { 315.0f, 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f };

    float min_diff = 360.0f;
    int closest_index = 0;

    for (int i = 0; i < 8; ++i) {
        float diff = fabs(current_direction - angles[i]);
        // Учитываем переход через 360°
        if (diff > 180.0f) diff = 360.0f - diff;

        if (diff < min_diff) {
            min_diff = diff;
            closest_index = i;
        }
    }

    return closest_index;
}

void CompassWidget::setDirection(float direction) {
    current_direction = fmod(direction, 360.0f);
    if (current_direction < 0) current_direction += 360.0f;
}

void CompassWidget::setDirectionIndex(int index) {
    if (index >= 0 && index < num_directions) {
        // Фиксированные углы для 8 направлений
        float angles[8] = { 315.0f, 0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f };
        current_direction = angles[index];
    }
}

void ObjectDetailsDisplay::draw()
{
    static float windowWidth;
    static float windowHeight;

    ImGui::Begin("Object Details");
    //ImGui::SeparatorText("Default Window Parameters");

    //ImGui::Dummy(ImVec2(0.0f, 2 * ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight()));
    //ImGui::Text("Window Size");
    //ImGui::SameLine();

    //ImGui::PushItemWidth(128.f);

    //ImGui::PushStyleColor(ImGuiCol_Button, sf::Color::Red);
    //ImGui::Button("X"); ImGui::SameLine();
    //ImGui::PopStyleColor();
    //ImGui::SameLine();

    //ImGui::InputInt("##windowWidth", &windowWidth);
    //ImGui::SameLine();

    //ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    //ImGui::Button("Y"); ImGui::SameLine();
    //ImGui::PopStyleColor();
    //ImGui::SameLine();

    //ImGui::InputInt("##windowHeight", &windowHeight);
    //ImGui::PopItemWidth();

    //ImGui::Separator();
    //ImGui::Dummy(ImVec2(0.0f, 2 * ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight()));
    //ImGui::Text("Window Flags");
    //ImGui::Separator();

    //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    //ImGui::Button("X");
    //ImGui::PopStyleColor();
    //ImGui::SameLine();
    //ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    //ImGui::InputFloat("##pos_x", &windowWidth, 1.0f, 1.0f, "%.3f");
    //ImGui::PopItemWidth();
    //ImGui::SameLine();

    //ImGui::PushStyleColor(ImGuiCol_Button, sf::Color(40, 159, 49));
    //ImGui::Button("Y");
    //ImGui::PopStyleColor();
    //ImGui::SameLine();
    //ImGui::PushItemWidth(ImGui::CalcItemWidth() * 0.5f - ImGui::GetStyle().ItemSpacing.x * 1.5f);
    //ImGui::InputFloat("##pos_y", &windowHeight, 1.0f, 1.0f, "%.3f");
    //ImGui::PopItemWidth();
    //ImGui::SameLine();
    //ImGui::Text("Position");

    compas.draw("Directions");
 
    ImGui::End();

}

void ObjectDetailsDisplay::update(const float& delta_time)
{
}
