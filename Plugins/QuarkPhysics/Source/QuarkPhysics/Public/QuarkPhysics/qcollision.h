
/************************************************************************************
 * MIT License
 *
 * Copyright (c) 2023 Eray Zesen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://github.com/erayzesen/QuarkPhysics
 *
**************************************************************************************/

#ifndef QCOLLISION_H
#define QCOLLISION_H
#include <vector>
#include "qparticle.h"
#include "qobjectpool.h"


class QWorld;
class QAABB;


/** @brief The QCollision class performs all collision detection operations. The relevant methods return contact data from the collision tests.
 */
class QCollision
{
public:
	QCollision(){}
	~QCollision();

	


	/** @brief Contains all the contact information required to resolve collisions. */
	struct Contact{
	public:
		/** The contact position of the collision */
		QVector position;
		/** The incident particle of the collision */
		QParticle *particle;
		/** The normal of the collision */
		QVector normal;
		/** The penetration of the collision */
		float penetration;
		/** The particle list of the reference face in the collision */
		std::vector<QParticle*> referenceParticles;
		/** Determines whether the contact is solved. */
		bool solved;
		Contact(QParticle *InParticle,QVector InPosition,QVector InNormal,float InPenetration,std::vector<QParticle*> InReferenceParticles ){
			this->particle=InParticle;
			this->position=InPosition;
			this->normal=InNormal;
			this->penetration=InPenetration;
			this->referenceParticles=InReferenceParticles;
			this->solved=false;
		}

		Contact(){

		}

		void Configure(QParticle *InParticle,QVector InPosition,QVector InNormal,float InPenetration,std::vector<QParticle*> InReferenceParticles){
			this->particle=InParticle;
			this->position=InPosition;
			this->normal=InNormal;
			this->penetration=InPenetration;
			this->referenceParticles=InReferenceParticles;
			this->solved=false;
		}



	};

	static QObjectPool<QCollision::Contact> contactPool;

	static QObjectPool<QCollision::Contact> &GetContactPool();


	struct Project{
		public:
			float min;
			float max;
			int minIndex;
			int maxIndex;
			Project(float projectMin,int minPointIndex,float projectMax,int maxPointIndex){
				min=projectMin;
				max=projectMax;
				minIndex=minPointIndex;
				maxIndex=maxPointIndex;
			}
			float Overlap(const Project other) const{
				float penetration=0.0f;
				if(other.min<min){
					penetration=min-other.max;
				}else{
					penetration=other.min-max;
				}
				return penetration;
			}

	};

	

	//Collision Methods
	/** Checks collisions between polygons. 
	 * @param particlesA A collection of particles that make up a polygon.
	 * @param particlesB Another collection of particles that make up a polygon.
	 * @param contacts A collection where collision contact information will be stored.
	 */
	static void PolygonAndPolygon(std::vector<QParticle*> &particlesA,std::vector<QParticle*> &particlesB,std::vector<QCollision::Contact*> &contacts);
	/** Checks collisions between circle(s) and polygon. 
	 * @param circleParticles A collection of particles representing one or more circles, each having a radius.
	 * @param polygonParticles A collection of particles that make up a polygon.
	 * @param contacts A collection where collision contact information will be stored.
	 */
	static void CircleAndPolygon(std::vector<QParticle*> &circleParticles,std::vector<QParticle*> &polygonParticles,std::vector<QCollision::Contact*> &contacts);

	
	/** Checks collisions between circle(s) and circle(s). 
	 * @param particlesA A collection of particles representing one or more circles, each having a radius.
	 * @param particlesB Another collection of particles representing one or more circles, each having a radius.
	 * @param contacts A collection where collision contact information will be stored.
	 */
	static void CircleAndCircle(std::vector<QParticle*> &particlesA,std::vector<QParticle*> &particlesB, QAABB boundingBoxB, std::vector<QCollision::Contact*> &contacts, float specifiedRadius=0.0f, bool velocitySensitive=false);
	/** Checks collisions between circle(s) and circle(s) for the self collisions. 
	 * @param particlesA A collection of particles representing one or more circles, each having a radius.
	 * @param contacts A collection where collision contact information will be stored.
	 */
	static void CircleAndCircleSelf(std::vector<QParticle*> &particles, std::vector<QCollision::Contact*> &contacts, float specifiedRadius=0.0f);
	/** Checks collisions between polyline and polygon. 
	 * @param polylineParticles A collection of particles that make up a polyline.
	 * @param polygonParticles Another collection of particles that make up a polygon.
	 * @param contacts A collection where collision contact information will be stored.
	 * \note To check collisions between two polylines, call this method twice with the polylines swapped as arguments.
	 * \note "Polyline" is commonly used to define polygons for softbody objects. In contrast to a solid and filled polygon , you can think of a polyline as a polygon shape formed by a  connected and knotted rope. If a polygon-shaped rope is deformed, it may not always remain a polygon, it can overlap and its corners can merge. Collision testing for a polyline is performed based on these assumptions
	 */
	static void PolylineAndPolygon(std::vector<QParticle*> &polylineParticles,std::vector<QParticle*> &polygonParticles,std::vector<QCollision::Contact*> &contacts);

