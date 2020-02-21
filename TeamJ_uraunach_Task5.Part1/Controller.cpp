#include "Projectile.h"
#include "Controller.h"
#include <cmath>
#include <iostream>
using namespace std;
namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
			int w = g.getWidth();
			for(int i = 0; i < w + 1; i++)
				dangerZone_.push_back(0);
			p -> x = w / 2.0;
		}
		if(pickSafeSpot(g) == 1)
			p -> x -= g.playerSpeed;
		else if(pickSafeSpot(g) == -1)
			p -> x += g.playerSpeed;
		else
			p -> x = p -> x;
		determineSafeSpots(g);
		//p -> x = pickSafeSpot(g);
		//double hi = 1.0000;
		cout << p -> x << endl;
	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
			int w = g.getWidth();
			for(int i = 0; i < w + 1; i++)
				dangerZone_.push_back(0);
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2.0f;
			g.newPlayer(p);
		}

	}

	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		float gravity = g.getGravity();
		float time = g.getGameTime();
		Prediction pred;
		pred.t = time - (p.vy + sqrt(p.vy * p.vy - (2 * gravity * p.y))) / gravity;
		pred.x = p.x + p.vx * (pred.t - time);
		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{
		list<Projectile> projectiles = g.getProjectiles();
		list<Explosion> explosions = g.getExplosions();
		float time = g.getGameTime();
		float timestep = g.getTimeStep();
		int w = g.getWidth();
		float lb, ub;
		//cout << time << endl;
		//dangerZone_ = new int[w+1];
		for(int i = 0; i < w+1; ++i)
		{
			//cout << dangerZone_[i];
			dangerZone_[i] = 0;
		}

		for(list<Explosion>::iterator it = explosions.begin();
				it != explosions.end(); it++)
		{	
			for(int i = floor(it -> x - g.explosionSize - 5);
					i <= ceil(it -> x + g.explosionSize + 5);
					++i)
			{
				if( i >= 0 && i <= w )
					dangerZone_[i] = 1;
			}
		}

		for(list<Projectile>::iterator it = projectiles.begin();
				it != projectiles.end(); it++)
		{
			Prediction pred = trackProjectile(*it, g);
			lb = pred.t - 750 * timestep;
			ub = pred.t + 750 * timestep + g.explosionTime;
			//cout << lb << "\t" << time << "\t" << ub << endl;
			//cout << time << "\t" << pred.t << endl;
			if(time >= lb && time <= ub)
			{
				for(int i = floor(pred.x - g.explosionSize - 5);
				i <= ceil(pred.x + g.explosionSize + 5);
				++i)
				{
					if( i >= 0 && i <= w )
					{
						dangerZone_[i] = 1;
						//cout << i << "\t" << p -> x << endl;
					}
				}

			}
		}
		cout << endl;
	}

	int Controller::pickSafeSpot(const Game& g)
	{
		int w = g.getWidth();
		float curr = p -> x * 1.0f;
		int l = 0, r = 0;
		for(int i = int(curr + 1); i < w+1; ++i) 
		{
			if(dangerZone_[i] == 0)
			{
				r = i - curr;
				break;
			}
		}
		for(int i = int(curr); i >= 0; i--)
		{
			if(dangerZone_[i] == 0)
			{
				l = curr - i;
				break;
			}
		}
		//cout << curr - g.playerSpeed << "\t" << curr + g.playerSpeed << endl;
		if(dangerZone_[int(curr)])
			return (l < r)? 1 : -1;
			//return (l < r)? curr - l : curr + r;
		else
			return 0;
	}
}
