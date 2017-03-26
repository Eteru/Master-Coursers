#pragma once

#include "lab2.hpp"

#include <complex>

namespace lab5
{
	void writeToCSV(std::vector<double> & mag)
	{
		static int crt = 1;
		bool showFreq = (crt == 3) ? true : false;
		std::ofstream csvFile("data\\mag" + std::to_string(crt) + ".csv");

		size_t i_max = std::distance(mag.begin(), std::max_element(mag.begin(), mag.end()));
		int Fs = 8000;

		for (int i = 0; i < mag.size(); ++i) {
			if (showFreq)
				csvFile << mag[i] << "," << static_cast<int>(i_max *Fs / mag.size())<< std::endl;
			else
				csvFile << mag[i] << std::endl;
		}

		csvFile.close();
		crt++;
	}

	void dft(int16_t *data, std::vector<std::complex<double>> & res, int size)
	{
		res.clear();
		std::vector<double> mag;
		double prod = -2.0 * lab2::PI / size;
		for (unsigned int k = 0; k < size; k++) {
			std::complex<double> sum(0.0, 0.0);

			for (unsigned int j = 0; j < size; j++) {
				std::complex<double> my_exponent(std::polar(1.0, j * k * prod));
				sum += static_cast<double>(data[j]) * std::exp(my_exponent);
			}

			mag.push_back(sqrt(sum.real() * sum.real() + sum.imag() * sum.imag()));
			res.push_back(sum);
		}
		writeToCSV(mag);
	}

	void idft(std::vector<std::complex<double>> & data, std::vector<int16_t> & res, int size)
	{
		res.clear();
		std::vector<double> mag;
		double prod = 2.0 * lab2::PI / size;
		for (unsigned int k = 0; k < size; k++) {
			std::complex<double> sum(0.0, 0.0);

			for (unsigned int j = 0; j < size; j++) {
				std::complex<double> my_exponent(std::polar(1.0, j * k * prod));
				sum += data[j] * std::exp(my_exponent);
			}

			res.push_back(static_cast<int16_t>(sum.real() / static_cast<double>(size)));
		}
	}

	void ex1()
	{
		lab2::WAV_HEADER wavHeader;
		FILE *wavFile;

		std::string filepath = "../../STM/STM/data/cat_scream.wav";
		if (fopen_s(&wavFile, filepath.c_str(), "rb") != 0) {
			std::cerr << "Unable to open file " << filepath.c_str() << std::endl;
		}

		fread(&wavHeader, sizeof lab2::WAV_HEADER, 1, wavFile);
		fclose(wavFile);

		wavHeader.NumOfChan = 1;
		wavHeader.SamplesPerSec = 8000;
		wavHeader.bitsPerSample = 16;
		wavHeader.Subchunk2Size = 2 * wavHeader.SamplesPerSec * wavHeader.bitsPerSample / 8;
		wavHeader.ChunkSize = 36 + wavHeader.Subchunk2Size;
		wavHeader.bytesPerSec = wavHeader.SamplesPerSec * 2;
		wavHeader.blockAlign = 2;
		int16_t *data = new int16_t[1024];

		double step = lab2::PI * 2.0 * 400 / 8000;
		double crt = 0;
		int A = 15000;

		for (int i = 0; i < 1024; i++) {
			data[i] = static_cast<int16_t>(A * sin(crt));
			crt += step;
		}

		std::vector<std::complex<double>> res;
		dft(data, res, 1024);

		FILE *fout;
		fopen_s(&fout, "s1.wav", "wb");
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, fout);
		fwrite(data, 2, 1024, fout);
		fclose(fout);

		FILE *fout2;
		fopen_s(&fout2, "s2.wav", "wb");
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, fout2);

		int16_t *data2 = new int16_t[1024];

		step = lab2::PI * 2.0 * 1000 / 8000;
		crt = 0;
		A = 10000;

		for (int i = 0; i < 1024; i++) {
			data2[i] = static_cast<int16_t>(A * sin(crt));
			crt += step;
		}

		dft(data2, res, 1024);

		fwrite(data2, 2, 1024, fout2);
		fclose(fout2);

		FILE *fout3;
		fopen_s(&fout3, "s3.wav", "wb");
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, fout3);

		int16_t *data3 = new int16_t[1024];

		for (int i = 0; i < 1024; i++) {
			data3[i] = data[i] + data2[i];
		}

		dft(data3, res, 1024);

		std::vector<int16_t> ires;
		idft(res, ires, 1024);

		FILE *fout4;
		fopen_s(&fout4, "s3idft.wav", "wb");
		fwrite(&wavHeader, sizeof lab2::WAV_HEADER, 1, fout4);

		fwrite(data3, 2, 1024, fout3);
		fwrite(&ires[0], 2, 1024, fout4);

		fclose(fout3);
		fclose(fout4);

		std::ofstream csvFile("data\\test.csv");

		for (int i = 0; i < ires.size(); ++i) {
			csvFile << data[i] << "," << ires[i] << std::endl;
		}

		csvFile.close();
	}
}