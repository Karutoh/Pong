#pragma once

#include <LWE/LWE.h>
#include <LWE/Level.h>

class Game : public lwe::Level
{
public:
	~Game() override;

	Game();

	Game(const Game& lvl);

	Game& operator=(const Game& lvl);

	void SetupResources() override;

	void Setup() override;

	void PostInitialize(lwe::GpuCmdBuffer* cmdBuffer) override;

	void OnUpdate(lwe::Input* input, const float delta) override;

	void PreRender(lwe::GpuCmdBuffer* cmdBuffer) override;
};