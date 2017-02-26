#pragma once

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.hpp"

namespace lab1
{
	void ex1()
	{
		std::cout << "=============== EX 1:" << std::endl << std::endl;
		const int imgSz = 256;
		std::map<std::string, int> frequency;

		frequency["Negru"] = 100;
		frequency["Alb"] = 100;
		frequency["Galben"] = 20;
		frequency["Portocaliu"] = 5;
		frequency["Rosu"] = 5;
		frequency["Purpuriu"] = 3;
		frequency["Albastru"] = 20;
		frequency["Verde"] = 3;

		std::vector<utils::ItemFreq> table(frequency.size());
		int i;
		std::map<std::string, int>::iterator it;
		for (it = frequency.begin(), i = 0; it != frequency.end(); ++it, ++i) {
			table[i].color = (*it).first;
			table[i].freq = float((*it).second) / imgSz;
		}

		std::sort(table.begin(), table.end(), 
			[](const utils::ItemFreq &a, const utils::ItemFreq &b) -> int
		{
			return a.freq < b.freq;
		});

		utils::InitShannonFano(table);
		utils::ShannonFano(table.begin(), table.end());
		utils::PrintShannonFano();
	}

	void ex2()
	{
		std::cout << std::endl << std::endl << "=============== EX 2:" << std::endl << std::endl;
		const int imgSz = 100;
		std::map<std::string, int> frequency;

		frequency["Negru"] = 40;
		frequency["Alb"] = 25;
		frequency["Galben"] = 15;
		frequency["Rosu"] = 10;
		frequency["Albastru"] = 10;

		std::vector<utils::ItemWInterval> table(frequency.size());
		int i;
		std::map<std::string, int>::iterator it;
		for (it = frequency.begin(), i = 0; it != frequency.end(); ++it, ++i) {
			table[i].color = (*it).first;
			table[i].freq = float((*it).second) / imgSz;
		}

		std::sort(table.begin(), table.end(),
			[](const utils::ItemWInterval &a, const utils::ItemWInterval &b) -> int
		{
			return a.freq > b.freq;
		});

		float crt = 0.f, end = 1.f;
		for (auto itt = table.begin(); itt != table.end(); ++itt) {
			(*itt).low = crt;
			(*itt).high = crt + (*itt).freq;
			crt += (*itt).freq;
		}

		std::vector<std::string> seq = {"Alb", "Negru", "Negru", "Galben", "Rosu", "Albastru"};
		float f = utils::AritComp(table, seq);
		std::cout << "Valoare compresie artimetica: " << f << std::endl;

		utils::AritDecomp(table, f, seq.size());
	}
}