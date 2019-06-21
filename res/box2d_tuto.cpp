// shapes between 0.1 and 10 meters (eg 100px/m)
// angles in radians

// body <- fixture(s) <- shape

// timestep = 1/60s (not the same as frame rate, time step must not change too much)
// velocityiters = 6, positioniters = 2

// circles, polygones (+ SetAsBox), edges, chains
// functions to compute overlap, distance between two shapes...

// static body: infinite mass, does not move, can be moved manually by user
// kinematic body: same as static body but can move according to its velocity (but has infinite mass)
// static and kinematic cannot collide between themselves
// dynamic body: move according to forces, non-zero mass
// several fixtures can be attached on a body and won't move relatively to each other (rigid bodies)
// fixtures have collision geometry and density (no need to specify mass manually, although its possible)
// keep pointers to all created objects, don't forget to destroy (using b2 functions!)

// defining a dynamic body
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody; // first thing to do is set the type
bodyDef.position.Set(0.0f, 2.0f); // body's origin position
bodyDef.angle = 0.25f * b2_pi;
// 2 pts of interest: body's origin (fixtures are attached relatively to it)
// & center of mass (handled automatically)
// do not create a body at origin and then move it, instead
// initialize the position of the body in the body definition

// damping is different than friction because friction only occurs on contact
bodyDef.linearDamping = 0.0f; // usually want that to be 0 but might be useful for top view
bodyDef.angularDamping = 0.01f; // usually want that between 0 and 0.1
bodyDef.gravityScale = 0.0f; // gravity to zero so the body can float
// optimizations
bodyDef.allowSleep = true;
bodyDef.awake = false;
// if the body should not rotate, even under load
bodyDef.fixedRotation = true;
// if the body moves very fast (continuous collision detection)
bodyDef.bullet = true;
// whether the body participates in collisions
bodyDef.active = true;
// to store other data ie the sfml drawing stuff
bodyDef.userData = &anything; // void pointer

// body factory
b2Body* dynamicBody = myWorld->CreateBody(&bodyDef); 
// box2d does not keep a reference to the body definition or any of the
// data it holds (except user data pointers) so we can use temporary
// body definitions or reuse the same body definitions
// ... 
myWorld->DestroyBody(dynamicBody); // bodies are destroyed automatically when the world is
dynamicBody = NULL;

// body
body.GetPosition(); // .x, .y
body.GetAngle();

// shapes don't know about bodies
// fixtures attach shapes to bodies
// a body may have zero or more fixtures

// fixture creation
b2FixtureDef fixtureDef;
fixtureDef.shape = &myShape;
fixtureDef.density = 1.0f;
b2Fixture* myFixture = myBody->CreateFixture(&fixtureDef);
// no need to store this pointer, automatically destroyed whenn parent body is destroyed
// can create multiple fixtures on one body
// or destroy a fixture on the parent body to model a breakable object
myBody->DestroyFixture(myFixture);
myBody->GetFixtureList();

// density is used to compute mass properties of parent body
fixture->SetDensity(5.0f); // can be 0 or positive
body->ResetMassData();

// friction is used to make objects slide along each other
float32 friction; // friction is product of frictions of both objects
friction = sqrt(fixtureA->friction * fixtureB->friction);
// friction usually between 0 and 1 (if one object has friction 0, contact will have 0 friction)
// default friction can be overriden using b2Contact::SetFriction

// others:
// restitution: whether an object bounces on collision (eg tennis ball)
// filtering: when some group of objects must not collide with an other group
// sensors: when need to detect collision without having an actual collision (so just detect overlap)


// joints: used when need to constrain bodies to each other
// distance joint, revolute joint, prismatic joint, pulley joint,
// gear joint, mouse joint, wheel joint, weld joint, 
// rope joint, friction joint, motor joint

// contacts: objects created by box2D to manage collision
// between fixtures (can have events..)

// world class: contains the bodies and joints
b2World* myWorld = new b2World(gravity, canBodiesSleep);
// ...
delete myWorld;
// simulation
float32 timestep = 1.0f / 60.0f;
int32 velocityIterations = 10;
int32 positionIterations = 8;
myWorld->Step(timeStep, velocityIterations, positionIterations);
// after step, access positions, draw objects etc
// also clear any forces you have applied to objetcs
myWorld->ClearForces();

// access bodies, example
b2Body* node = myWorld->GetBodyList();
while (node) {
    b2Body* b = node;
    node = node->GetNext();

    GameActor* myActor = (GameActor*)b->GetUserData();
    if (myActor->IsDead()) {
        myWorld->DestroyBody(b);
    }
}

// AABB queries: determine all shapes in a region
// ray casts: do line-of-sight checks, fire guns...

// apply forces, torques and impulses to a body
// point is sthe point where the force or impulse is applied,
// usually the center of mass
void ApplyForce(const b2Vec& force, const b2Vec2& point);
void ApplyTorque(float32 torque);
void ApplyImpulse(const b2Vec& impulse, const b2Vec2& point);
void ApplyAngularImpulse(float32 impulse);

// functions GetWorldPoint/Vector, GetLocalPoint/Vector conversions

// box2D uses meters/kilograms/seconds
// use MKS in engine and just convert to pixels when rendering
// this way conversion from meters to pixels only happens in renderining

// http://www.iforce2d.net/b2dtut/top-down-car
// http://domasx2.github.io/gamejs-box2d-car-example/