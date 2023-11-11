#include "Game.h"
#include "LWE/IO/FontAtlas.h"

#include <LWE/GameLoop.h>
#include <LWE/IO/RenderWindow.h>
#include <LWE/Systems/RigidBodySystem.h>
#include <LWE/Systems/Portrait2DSystem.h>
#include <LWE/Systems/GuiSystem.h>
#include <LWE/Components/AABB2D.h>
#include <LWE/Components/Circle.h>
#include <LWE/Components/Portrait2D.h>
#include <LWE/Gui/LabelGui.h>
#include <LWE/IO/HID/Input.h>
#include <LWE/IO/HID/GenericInputHandler.h>
#include <LWE/IO/HID/Keyboard.h>

Game::~Game()
{
}

Game::Game()
	: Level("Game")
{
}

Game::Game(const Game& lvl)
	: Level(lvl)
{
}

Game& Game::operator=(const Game& lvl)
{
	if (this == &lvl)
		return *this;

	Level::operator=(lvl);

	return *this;
}

void Game::SetupResources()
{
	Level::SetupResources();

	// Setup Resource Code Here
	AddResource(new lwe::Mesh("PortraitGui", lwe::portraitGuiVerts, lwe::portraitGuiIndices));

	lwe::UInt_8 circleData[] = {255, 255, 255, 255};
	lwe::Img* circle = new lwe::Img("Circle", 8, 4, 1, 1, (lwe::Byte*)circleData, lwe::ImgAspect::IMG_ASPECT_COLOR);
	AddResource(circle);
}

void Game::Setup()
{
	Level::Setup();

	// Setup Code Here

	lwe::GameLoop* gl = (lwe::GameLoop*)GetParent("GameLoop");

	lwe::RenderWindow* win = gl->GetWindow();

	// Adding Systems Here
	AddSystem(new lwe::Portrait2DSystem());
	AddSystem(new lwe::RigidBodySystem());

	lwe::GuiSystem* gui = new lwe::GuiSystem();
	gui->AddResource(new lwe::FontAtlas("Resources/Fonts/Hack-Regular_48.ehf"));
	AddSystem(gui);

	lwe::Vec2_f scale = win->GetSwapChain()->GetScale();
	lwe::Vec2_f center = win->GetSwapChain()->GetScale() / 2;

	// Score Gui
	lwe::LabelGui* plyScore = new lwe::LabelGui("PlyScore", "Hack-Regular_48", "0");
	plyScore->SetPosition({20.0f, 20.0f});
	gui->AddGui(plyScore);

	lwe::LabelGui* aiScore = new lwe::LabelGui("AiScore", "Hack-Regular_48", "0");
	aiScore->SetPosition({scale.x - 68.0f, 20.0f});
	gui->AddGui(aiScore);

	// Bounds Entity
	lwe::Entity* bounds = new lwe::Entity("Bounds");
	bounds->SetScale({win->GetSwapChain()->GetScale(), 1.0f});

	lwe::AABB2D* collider = new lwe::AABB2D();
	bounds->AddComponent(collider);

	AddEntity(bounds);

	// Field Separator Entity

	lwe::Entity* fieldSep = new lwe::Entity("FieldSeparator");
	fieldSep->SetScale({10.0f, scale.y, 1.0f});
	fieldSep->SetPos({center.x - 5.0f, 0.0f, 0.0f});

	lwe::Portrait2D* fieldSepImg = new lwe::Portrait2D("PaddleImg", "Circle");
	fieldSepImg->SetColor({0.75f});
	fieldSep->AddComponent(fieldSepImg);

	AddEntity(fieldSep);

	// Paddle Entity Template
	lwe::Entity* paddle = new lwe::Entity("Paddle");
	paddle->SetScale({20.0f, 200.0f, 1.0f});

	lwe::Portrait2D* paddleImg = new lwe::Portrait2D("PaddleImg", "Circle");
	paddleImg->SetColor({0.75f});
	paddle->AddComponent(paddleImg);

	AddTemplate(paddle);

	// Player's Paddle
	lwe::Entity* plyPaddle = CreateEntity("Paddle", "PlyPaddle");
	plyPaddle->SetPos({50.0f, center.y - 100.0f, 0.0f});

	// Player's Paddle
	lwe::Entity* aiPaddle = CreateEntity("Paddle", "AiPaddle");
	aiPaddle->SetPos({scale.x - 50.0f, center.y - 100.0f, 0.0f});

	// Ball Entity
	lwe::Entity* ball = new lwe::Entity("Ball");
	ball->SetScale({20.0f, 20.0f, 1.0f});
	ball->SetPos({center.x - 10.0f, center.y - 10.0f, 0.0f});

	lwe::Circle* ballCollider = new lwe::Circle("BallCollider", 50.0f);
	ball->AddComponent(ballCollider);

	lwe::Portrait2D* ballImg = new lwe::Portrait2D("BallImg", "Circle");
	ballImg->SetColor({1.0f, 0.0f, 0.0f});
	ball->AddComponent(ballImg);

	AddEntity(ball);
}

void Game::PostInitialize(lwe::GpuCmdBuffer* cmdBuffer)
{
	Level::PostInitialize(cmdBuffer);

	// Post Initialization Code Here
}

void Game::OnUpdate(lwe::Input* input, const float delta)
{
	Level::OnUpdate(input, delta);

	// Update Code Here

	lwe::GenericInputHandler* handler = (lwe::GenericInputHandler*)input->GetHandler("GenericInputHandler");

	const lwe::Keyboard* keyboard = handler->GetPrimaryKeyboard();

	lwe::Entity* plyPaddle = GetEntity("PlyPaddle");

	if (keyboard->IsDown(lwe::Keyboard::W))
		plyPaddle->SetPos(plyPaddle->GetPos() - lwe::Vec3_f(0.0f, 400.0f, 0.0f) * delta);

	if (keyboard->IsDown(lwe::Keyboard::S))
		plyPaddle->SetPos(plyPaddle->GetPos() + lwe::Vec3_f(0.0f, 400.0f, 0.0f) * delta);
}

void Game::PreRender(lwe::GpuCmdBuffer* cmdBuffer)
{
	Level::PreRender(cmdBuffer);

	// Pre-Render Code Here
}