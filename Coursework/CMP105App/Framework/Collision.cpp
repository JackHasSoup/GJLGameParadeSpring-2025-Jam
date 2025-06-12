#include "Collision.h"

// check AABB for collision. Returns true if collision occurs.
bool Collision::checkBoundingBox(GameObject *s1, GameObject *s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2->getCollisionBox().left)
		return false;
	if (s1->getCollisionBox().left > s2->getCollisionBox().left + s2->getCollisionBox().width)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2->getCollisionBox().top)
		return false;
	if (s1->getCollisionBox().top > s2->getCollisionBox().top + s2->getCollisionBox().height)
		return false;

	return true;
}

// check for bounding box collision with a point/vector2 rather than two boxes.
bool Collision::checkBoundingBox(GameObject *s1, sf::Vector2i s2)
{
	if (s1->getCollisionBox().left + s1->getCollisionBox().width < s2.x)
		return false;
	if (s1->getCollisionBox().left > s2.x)
		return false;
	if (s1->getCollisionBox().top + s1->getCollisionBox().height < s2.y)
		return false;
	if (s1->getCollisionBox().top > s2.y)
		return false;

	return true;
}

// check bounding circle collision. Returns true if collision occurs.
bool Collision::checkBoundingCircle(GameObject *s1, GameObject *s2)
{
	// Get radius and centre of sprites.
	float radius1 = s1->getSize().x / 2;
	float radius2 = s2->getSize().x / 2;
	float xpos1 = s1->getPosition().x + radius1;
	float xpos2 = s2->getPosition().x + radius2;
	float ypos1 = s1->getPosition().y + radius1;
	float ypos2 = s2->getPosition().y + radius2;

	if (pow(xpos2 - xpos1, 2) + pow(ypos2 - ypos1, 2) < pow(radius1 + radius2, 2))
	{
		return true;
	}
	return false;
}

bool Collision::checkSimpleCircle(GameObject *sp1, GameObject *sp2)
{
	float minDist = VectorHelper::magnitudeSqrd(sp1->getSize() / 2.f) + VectorHelper::magnitudeSqrd(sp2->getSize() / 2.f);
	float dist = VectorHelper::magnitudeSqrd(sp1->getPosition() - sp2->getPosition());

	return minDist > dist;
}

bool Collision::checkSATBool(GameObject *g1, GameObject *g2)
{
	return checkSAT(g1, g2).collision;
}

CollisionData Collision::checkSAT(GameObject *g1, GameObject *g2)
{
	// This function uses Seperate Axis Theorem rather than AABB for collision checks because its largely meant for checking collisions against tilemaps
	// Because some of the tile in the map are sloped diagonally, their collision is checked rotated by 45deg
	// Only 2 tiles are actually rotated, but this is definately worth it
	// Also the player is rotated, which is importatn
	// Since you can't use AABB on a rect that isn't axis aligned, I need to use SAT ( I think, I don't know what else I would do)
	// Reference: https://dyn4j.org/2010/01/sat/
	// Though the article includes code reference, its quite clear to see this wasn't a direct copy and was used as just reference

	CollisionData data;

	const sf::FloatRect box1 = g1->getCollisionBox();
	const sf::FloatRect box2 = g2->getCollisionBox();

	// quick check to see if objects are generally sharing space and colliders
	if (!g2->isCollider() || !g1->isCollider())
	{
		return data;
	}
	if (checkSimpleCircle(g1, g2))
	{
		std::vector<sf::Vector2f> corners1 = getCorners(*g1);
		std::vector<sf::Vector2f> corners2 = getCorners(*g2);

		std::vector<sf::Vector2f> axes;

		// Get edges of this object
		for (int i = 0; i < corners1.size(); ++i) // use ++i to increment i then return, rather than return i then increment it
		{
			sf::Vector2f edge = corners1[(i + 1) % corners1.size()] - corners1[i];
			axes.push_back({-edge.y, edge.x}); // push bkac the perpendicular vector to the edge
		}

		// Get edges of other object (use size because of custom collision shapes)
		for (int i = 0; i < corners2.size(); ++i) // use ++i to increment i then return, rather than return i then increment it
		{
			sf::Vector2f edge = corners2[(i + 1) % corners2.size()] - corners2[i];
			axes.push_back({-edge.y, edge.x}); // push bkac the perpendicular vector to the edge
		}

		float minPenetration = std::numeric_limits<float>::max();
		sf::Vector2f smallestAxis;

		for (const sf::Vector2f &axis : axes)
		{
			float min1, max1, min2, max2;

			projectPointsToAxis(corners1, axis, min1, max1);
			projectPointsToAxis(corners2, axis, min2, max2);

			if (max1 < min2 || max2 < min1)
			{
				return data; // no collision
			}

			float overlap = std::min(max1, max2) - std::max(min1, min2);
			if (overlap < minPenetration)
			{
				minPenetration = overlap;
				smallestAxis = axis;
			}
		}

		// normalize the axis to use as a normal
		sf::Vector2f normal = VectorHelper::normalise(smallestAxis);

		const sf::Vector2f centre1 = getCentre(*g1);
		const sf::Vector2f centre2 = getCentre(*g2);
		if (VectorHelper::dot(centre2 - centre1, normal) < 0)
		{
			normal = -normal;
		}

		data.normal = normal;
		data.penetrationDepth = minPenetration;
		data.contactPoint = (centre1 + centre2) * 0.5f; // approximate, but gets the job done
		data.collision = true;
		return data;
	}
	return data;
}

