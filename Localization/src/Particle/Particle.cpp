/**
*	@File   - Particle.cpp
*	@Author - John Allard, Alex Rich. Summer 2014.
*	@Info   - This is the definition of the Particle class for use by the 3DLocalization program, declared in the Particle.h file. This file will define all
*			of then public and private member functions for the class. For a more in-depth description of this class, see it's section 
*			in the Documentation/CodeDocumentation directory.
**/


#include "Particle.h"

namespace MCL
{

	Particle::Particle() :
	weightmin(0), weightmax(100), defaultweight(10), perspective()
	{
		weight = defaultweight;
	}

	Particle::Particle(Perspective p) :
	weightmin(0), weightmax(100), defaultweight(10), perspective(p)
	{
		weight = defaultweight;
	}

	Particle(Perspective p, float w) :
	weightmin(0), weightmax(100), defaultweight(10), perspective(p)
	{
		if(w >= weightmin && w <= weightmax)
			weight = w;
		else
			weight = defaultweight;
	}

	//====Destructor====//
	~Particle()
	{
		// HOLDER FUNCTION
	}


	//====Get and Set====//
	bool Particle::SetPerspective(Perspective p)
	{
		this->perspective = p;
	}

	MCL::Perspective Particle::getPerspective() const
	{
		return this->perspective;
	}

	float Particle::getPerspective(int index) const
	{
		if(index >= 0 && index < 6)
		{
			std::vector<float> temp = perspective.ToVector();
			return 
		}
		else
		{
			MCL::ErrorIO(new std::string(" Invalid Index Argument in GetPerspective Function Call"));
			throw std::logic_error("Invalid Index in GetPerspective Call");
		}
	}

	float Particle::getWeight() const
	{
		return this->weight;
	}

	bool Particle::setWeight(float)
	{
		if(w >= weightmin && w <= weightmax)
		{
			weight = w;
			return true;
		}
		else
			return false;
	}



}
