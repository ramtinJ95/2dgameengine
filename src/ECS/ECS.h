#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
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

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

class Registry {
};

#endif
