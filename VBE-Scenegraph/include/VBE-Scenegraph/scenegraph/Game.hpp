#ifndef GAME_HPP
#define GAME_HPP

#include <map>

#include <VBE-Scenegraph/scenegraph/GameObject.hpp>
#include <VBE-Scenegraph/scenegraph/ContainerObject.hpp>
#include <VBE/system/Window.hpp>

class Game : public ContainerObject {
	public:
        Game(const Window::DisplayMode& mode, const ContextSettings& settings, bool defaultClose = true);
		virtual ~Game();
		static Game* i() { return Game::instance;}

		GameObject* getObjectByName(std::string name) const;
		GameObject* getObjectByID(int id) const;
		float getTimeSinceFixed() const;
		float getFixedUpdateTime() const;

		virtual void run();

		void setFixedUpdateRate(int fixedUpdateRate);
		void setDynamicFramerate();

		bool isRunning = true;
	protected:
		virtual void update(float deltaTime) override;
		virtual void draw() const override;
	private:
        Window window;

		std::map<std::string, GameObject*> nameMap;
		std::map<int, GameObject*> idMap;
		int idCounter = 1;

		int fixedUpdateRate = 0;
		bool defaultClose = false;
		bool isFixedUpdateRate = false;
		float lastFixedUpdate = 0.0f;
		float timeSinceFixed = 0.0f;

		static Game* instance;

		friend class GameObject;
		friend class ContainerObject;
};

#endif //GAME_HPP