std::vector<sf::Vector2f> Collision::getCorners(GameObject &obj)
{
	// If we have special collision, do this one size fits all solution to getting and transforming "corners" (really vertices but I liek consistent naming)
	std::vector<sf::Vector2f> corners;

	sf::ConvexShape const& collisionShape = obj.getBaseHull();
	sf::Transform transform = obj.getTransform();

	size_t pointCount = collisionShape.getPointCount();
	corners.reserve(pointCount);
	
	// go through all the points in the collision shape and transform them
	for (size_t i = 0; i < pointCount; ++i)
	{
		auto p = VectorHelper::rotateDegrees(collisionShape.getPoint(i) * obj.getScale().x, obj.getRotation()) + obj.getPosition() + obj.getOrigin();
		corners.push_back(transform.transformPoint(collisionShape.getPoint(i)));
	}
	return corners;
}

void Collision::projectPointsToAxis(const std::vector<sf::Vector2f> &corners, const sf::Vector2f &axis, float &min, float &max)
{
	min = max = VectorHelper::dot(corners[0], axis); // set min and max in same line to avoid doing the dot twice

	// foreach corner, project it onto the axis
	// then find min and max value sfrom that
	for (const sf::Vector2f &corner : corners)
	{
		float projected = (corner.x * axis.x + corner.y * axis.y);
		if (projected < min)
			min = projected;
		if (projected > max)
			max = projected;
	}
}

sf::Vector2f Collision::getCentre(GameObject &obj)
{
	if (KinematicObject *physObj = dynamic_cast<KinematicObject *>(&obj)) // if this inherits kinematic
	{
		return physObj->getCentre();
	}

	int x = 0;
	int y = 0;

	float count = 0;
	for (auto &c : getCorners(obj))
	{
		count++;
		x += c.x;
		y += c.y;
	}
	return (sf::Vector2f(x, y) / count);
}

CollisionData Collision::checkEdgeCollision(GameObject* g1, GameObject* g2)
{
	//check each edge in g1 against each edge in g2
	std::vector<sf::Vector2f> corners1 = getCorners(*g1);
	std::vector<sf::Vector2f> corners2 = getCorners(*g2);
	CollisionData data;

	if (!g1->isCollider() || !g2->isCollider())
	{
		return data; // no collision if not colliders
	}

	for (size_t i = 0; i < corners1.size(); ++i)
	{
		sf::Vector2f edge1Start = corners1[i];
		sf::Vector2f edge1End = corners1[(i + 1) % corners1.size()];
		for (size_t j = 0; j < corners2.size(); ++j)
		{
			sf::Vector2f edge2Start = corners2[j];
			sf::Vector2f edge2End = corners2[(j + 1) % corners2.size()];
			sf::Vector2f edge1 = edge1End - edge1Start;
			sf::Vector2f edge2 = edge2End - edge2Start;
			float denom = (-edge2.x * edge1.y + edge1.x * edge2.y);
			if (denom == 0) continue; // parallel edges
			float ua = (-edge1.y * (edge1Start.x - edge2Start.x) + edge1.x * (edge1Start.y - edge2Start.y)) / denom;
			float ub = (edge2.x * (edge1Start.y - edge2Start.y) - edge2.y * (edge1Start.x - edge2Start.x)) / denom;
			if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
			{
				data.collision = true;
				data.penetrationDepth = std::min(ua, ub); // penetration depth is the minimum of the two
				data.contactPoint = edge1Start + ua * edge1;
				data.normal = -VectorHelper::normalise(sf::Vector2f(-edge1.y, edge1.x)); // perpendicular to the first edge
				return data; // return on first collision found
			}
		}
	}
	return data; // no collision found
}
