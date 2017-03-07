#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

namespace lab2
{
	static const double PI = 3.14159265358979323846;

	struct  WAV_HEADER {
		char                RIFF[4];        // RIFF Header      Magic header
		unsigned long       ChunkSize;      // RIFF Chunk Size  
		char                WAVE[4];        // WAVE Header      
		char                fmt[4];         // FMT header       
		unsigned long       Subchunk1Size;  // Size of the fmt chunk                                
		unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
		unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Stereo                   
		unsigned long       SamplesPerSec;  // Sampling Frequency in Hz                             
		unsigned long       bytesPerSec;    // bytes per second 
		unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
		unsigned short      bitsPerSample;  // Number of bits per sample      
		char                Subchunk2ID[4]; // "data"  string   
		unsigned long       Subchunk2Size;  // Sampled data length    

	};

	void ex1()
	{
		WAV_HEADER wavHeader;
		FILE *wavFile;
		int headerSize = sizeof(WAV_HEADER), filelength = 0;

		std::string filepath = "../../STM/STM/data/cat_scream.wav";

		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, headerSize, 1, wavFile);

		filelength = 4 + (8 + wavHeader.Subchunk1Size) + (8 + wavHeader.Subchunk2Size) + 8;
		std::cout << "File is                    :" << filelength << " bytes." << std::endl;
		std::cout << "File format is             :" << wavHeader.WAVE[0]
			<< wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << std::endl;
		std::cout << "File audio format is       :" << wavHeader.AudioFormat << std::endl;
		std::cout << "File number of channels is :" << wavHeader.NumOfChan << std::endl;
		std::cout << "File sample rate is        :" << wavHeader.SamplesPerSec << std::endl;
		std::cout << "File bits per sample is    :" << wavHeader.bitsPerSample << std::endl;

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		char *buffer = new char[bytesPerSample];

		for (int i = 0; i < numSamples; ++i) {
			size_t bytesRead = fread(buffer, 1, bytesPerSample, wavFile);

			int value = static_cast<int>(buffer[0]);
			std::cout << (value < 0 ? 256 - std::fabs(value) : value) << " ";
		}

		fclose(wavFile);
	}

	void ex2()
	{
		WAV_HEADER wavHeader;
		FILE *wavFile;

		std::string filepath = "../../STM/STM/data/cat_scream.wav";
		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, sizeof WAV_HEADER, 1, wavFile);
		fclose(wavFile);

		wavHeader.NumOfChan = 1;
		wavHeader.SamplesPerSec = 8000;
		wavHeader.bitsPerSample = 16;
		wavHeader.Subchunk2Size = 2 * wavHeader.SamplesPerSec * wavHeader.bitsPerSample / 8;
		wavHeader.ChunkSize = 36 + wavHeader.Subchunk2Size;
		wavHeader.bytesPerSec = wavHeader.SamplesPerSec * 2;
		wavHeader.blockAlign = 2;
		int16_t *data = new int16_t[wavHeader.SamplesPerSec * 2];

		double step = PI * 2.0 * 400 / 8000;
		double crt = 0;
		int A = 15000;

		for (int i = 0; i < wavHeader.SamplesPerSec * 2; i++) {
			data[i] = static_cast<int16_t>(A * sin(crt));
			crt += step;
		}

		FILE *fout;
		fopen_s(&fout, "ex2a.wav", "wb");
		fwrite(&wavHeader, sizeof WAV_HEADER, 1, fout);
		fwrite(data, 2, wavHeader.SamplesPerSec * 2, fout);
		fclose(fout);

		FILE *fout2;
		fopen_s(&fout2, "ex2b.wav", "wb");
		fwrite(&wavHeader, sizeof WAV_HEADER, 1, fout2);

		int16_t *data2 = new int16_t[wavHeader.SamplesPerSec * 2];

		step = PI * 2.0 * 1000 / 8000;
		crt = 0;
		A = 10000;

		for (int i = 0; i < wavHeader.SamplesPerSec * 2; i++) {
			data2[i] = static_cast<int16_t>(A * sin(crt));
			crt += step;
		}

		fwrite(data2, 2, wavHeader.SamplesPerSec * 2, fout2);
		fclose(fout2);

		FILE *fout3;
		fopen_s(&fout3, "ex2c.wav", "wb");
		fwrite(&wavHeader, sizeof WAV_HEADER, 1, fout3);

		int16_t *data3 = new int16_t[wavHeader.SamplesPerSec * 2];

		for (int i = 0; i < wavHeader.SamplesPerSec * 2; i++) {
			data3[i] = data[i] + data2[i];
		}

		fwrite(data3, 2, wavHeader.SamplesPerSec * 2, fout3);
		fclose(fout3);
	}
}