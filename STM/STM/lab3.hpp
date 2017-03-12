#pragma once

#include "utils.hpp"
#include "lab2.hpp"

#include <iostream>
#include <fstream>

namespace lab3
{
	void ex1()
	{
		lab2:: WAV_HEADER wavHeader;
		FILE *wavFile, *outWavFile;
		int headerSize = sizeof(lab2::WAV_HEADER), filelength = 0;

		std::string filepath = "../../STM/STM/data/beep.wav";
		std::string outFilepath = "../../STM/STM/data/beep1.wav";

		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, headerSize, 1, wavFile);

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		fopen_s(&outWavFile, outFilepath.c_str(), "wb");
		wavHeader.bitsPerSample = 8;
		wavHeader.Subchunk2Size = 2 * wavHeader.SamplesPerSec * wavHeader.bitsPerSample / 8;
		wavHeader.ChunkSize = 36 + wavHeader.Subchunk2Size;
		wavHeader.bytesPerSec = wavHeader.SamplesPerSec;
		wavHeader.blockAlign = 1;
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, outWavFile);

		int16_t inValue;
		std::vector<int8_t> outValues;

		for (int i = 0; i < numSamples; ++i) {
			size_t bytesRead = fread(&inValue, bytesPerSample, 1, wavFile);

			float aux = utils::sign(inValue) * log(1 + 255 * fabsf(float(inValue) / 32768)) / 5.5452f;
			outValues.push_back(floor(128 * aux));
		}

		fwrite(&outValues[0], 1, numSamples, outWavFile);
		fclose(outWavFile);

		fclose(wavFile);
	}

	void ex2()
	{
		lab2::WAV_HEADER wavHeader;
		FILE *wavFile, *outWavFile;
		int headerSize = sizeof(lab2::WAV_HEADER), filelength = 0;

		std::string filepath = "../../STM/STM/data/beep1.wav";
		std::string outFilepath = "../../STM/STM/data/beep2.wav";

		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, headerSize, 1, wavFile);

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		fopen_s(&outWavFile, outFilepath.c_str(), "wb");
		wavHeader.bitsPerSample = 16;
		wavHeader.Subchunk2Size = 2 * wavHeader.SamplesPerSec * wavHeader.bitsPerSample / 8;
		wavHeader.ChunkSize = 36 + wavHeader.Subchunk2Size;
		wavHeader.bytesPerSec = wavHeader.SamplesPerSec * 2;
		wavHeader.blockAlign = 2;
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, outWavFile);

		char inValue;
		std::vector<int16_t> outValues;

		for (int i = 0; i < numSamples; ++i) {
			size_t bytesRead = fread(&inValue, bytesPerSample, 1, wavFile);

			int value = inValue;

			float aux = utils::sign(value) * log(std::powf(256, fabsf(float(value) / 128)) - 1) / 255;
			outValues.push_back(ceil(32768 * aux));
		}

		fwrite(&outValues[0], 2, numSamples, outWavFile);
		fclose(outWavFile);

		fclose(wavFile);
	}

	void ex3()
	{
		lab2::WAV_HEADER wavHeader;
		FILE *wavFile;
		int headerSize = sizeof(lab2::WAV_HEADER), filelength = 0;

		std::string filepath = "../../STM/STM/data/beep1.wav";
		std::string outFilepath = "../../STM/STM/data/beep1.csv";

		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, headerSize, 1, wavFile);

		unsigned int bytesPerSample = 1;
		unsigned int numSamples = wavHeader.ChunkSize;

		std::ofstream csvFile(outFilepath);
		int8_t inValue;

		int hist[256] = {0};

		for (int i = 0; i < numSamples; ++i) {
			size_t bytesRead = fread(&inValue, bytesPerSample, 1, wavFile);

			hist[int(inValue) + 128] += 1;
		}

		for (int i = 0; i < 256; ++i) {
			csvFile << static_cast<int>(hist[i]) << std::endl;
		}

		csvFile.close();

		fclose(wavFile);
	}
}