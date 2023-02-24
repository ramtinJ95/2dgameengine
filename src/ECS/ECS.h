#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <memory>
#include <utility>
#include <vector>
#include "set"
#include "typeindex"
#include "unordered_map"
const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;


// I stands for interface, instead of having somethin like BaseComponet etc,
// we dont want too much OOP ideas creeping into the code.
struct IComponent {
    protected:
	static int nextId;
};
// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent{
    static int GetId() {
	// Return the unique id of Component<T>
	static auto id = nextId++;
	return id;
    }
};

class Entity {
    private:
	int id;
    public:
	Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
	int GetId() const;

	Entity& operator =(const Entity& other) = default;
	bool operator ==(const Entity& other) const {return id == other.id;}
	bool operator !=(const Entity& other) const {return id != other.id;}
	bool operator >(const Entity& other) const {return id > other.id;}
	bool operator <(const Entity& other) const {return id < other.id;}
};

class System {
    private:
	Signature componentSignature;
	std::vector<Entity> entities;
    public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntites() const;
	const Signature& GetComponentSignature() const;

	template <typename TComponent> void RequireComponent();

};


// This IPool class is just here because I want to be able to define a private vector
// componentPools in my registry class without saying what the type of Pool is. So 
// by making the destructor virtual and then having that in the Pool template class 
// definiton I have essentially made IPool into an abstract class that just serves as
// a placeholder type until compile time where the compiler will use the Template class Pool
// to generate all the types of Pools that we are going to want/need.

class IPool {

    public:
	virtual ~IPool() {}
};


template<typename T>
class Pool: IPool {
    private:
	std::vector<T> data;

    public:
	Pool(int size = 100) {
	    data.resize(size);
	}
	virtual	~Pool() = default;
	
	bool isEmpty() const {
	    return data.empty();
	}
	int GetSize() const {
	    return data.size();
	}
	void Resize(int n) {
	    return data.resize(n);
	}
	void Clear() {
	    return data.clear();
	}
	void Add(T object) {
	    return data.push_back(object);
	}
	void Set(int index, T object) {
	    data[index] = object;
	}
	T& Get(int index) {
	    return static_cast<T&>(data[index]);
	}
	T& operator [](unsigned int index) {
	    return data[index];
	}
};

class Registry {
    private:
	int numEntitets = 0;
	
	std::vector<IPool*> componentPools;
	std::vector<Signature> entityComponentSignatures;
	std::unordered_map<std::type_index, System*> systems;
    // we need the 2 sets below because we dont want to add entites in the middle of a
    // update tick or render tick since that would mess up the rendering and the different
    // systems that we have such as collision detection etc.
    // These two lists will be updated at the end of the game loop tick, i.e
    // we will only add and remove entites at the end of the game loop for 1 frame. 
	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitesToBeKilled;
    public:
	Registry() = default;

	Entity CreateEntity();
	void KillEntity(Entity entity);
	
	template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template<typename TComponent> void RemoveComponent(Entity entity);
	template<typename Tcomponent> bool HasComponent(Entity entity);
        
        template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template<typename TSystem> void RemoveSystem();
        template<typename TSystem> bool HasSystem() const;
        template<typename TSystem> TSystem& GetSystem() const;

        void AddEntityToSystems(Entity entity);
	void Update();	
};

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
};

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template<typename TComponent, typename ...TArgs> 
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    if (componentId >= componentPools.size()) {
	// reason for 5 is we dont want to allocate too much extra memory
	// but we also dont want to resize the vector too much since that is
	// expensive. 
	
	componentPools.resize(componentId + 5, nullptr);
    }

    if (!componentPools[componentId]) {
	Pool<TComponent>* newComponentPool = new Pool<TComponent>();
	componentPools[componentId] = newComponentPool;
    }

    Pool<TComponent>* componentPool = componentPools[componentId];

    if(entityId >= componentPool->GetSize()) {
	componentPool->Resize(numEntitets);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);

    entityComponentSignatures[entityId].set(componentId);

}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}

#endif
