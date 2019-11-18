#ifndef __P2ANIMATION_H__
#define __P2ANIMATION_H__
#include <assert.h>
#include "SDL/include/SDL_rect.h"

#define MAX_FRAMES 50

class Animation
{
public:
	float firstLoopFrame = 0.0f;
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	int last_frame = 0;


private:

	float current_frame;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect)
	{
		if (last_frame < MAX_FRAMES)
			frames[last_frame++] = rect;
		assert(last_frame < MAX_FRAMES);
	}

	void load_animation(pugi::xml_node animationIterator, p2SString name)
	{
		for (animationIterator = animationIterator; animationIterator != NULL; animationIterator = animationIterator.next_sibling("animation"))
		{
			if (name == animationIterator.attribute("name").as_string())
			{
				for (pugi::xml_node framesIterator = animationIterator.child("sprite"); framesIterator; framesIterator = framesIterator.next_sibling("sprite"))
				{
					SDL_Rect frame;
					frame.x = framesIterator.attribute("x").as_int();
					frame.y = framesIterator.attribute("y").as_int();
					frame.w = framesIterator.attribute("w").as_int();
					frame.h = framesIterator.attribute("h").as_int();

					this->loop = animationIterator.child("animValues").attribute("loop").as_bool();
					this->speed = animationIterator.child("animValues").attribute("speed").as_float();

					this->PushBack(frame);
				}
				break;
			}
		}
	}


	SDL_Rect& GetCurrentFrame(float dt) //float dt
	{
		float tmp = speed;
		current_frame += tmp * dt; //*dt

		if (current_frame < 0)
		{
			current_frame = 0;
		}

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? firstLoopFrame : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}

	float GetCurrentFrameinFloat()
	{
		return current_frame;
	}

	int GetLastFrameinInt()
	{
		return last_frame;
	}

	void ChangeCurrentFrame(float frame)
	{
		current_frame = frame;
	}
};

#endif