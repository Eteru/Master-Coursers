#pragma once

#include "lab2.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

namespace lab4
{
	int16_t sampleToDBFS(int16_t value, int bits)
	{
		return 20 * static_cast<int16_t>(std::log(std::fabs(value) / (1 << (bits - 1))));
	}

	int16_t dbfsToPCM(int16_t value, int bits)
	{
		return static_cast<int16_t>((1 << (bits - 1)) * std::pow(10, std::fabs(value) / 20.f));
	}

	void applyCompression(std::vector<int16_t> & dbfs, int lowerThreshold, int upperThreshold)
	{
		for (size_t i = 0; i < dbfs.size(); ++i) {
			if (dbfs[i] > upperThreshold) {
				dbfs[i] = upperThreshold + (dbfs[i] - upperThreshold) / 2;
			}
			else if (dbfs[i] < lowerThreshold) {
				dbfs[i] = lowerThreshold - (lowerThreshold - dbfs[i]) / 2;
			}
		}
	}

	void ex1(lab2::WAV_HEADER & wavHeader, std::vector<int16_t> & dBFS, std::vector<bool> & signs)
	{
		FILE *wavFile;
		int headerSize = sizeof(lab2::WAV_HEADER);

		std::string filepath = "../../STM/STM/data/bike.wav";

		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, headerSize, 1, wavFile);

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		int16_t inValue;

		for (unsigned int i = 0; i < numSamples; ++i) {
			size_t bytesRead = fread(&inValue, bytesPerSample, 1, wavFile);

			signs.push_back(inValue >= 0 ? true : false);
			dBFS.push_back(sampleToDBFS(inValue, 16));
		}

		fclose(wavFile);
	}

	void ex2(lab2::WAV_HEADER & wavHeader, std::vector<int16_t> & dbfs, std::vector<bool> & signs)
	{
		// apply compression
		applyCompression(dbfs, -60, -40);

		FILE *wavFile;
		int headerSize = sizeof(lab2::WAV_HEADER);

		std::string filepath = "../../STM/STM/data/ex2.wav";

		fopen_s(&wavFile, filepath.c_str(), "wb");

		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, wavFile);

		std::vector<int16_t> pcm(dbfs.size());
		for (int i = 0; i < dbfs.size(); ++i) {
			pcm[i] = (signs[i] ? 1 : -1) * dbfsToPCM(dbfs[i], 16);
		}

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		fwrite(&pcm[0], 2, numSamples, wavFile);
		fclose(wavFile);
	}

	void ex3(lab2::WAV_HEADER & wavHeader, std::vector<int16_t> & dbfs, std::vector<bool> & signs)
	{
		int16_t maxVal = static_cast<int16_t>(std::fabs(*std::max_element(dbfs.begin(), dbfs.end())));

		std::transform(dbfs.begin(), dbfs.end(), dbfs.begin(), [&](int16_t val) {
			return val + maxVal;
		});

		FILE *wavFile;
		int headerSize = sizeof(lab2::WAV_HEADER);

		std::string filepath = "../../STM/STM/data/ex3.wav";

		fopen_s(&wavFile, filepath.c_str(), "wb");

		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, wavFile);

		std::vector<int16_t> pcm(dbfs.size());
		for (int i = 0; i < dbfs.size(); ++i) {
			pcm[i] = (signs[i] ? 1 : -1) * dbfsToPCM(dbfs[i], 16);
		}

		unsigned int bytesPerSample = wavHeader.bitsPerSample / 8;
		unsigned int numSamples = wavHeader.ChunkSize / bytesPerSample;

		fwrite(&pcm[0], 2, numSamples, wavFile);
		fclose(wavFile);
	}
}
