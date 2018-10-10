#pragma once

#define N_CHANNELS 2

#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <portaudio.h>
#include <sndfile.h>

namespace Audio{

	class Sample{
		private:
			int n_channels;
			unsigned long int n_frames;

		public:
			float *data;
			unsigned long int position;

			Sample(char *filename);
			~Sample();

			unsigned long int get_n_frames();
			int get_n_channels();

			bool finished();

	};


	class Player {
		private:
			Sample *audio_sample;
			bool playing;

			PaStreamParameters  outputParameters;
			PaStream*           stream;
			PaError             err;
			PaTime              streamOpened;


		public:
			Player();
			~Player();

			static int PA_Callback (const void *inputBuffer, void *outputBuffer,
							 unsigned long framesPerBuffer,
							 const PaStreamCallbackTimeInfo* timeInfo,
							 PaStreamCallbackFlags statusFlags,
							 void *userData );

			void init();
			void pause();
			void stop();
			void play(Sample *audiosample);
			Sample *get_data();
	};
}
