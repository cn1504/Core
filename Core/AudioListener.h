#pragma once
#include "Core.h"

namespace Core
{
	
	class Entity;
	class AudioFile;


	class Track
	{
	private:
		AudioFile* file;
		Entity* bond;
		glm::vec3 position;
		glm::vec3 velocity;
		float volume;
		bool loop;

		float elapsedTime;

		ALuint buffer;
		ALuint source;

		void Play();

	public:
		Track(AudioFile* file, Entity* bond, float volume, bool loop);
		Track(AudioFile* file, glm::vec3 position, glm::vec3 velocity, float volume, bool loop);
		~Track();

		void Update();
		bool HasFinished();
	};


	static class AL
	{
	private:
		ALCdevice *device;
		ALCcontext *ctx;
		Entity *entity;
		std::list<Track*> tracklist;

	public:
		void Init();
		void Update();
		void Close();

		void SetCurrentListener(Entity* e);
		void PlayTrack(Track* track);

	} AudioListener;

}