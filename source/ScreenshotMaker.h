#pragma once

#include "UnigineComponentSystem.h"
#include "UnigineViewport.h"

class ScreenshotMaker : public Unigine::ComponentBase
{
	enum Format
	{
		tga = 0,
		png,
		jpg
	};

	enum State
	{
		STATE_START = 0,
		STATE_WARMUP,
		STATE_SAVE,
		STATE_DONE,
	};

public:
	COMPONENT_DEFINE(ScreenshotMaker, Unigine::ComponentBase);

	PROP_PARAM(String, name_prefix, "screenshot");
	PROP_PARAM(IVec2, size, Unigine::Math::ivec2(640, 480));
	PROP_PARAM(Switch, format, 0, "tga,png,jpg");
	PROP_PARAM(Toggle, alpha_channel, false);

	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

private:
	Unigine::TexturePtr texture;
	Unigine::ViewportPtr viewport;
	int count = 0;

	int state = STATE_DONE;
	const int warmup_frames = 30;
	int warm_up_count = 0;

	void init();
	void update();

};