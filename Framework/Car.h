
#include "Entity.h"

#include "Box2D.h"

// Forward Declarations
class BackBuffer;
class Sprite;

class Car : public Entity
{
	// Member Data
public:
	Car();
	~Car();

	bool Initialise(Sprite* sprite, b2World* gameWorld);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void StopMove();
protected:
private:
	void box2DBodyCreate(b2World* gameWorld, float width, float height);

	// Memeber Variables
public:
protected:
private:

	b2Body* mp_body;
};