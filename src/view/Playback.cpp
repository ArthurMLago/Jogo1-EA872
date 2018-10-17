#include "Playback.hpp"

using namespace Audio;

Sample::Sample(char *filename){
	SNDFILE * infile;
	SF_INFO sfinfo;

	float buffer[2520];

	if (! (infile = sf_open (filename, SFM_READ, &sfinfo))){
		printf ("Not able to open input file %s.\n", filename) ;
		/* Print the error message from libsndfile. */
		puts (sf_strerror (NULL)) ;

		n_channels = 0;
		n_frames = 0;
		data = NULL;
		return;
	}

	n_frames = sfinfo.frames;
	// Aceitamos no maximo 2 canais:
	//n_channels = sfinfo.channels > 2 ? 2 : sfinfo.channels;
	n_channels = N_CHANNELS;

	// Alocar buffer do tamanho necessario:
	data = (float *)malloc(n_frames * n_channels * sizeof(float));

	int read_bytes;
	int offset = 0;
	int channel = 0;
	do{
		read_bytes = sf_read_float(infile, buffer, 2520);
		for (int i = 0; i < read_bytes; i++){
			//printf("%f\n", buffer[i]);


			if (channel < n_channels){
				data[offset] = buffer[i];
				offset++;
			}
			// Se o arquivo nao tem canais o suficiente para oque queremos, completar com o ultimo canal:
			if (offset % n_channels >= sfinfo.channels){
				for (int j = 0; j < n_channels - sfinfo.channels; j++){
					data[offset] = buffer[i];
					offset++;
				}
			}
			channel = (channel + 1) % sfinfo.channels;
		}

	}while(read_bytes == 2520);

	sf_close(infile);

	this->position = 0;


}

Sample::~Sample(){
	if (data != NULL)
		free(data);
}

unsigned long int Sample::get_n_frames(){
	return n_frames;
}

int Sample::get_n_channels(){
	return n_channels;
}

bool Sample::finished(){
	if (this->position >= this->n_frames)
		return true;
	return false;
}



Player::Player() {
	this->playing = false;
	this->audio_sample = NULL;

	PaError err;

	err = Pa_Initialize();
	if( err != paNoError ) {
		std::cerr << "Error on Pa_Initialize()" << std::endl;
		return;
	}

	outputParameters.device = Pa_GetDefaultOutputDevice();// [> Default output device. <]
	if (outputParameters.device == paNoDevice) {
		std::cerr << "Error: No default output device on Pa_GetDefaultOutputDevice()" << std::endl;
		return;
	}

	outputParameters.channelCount = N_CHANNELS;                   //  [> Mono output. <]
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream( &stream,
						 NULL,    //  [> No input. <]
						 &outputParameters,
						 44100,
						 64,      // [> Frames per buffer. <]
						 paClipOff,// [> We won't output out of range samples so don't bother clipping them. <]
						 &Player::PA_Callback,
						 this );

	if( err != paNoError ) {
		std::cerr << "Error on Pa_OpenStream()" << std::endl;
		return;
	}

	err = Pa_StartStream( stream );
	if( err != paNoError ) {
		std::cerr << "Error on Pa_StartStream()" << std::endl;
		return;
	}


}

Player::~Player() {
	stop();
}

void Player::play(Sample *audiosample){
	this->audio_sample = audiosample;
}

void Player::pause() {
  this->playing = false;
}

void Player::stop() {
	PaError err;
	err = Pa_StopStream( stream );
	if( err != paNoError ) {
		std::cerr << "Error on Pa_StopStream()" << std::endl;
		return;
	}

	err = Pa_CloseStream( stream );
	if( err != paNoError ) {
		std::cerr << "Error on Pa_StopStream()" << std::endl;
		return;
	}

	Pa_Terminate();

}


Sample *Player::get_data(){
	return audio_sample;
}


int Player::PA_Callback (const void *inputBuffer, void *outputBuffer,
				 unsigned long framesPerBuffer,
				 const PaStreamCallbackTimeInfo* timeInfo,
				 PaStreamCallbackFlags statusFlags,
				 void *userData ){

	Player *player = (Player*) userData;
	float *buffer = (float *) outputBuffer;
	Sample *s = player->get_data();

	if (s != NULL){
		unsigned long int pos = s->position;
		int i = 0;
		while(i/N_CHANNELS < framesPerBuffer){
			if (pos < s->get_n_frames()){
				for (int j = 0; j < N_CHANNELS; j++){
					buffer[i++] = s->data[pos*N_CHANNELS + j] ;
				}
				pos++;
			}else{
				for (int j = 0; j < N_CHANNELS; j++){
					buffer[i++] = 0;
				}
			}

		}
		s->position = pos;
	}else{
		memset(buffer, 0, framesPerBuffer * sizeof(float) * N_CHANNELS);
	}
	return 0;

}
