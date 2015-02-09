/*
 * pstate-frequency Easier control of the Intel p-state driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * For questions please contact pyamsoft at pyam.soft@gmail.com
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "include/psfreq_color.h"
#include "include/psfreq_logger.h"
#include "include/psfreq_sysfs.h"

namespace psfreq {

	sysfs::sysfs()
	{
		basePath = "/sys/devices/system/cpu/";
	}

	sysfs::~sysfs()
	{
	}

	void sysfs::write(const std::string &path, const std::string &file, const std::string &buffer) const
	{
		std::ostringstream log;
		log << "pstate-frequency [psfreq_sysfs.cpp]: sysfs::write" << std::endl;
		psfreq::logger::d(log);

		std::ostringstream oss;
		oss << path << file;
		const std::string absolutePath = oss.str();
		log << "\tPath: " << absolutePath << std::endl;
		log << "\tOpening outputFile..." << std::endl;
		psfreq::logger::d(log);

		std::ofstream outputFile;
		outputFile.open(absolutePath.c_str());
		if (!outputFile.is_open()) {
			std::ostringstream oss;
			oss << PSFREQ_COLOR_BOLD_RED << "Output file: " << absolutePath
				<< " could not be opened." << PSFREQ_COLOR_OFF << std::endl;
			psfreq::logger::e(oss.str());
			psfreq::logger::close();
			exit(EXIT_FAILURE);
		}
		log << "\tWritting buffer: " << buffer << " to outputFile: "
			<< absolutePath << std::endl;
		psfreq::logger::d(log);
		outputFile << buffer << std::endl;

		log << "\tClosing outputFile..." << std::endl;
		psfreq::logger::d(log);
		outputFile.close();
	}

	void sysfs::write(const std::string &path,const std::string &file, const int number) const
	{
		std::ostringstream oss;
		oss << number;
		write(path, file, oss.str());
	}

	void sysfs::write(const std::string &file, const std::string &buffer) const
	{
		write(basePath, file, buffer);
	}

	void sysfs::write(const std::string &file, const int number) const
	{
		write(basePath, file, number);
	}

	const std::string sysfs::read(const std::string &path, const std::string &file) const
	{
		std::ostringstream log;
		log << "pstate-frequency [psfreq_sysfs.cpp]: sysfs::read" << std::endl;
		psfreq::logger::d(log);

		std::ostringstream oss;
		oss << path << file;
		const std::string absolutePath = oss.str();
		log << "\tPath: " << absolutePath << std::endl;
		log << "\tOpening inputFile..." << std::endl;
		psfreq::logger::d(log);

		std::string content;
		std::ifstream inputFile;
		inputFile.open(absolutePath.c_str());
		if (!inputFile.is_open()) {
			std::ostringstream oss;
			oss << PSFREQ_COLOR_BOLD_RED << "Input file: " << absolutePath
				<< " could not be opened." << PSFREQ_COLOR_OFF << std::endl;
			psfreq::logger::e(oss.str());
			psfreq::logger::close();
			exit(EXIT_FAILURE);
		}
		log << "\tGetting a line from inputFile..." << std::endl;
		psfreq::logger::d(log);
		std::getline(inputFile, content);

		log << "\tClosing inputFile..." << std::endl;
		psfreq::logger::d(log);

		inputFile.close();
		return content;
	}

	const std::string sysfs::read(const std::string &file) const
	{
		return read(basePath, file);
	}

	const std::vector<std::string> sysfs::readAll(const std::string &file) const
	{
		return readAll(basePath, file);
	}

	const std::vector<std::string> sysfs::readAll(const std::string &path, const std::string &file) const
	{
		std::ostringstream log;
		log << "pstate-frequency [psfreq_sysfs.cpp]: sysfs::readAll" << std::endl;
		psfreq::logger::d(log);

		std::ostringstream oss;
		oss << path << file;
		const std::string absolutePath = oss.str();
		log << "\tPath: " << absolutePath << std::endl;
		log << "\tOpening inputFile..." << std::endl;
		psfreq::logger::d(log);

		std::ifstream inputFile;
		inputFile.open(absolutePath.c_str());
		if (!inputFile.is_open()) {
			std::ostringstream oss;
			oss << PSFREQ_COLOR_BOLD_RED << "Input file: " << absolutePath
				<< " could not be opened." << PSFREQ_COLOR_OFF << std::endl;
			psfreq::logger::e(oss.str());
			psfreq::logger::close();
			exit(EXIT_FAILURE);
		}
		std::vector<std::string> contents = std::vector<std::string>();
		while (true) {
			log << "\tReading single input from inputFile..." << std::endl;
			psfreq::logger::d(log);
			std::string content;
			inputFile >> content;
			if (inputFile.eof()) {
				log << "\tEOF reached." << std::endl;
				psfreq::logger::d(log);
				break;
			}
			contents.push_back(content);
		}
		log << "\tClosing inputFile..." << std::endl;
		psfreq::logger::d(log);

		inputFile.close();
		return contents;
	}
}