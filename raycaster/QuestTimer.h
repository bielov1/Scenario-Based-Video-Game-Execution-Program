#pragma once

class QuestTimer
{
private:
	int timer;
	bool active;
public:
	QuestTimer()
		: timer(0), active(false) {}

	void reduce_time(int sec)
	{
		timer -= sec;
		if (timer < 0) 
			timer = 0;
	}

	void increase_timer(int sec)
	{
		timer += sec;
	}

	void set_timer(int time)
	{ timer = time; }

	int get_timer_count()
	{ return timer; }

	void activate()
	{ active = true; }

	bool is_active()
	{ return active; }

	void disable()
	{ active = false; }
};