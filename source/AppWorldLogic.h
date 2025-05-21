#ifndef __APP_WORLD_LOGIC_H__
#define __APP_WORLD_LOGIC_H__

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <UnigineLogic.h>
#include <UnigineStreams.h>

inline constexpr float SCALE{200.0f};
inline constexpr int BOX_SPAWN_FREQ{100};

class AppWorldLogic : public Unigine::WorldLogic
{

public:
	AppWorldLogic();
	~AppWorldLogic() override;

	int init() override;

	int update() override;
	int postUpdate() override;
	int updatePhysics() override;

	int shutdown() override;

	int save(const Unigine::StreamPtr &stream) override;
	int restore(const Unigine::StreamPtr &stream) override;
private:
	// vector to store boxes data
    std::vector<std::pair<Unigine::Math::vec3, Unigine::Math::vec3>> m_boxesData; 
	std::vector<const char*> m_materialPaths{
		"woodfloor1/woodfloor1.mat",
		"woodfloor2/woodfloor2.mat",
		"woodfloor3/woodfloor3.mat",
		"woodfloor4/woodfloor4.mat",
		"woodfloor5/woodfloor5.mat",
	};
	int m_frameCounter{0};
	int m_boxCounter{0};
};

#endif // __APP_WORLD_LOGIC_H__
