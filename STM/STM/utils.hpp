#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

namespace utils
{
	struct ItemFreq {
		std::string color;
		float freq;
	};

	struct ItemWInterval {
		std::string color;
		float freq;
		float low;
		float high;
	};

	std::map<std::string, std::string> code;

	inline void InitShannonFano(std::vector<ItemFreq> & colors)
	{
		code.clear();

		for (auto it = colors.begin(); it != colors.end(); ++it) {
			code[(*it).color] = "";
		}
	}

	void ShannonFano(std::vector<ItemFreq>::iterator begin, std::vector<ItemFreq>::iterator end)
	{
		size_t dist = std::distance(begin, end);
		if (dist == 1)
			return;

		float totalFreq = 0.f;
		for (auto it = begin; it != end; ++it)
			totalFreq += (*it).freq;

		float halfFreq = 0.5f * totalFreq;

		totalFreq = 0.f;
		std::vector<ItemFreq>::iterator mid = end;
		for (auto it = begin; it != end; ++it) {
			totalFreq += (*it).freq;

			if (totalFreq <= halfFreq) {
				code[(*it).color] += "0";
			}
			else {
				code[(*it).color] += "1";

				if (mid == end)
					mid = it;
			}
		}

		ShannonFano(begin, mid);
		ShannonFano(mid, end);
	}

	inline void PrintShannonFano()
	{
		for (auto it = code.begin(); it != code.end(); ++it) {
			std::cout << std::left << std::setfill(' ') << std::setw(15) << (*it).first << (*it).second << std::endl;
		}
	}

	float AritComp(std::vector<ItemWInterval> & table, std::vector<std::string> & sequence)
	{
		float low = 0.f, high = 1.f;

		for (auto its = sequence.begin(); its != sequence.end(); ++its) {
			for (auto it = table.begin(); it != table.end(); ++it) {

				if ((*its) == (*it).color) {
					float range = high - low;

					high = low + range * (*it).high;
					low = low + range * (*it).low;

					break;
				}
			}
		}

		return (high + low) / 2.f;
	}

	void AritDecomp(std::vector<ItemWInterval> & table, float f, size_t symbCount)
	{
		int c = 0;
		while (c < symbCount) {
			for (auto it = table.begin(); it != table.end(); ++it) {
				if (f >= (*it).low && f <= (*it).high) {
					std::cout << (*it).color << std::endl;

					f = (f - (*it).low) / ((*it).high - (*it).low);

					++c;
					break;
				}
			}
		}
	}

	int sign(int x)
	{
		return (x >= 0) ? 1 : -1;
	}
}