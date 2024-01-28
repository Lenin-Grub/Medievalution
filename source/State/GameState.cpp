#include "../stdafx.h"
#include "GameState.h"

GameState::GameState(StateData* state_data)
	:State(state_data),
	camera(),
	world_map()
{
	texture.loadFromFile("resources/SptitreAtlas/Archer/Shot_1.png");
	sprite.setTexture(texture);
	
	animator = std::make_unique<Animator>(sprite);
	int a = 0;
	animator->addFrame(sf::IntRect(a,0,128,128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->addFrame(sf::IntRect(a +=128, 0, 128, 128));
	animator->setFrameTime(0.5f);
	animator->pause();
}

GameState::~GameState() 
{ 
}

void GameState::updateEvents()
{
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		camera.zoom();
		camera.scroll();
	}
}

//to do@ метод будет пухнуть, вынести куда-нибудь отдельно
void GameState::updateImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::ShowDemoWindow();
	ImGui::Begin("GameMenu##", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	if (ImGui::Button(u8"Выход##toMainMenu"))
	{
		endState();
	}
	ImGui::End();

	std::string str = world_map.getProvinceName();
	char* chr = const_cast<char*>(str.c_str());

	ImGui::SetNextWindowBgAlpha(0.55f);
	ImGui::Begin("T", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

	ImGui::TextColored(ImVec4(1, 1, 1, 0.5),u8"Это карта мира\n" "Используй WSAD для навигации\n" "Используй колесико для масштабирования\n" "Это окошко можно передвинуть ЛКМ");
	ImGui::Separator();
	
	ImGui::TextColored(ImVec4(1,1,0,1),"Metrics: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	if (ImGui::IsMousePosValid())
		ImGui::Text("Mouse Window Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
	else
		ImGui::Text("Mouse Position: <invalid>");
	
	ImGui::Text("Mouse View Position: (%.1f,%.1f)",core::mousePosView.x, core::mousePosView.y);
	ImGui::Text(Localisation::getInstance().getStringByKey("T_Province_name").c_str(), chr);
	ImGui::Text(Localisation::getInstance().getStringByKey("T_Province_id").c_str(), world_map.getProvinceID());
	
	ImGui::Separator();
	ImGui::SliderFloat(Localisation::getInstance().getStringByKey("T_transperecny").c_str(), &world_map.transperency, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("Animation", nullptr);
	ImGui::Image(sprite,sf::Vector2f(256,256));
	ImGui::Separator();

	{
		auto ft = animator->getFrameTime();
		auto cf = animator->getCurrentFrame();
		auto pl = animator->isPlayed();

		ImGui::SliderFloat("speed", &ft, 0.0f, 1.0f);
		ImGui::SliderInt("frame", &cf, 0, animator->getFrames().size() - 1);
		ImGui::Checkbox("play", &pl);

		animator->setFrameTime(ft);
		animator->setCurrentFrame(cf);
		animator->play(pl);
	}

	ImGui::End();
}

void GameState::update(const float& dtime)
{
	updateMousePositions();
	camera.update(dtime);

	animator->update(0.1f);

	/*
		todo @ при высоком фпс лагает
		скорее всего за 1 кадр делает поиск по всем 10000+ провинций
		и каждый кадр сверяет текущий цвет с текстурой
		нужно добавить проверку
		если провинция найдена и позиция мыши не изменилась, искать не нужно
	*/
	sf::Color& color = world_map.getColor();

	world_map.select_color = color;
	world_map.shader.setParameter("select_color", world_map.select_color);
	world_map.shader.setParameter("transperency", world_map.transperency);
}

void GameState::render(sf::RenderTarget* target)							
{
	if (!target)
		target = this->window.get();
	target->setView(core::view);

//	рисуем динамечские объекты
	world_map.draw(*target,sf::RenderStates::Default);

//	рисуем статические объекты после установки дефолтной камеры (gui и тд)
	target->setView(this->window->getDefaultView());

//	возвращаем камеру вида игры
	target->setView(core::view);
}

void GameState::updateObserver(){}