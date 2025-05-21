#include "ScreenshotMaker.h"
#include "UnigineConsole.h"
#include "UnigineInput.h"
#include "UnigineGame.h"

REGISTER_COMPONENT(ScreenshotMaker);

using namespace Unigine;

void ScreenshotMaker::init()
{
	texture = Texture::create();
	viewport = Viewport::create();
	viewport->setSkipFlags(Viewport::SKIP_VISUALIZER);

	Console::setOnscreen(1);
	Console::onscreenMessageLine(Math::vec4_green, "Screenshot component is initialized.");
}

void ScreenshotMaker::update()
{
	auto player = Game::getPlayer();
	if (!player)
	{
		state = STATE_DONE;
		Console::onscreenMessageLine(Math::vec4_red, "No active camera.");
		return;
	}

	if (state == STATE_DONE)
	{
		warm_up_count = 0;
		state = STATE_WARMUP;
		Console::onscreenMessageLine(Math::vec4_green, "Trying to take a screenshot...");
	}

	if (state == STATE_WARMUP)
	{
		viewport->setMode(Render::getViewportMode());

		if (warm_up_count == 0)
		{
			// First frame we render with velocity buffer turned off to avoid temporal effects artifacts
			viewport->appendSkipFlags(Viewport::SKIP_VELOCITY_BUFFER);
			viewport->renderTexture2D(player->getCamera(), texture, size.get().x, size.get().y);
			viewport->removeSkipFlags(Viewport::SKIP_VELOCITY_BUFFER);
		}
		else
		{
			// We temporarily set exposure adaptation time to 0, otherwise the final image may be too dark
			float exposure_adaptation = Render::getExposureAdaptation();
			Render::setExposureAdaptation(0.0f);
			viewport->renderTexture2D(player->getCamera(), texture, size.get().x, size.get().y);
			Render::setExposureAdaptation(exposure_adaptation);
		}

		warm_up_count++;
		if (warm_up_count == warmup_frames)
			state = STATE_SAVE;
	}

	if (state == STATE_SAVE)
	{
		Render::asyncTransferTextureToImage(
			nullptr,
			MakeCallback([this](ImagePtr image)
				{
					if (!alpha_channel.get() || format.get() == Format::jpg)
					{
						if (image->getFormat() == Image::FORMAT_RGBA8)
							image->convertToFormat(Image::FORMAT_RGB8);
						else if (image->getFormat() == Image::FORMAT_RGBA16F)
							image->convertToFormat(Image::FORMAT_RGB16F);
					}

					if (!Render::isFlipped())
						image->flipY();

					auto str_formats = String::split("tga,png,jpg", ",");

					String fullName = String::format("%s_%d.%s", name_prefix.get(), count, str_formats[format.get()]);
					image->save(("../frames/"+fullName).get());
					Console::onscreenMessageLine(Math::vec4_green, "%s saved.", fullName.get());

				}),
			texture);

		count++;
		state = STATE_DONE;
	}
}