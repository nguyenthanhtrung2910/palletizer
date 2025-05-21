#include <fstream>
#include <regex>
#include <string>
#include <random>
#include "AppWorldLogic.h"
#include "UnigineString.h"
#include "UnigineObjects.h"
#include "UniginePrimitives.h"
#include "UnigineEngine.h"

// World logic, it takes effect only when the world is loaded.
// These methods are called right after corresponding world script's (UnigineScript) methods.
using namespace Unigine;

AppWorldLogic::AppWorldLogic()
{}

AppWorldLogic::~AppWorldLogic()
{}

int AppWorldLogic::init()
{
	// Write here code to be called on world initialization: initialize resources for your world scene during the world start.
	std::ifstream file("../pallet_result.txt");
    if (!file) {
        std::cerr << "Error opening file.\n";
        return 1;
    }
    // regex pattern: matches (a, b, c) -> (x, y, z)
    std::regex pattern(R"(\((\d+), (\d+), (\d+)\) -> \((\d+), (\d+), (\d+)\))");

    std::string line;
    while (std::getline(file, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, pattern)) {
            float a{std::stof(matches[1])/SCALE};
            float b{std::stof(matches[2])/SCALE};
            float c{std::stof(matches[3])/SCALE};
            float x{std::stof(matches[4])/SCALE};
            float y{std::stof(matches[5])/SCALE};
            float z{std::stof(matches[6])/SCALE};
			Math::vec3 centerPoint{(a+x)/2 - 6.0f, (b+y)/2 - 4.0f, (c+z)/2 + 1.44f};
			Math::vec3 size{Math::abs(a-x), Math::abs(b-y), Math::abs(c-z)};
            m_boxesData.emplace_back(centerPoint, size);
        }
    }
    file.close();
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::update()
{
	// Write here code to be called before updating each render frame: specify all graphics-related functions you want to be called every frame while your application executes.
	if (m_frameCounter == BOX_SPAWN_FREQ)
	{
		ObjectMeshDynamicPtr box{Primitives::createBox(m_boxesData[m_boxCounter].second)};
		box->setTransform((Math::Mat4)Math::translate(m_boxesData[m_boxCounter].first));
		// random distribution
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 4);
		box->setMaterialFilePath(m_materialPaths[distr(gen)], "*");
		m_frameCounter = 0;
		m_boxCounter++;
	}
	else
	{
		m_frameCounter++;
	}
	return 1;
}

int AppWorldLogic::postUpdate()
{
	// The engine calls this function after updating each render frame: correct behavior after the state of the node has been updated.
	return 1;
}

int AppWorldLogic::updatePhysics()
{
	// Write here code to be called before updating each physics frame: control physics in your application and put non-rendering calculations.
	// The engine calls updatePhysics() with the fixed rate (60 times per second by default) regardless of the FPS value.
	// WARNING: do not create, delete or change transformations of nodes here, because rendering is already in progress.
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::shutdown()
{
	// Write here code to be called on world shutdown: delete resources that were created during world script execution to avoid memory leaks.
	return 1;
}

int AppWorldLogic::save(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is saving its state (i.e. state_save is called): save custom user data to a file.
	UNIGINE_UNUSED(stream);
	return 1;
}

int AppWorldLogic::restore(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is restoring its state (i.e. state_restore is called): restore custom user data to a file here.
	UNIGINE_UNUSED(stream);
	return 1;
}
