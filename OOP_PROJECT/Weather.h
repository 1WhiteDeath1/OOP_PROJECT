#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// base class for every kind of weather. the particles (rain drops, snow flakes) live in
// screen coordinates, so they always cover the screen wherever the camera is
class Weather
{
protected:
	static const int COUNT = 400;   // number of particles
	float px[COUNT], py[COUNT];     // particle positions on the screen

	void scatter();                 // place all particles at random spots on the screen
public:
	virtual ~Weather() = default;
	virtual void update(float dt) = 0;
	virtual void render(RenderWindow& window) = 0;
	virtual const char* getName() const = 0;
	// how grey the sky gets: 0 = clear sky, 1 = completely grey
	virtual float getCloudiness() const { return 0; }
};


class ClearWeather : public Weather {
public:
	void update(float dt) override {}
	void render(RenderWindow& window) override {}
	const char* getName() const override { return "Clear"; }
};


class Rain : public Weather {
public:
	Rain() { scatter(); }
	void update(float dt) override;
	void render(RenderWindow& window) override;
	const char* getName() const override { return "Rain"; }
	float getCloudiness() const override { return 0.7f; }
};


class Snow : public Weather {
	float time = 0; // used to sway the flakes from side to side
public:
	Snow() { scatter(); }
	void update(float dt) override;
	void render(RenderWindow& window) override;
	const char* getName() const override { return "Snow"; }
	float getCloudiness() const override { return 0.5f; }
};


// changes the weather on a fixed, hardcoded schedule
class WeatherSystem
{
	Weather* current = nullptr;
	int step = 0;           // where we are in the schedule
	float timeLeft = 0;     // seconds until the next change

	void startStep();       // create the weather for the current step
public:
	WeatherSystem() { startStep(); }
	~WeatherSystem() { delete current; }
	WeatherSystem(const WeatherSystem&) = delete;            // owns a pointer, so no copies
	WeatherSystem& operator=(const WeatherSystem&) = delete;

	void update(float dt);
	void render(RenderWindow& window) { current->render(window); }
	const Weather& getWeather() const { return *current; }
};