	/** Checks collisions between polyline and polyline. 
	 * @param circleParticles A collection of particles representing one or more circles, each having a radius.
	 * @param polylineParticles A collection of particles that make up a polyline.
	 * @param polylineAABB AABB of polyline for tests between circle and AABB.
	 * @param contacts A collection where collision contact information will be stored.
	 * @param circlesArePolygon It defines whether circle particles is the part of a polygon.  
	 */
	static void PolylineAndPolyline(std::vector<QParticle*> &testPolylineParticles,std::vector<QParticle*> &targetPolylineParticles,QAABB polylineAABB , std::vector<QCollision::Contact*> &contacts);

	static void CircleAndPolyline2(std::vector<QParticle*> &particles,std::vector<QParticle*> &polygonParticles,QAABB polylineAABB , std::vector<QCollision::Contact*> &contacts);


	//Geometry Helper Methods
	/** Checks intersection between lines.
	 * @param d1A The point A of the d1 line.
	 * @param d1B The point B of the d1 line.
	 * @param d2A The point A of the d2 line.
	 * @param d2B The point B of the d2 line.
	 * @return An intersection position. If the intersection doesn't exist, returns QVector::NaN. 
	 */
	static QVector LineIntersectionLine(QVector d1A,QVector d1B, QVector d2A, QVector d2B);
	/** Checks whether a specified point is inside the specified polygon.
	 * @param point A point to check.
	 * @param polygon A collection of particles that make up a polygon.
	 * @return Returns true if the point is inside the polygon, false otherwise.
	 */
	static bool PointInPolygon(QVector &point, std::vector<QParticle*> &polygon );
	/** Checks whether a specified point is inside the specified polygon.
	 * @param point A point to check.
	 * @param polygon A collection of vectors that make up a polygon.
	 * @return Returns true if the point is inside the polygon, false otherwise.
	 */
	static bool PointInPolygon(QVector &point, std::vector<QVector> &polygon );
	/** Checks whether a specified point is inside the specified particle polygon.
	 * @param point A point to check.
	 * @param polygon A collection of particles that make up a polygon.
	 * @return Returns true if the point is inside the polygon, false otherwise.
	 */
	static bool PointInPolygon2(QVector point, std::vector<QParticle*> &polygon );

	
	
private:

	//Collision Helper Methods
	static void ClipContactParticles(QParticle *referenceParticles[], QParticle *incidentParticles[], std::vector<QCollision::Contact*> &contacts );
	static Project ProjectToAxis(QVector &normal,std::vector<QParticle*> &polygon);
	static std::vector<QVector> ParticlePolygonToPolygon(std::vector<QParticle*> particlePolygon);
	static std::pair<int,int> FindNearestSideOfPolygon(const QVector point, std::vector<QParticle*> polygonParticles,bool checkSideRange=false, bool checkNegativeDistance=false);
	static int FindNearestParticleOfPolygon(QParticle * particle, std::vector<QParticle*> polygonParticles);
	static int FindExtremeParticleOfAxis(std::vector<QParticle*> polygonParticles, QVector axisNormal);
	static bool PointInPolygonWN(const QVector point, std::vector<QParticle*> polygonParticles);

	


};




#endif // QCOLLISION_H
