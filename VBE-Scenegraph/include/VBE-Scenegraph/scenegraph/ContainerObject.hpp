#ifndef CONTAINEROBJECT_HPP
#define CONTAINEROBJECT_HPP

#include <queue>
#include <set>

#include <VBE-Scenegraph/scenegraph/GameObject.hpp>

class ContainerObject : public GameObject {
	public:
		ContainerObject();
		virtual ~ContainerObject();
	protected:
		virtual void update(float deltaTime) override;
		virtual void fixedUpdate(float deltaTime) override;
		virtual void draw() const override;
	private:
		void addToContainer(GameObject* obj) override final;
		void removeFromContainer(GameObject* obj) override final;
		void manageObjects();

		struct FunctorCompareDraw{
				bool operator()(const GameObject* a, const GameObject* b) {
					if(a->drawPriority == b->drawPriority)
						return (a->id < b->id);
					return (a->drawPriority < b->drawPriority);
				}
		};
		struct FunctorCompareUpdate{
				bool operator()(const GameObject* a, const GameObject* b) {
					if(a->updatePriority == b->updatePriority)
						return (a->id < b->id);
					return (a->updatePriority < b->updatePriority);
				}
		};

		std::set<GameObject*, FunctorCompareDraw> drawTasks;
		std::set<GameObject*, FunctorCompareUpdate> updateTasks;

		std::queue<GameObject*> objectTasksToAdd;
		std::queue<GameObject*> objectTasksToRemove;

		friend class GameObject;
		friend class Game;
};

#endif // CONTAINEROBJECT_HPP
