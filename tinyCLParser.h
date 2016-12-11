/*! \file tinyCLParser.h (C++ header file)
 \brief C++ simple header template library for command line parsing
*/

/*! \file
 \author Axel Elenga <axel_dan@hotmail.fr>
 \copyright LGPL
 \date Original version: February 2014 / Updated: Summer 2014 / Documented: Febr 2016
 
 This program is free software; you can redistribute it and/or modify
it under the terms of the Lesser GNU General Public License as published by
the Free Software Foundation.\n
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
Lesser GNU General Public License for more details.
*/

#ifndef tinyCLParser_H
#define tinyCLParser_H
#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <vector>

//! Contains all tools of the \b tinyCLParser library
namespace tinyclparser
{

/*! \brief Initialization trait class to handle default value of data type

 \par How to use
 It is possible to define specific initialization trait for another type by using template specialization
 Example:
 \code
	template<>
	struct InitializationTraits<float>
	{
		static float init()
		{ return 0.0; }
	};
 \endcode 
*/
template<typename T>
struct InitializationTraits
{
	static T init()
	{}
};

//!  Initialization trait class specialization for \c int type
template<>
struct InitializationTraits<int>
{
	static int init()
	{ return 0; }
};

//! Initialization trait class specialization for \c double type
template<>
struct InitializationTraits<double>
{
	static double init()
	{ return 0.0; }
};

//! Initialization trait class specialization for \c std::string type
template<>
struct InitializationTraits<std::string>
{
	static std::string init()
	{ return std::string(); }
};

//! Initialization trait class specialization for \c bool type
template<>
struct InitializationTraits<bool>
{
	static bool init()
	{ return false; }
};

/*! \brief Casting trait class to wrap casting method from \c char* to data \c type

 \par How to use
 It is possible to define specific casting trait for another type by using template specialization
 Example:
 \code
	template<>
	struct CastingTraits<float>
	{
		static float cast(const char* str)
		{ return atof(str); }
	};
 \endcode 
*/
template<typename T>
struct CastingTraits
{
	static T cast(const char* str)
	{}
};

//! Casting trait class specialization from \c char* to \c int type
template<>
struct CastingTraits<int>
{
	static int cast(const char* str)
	{ return atoi(str); }
};

//! Casting trait class specialization from \c char* to \c double type
template<>
struct CastingTraits<double>
{
	static double cast(const char* str)
	{ return atof(str); }
};

//! Casting trait class specialization from \c char* to \c std::string type
template<>
struct CastingTraits<std::string>
{
	static std::string cast(const char* str)
	{ return std::string(str); }
};

//! Casting trait class specialization from \c char* to \c bool type
template<>
struct CastingTraits<bool>
{
	static bool cast(const char* str)
	{ return true; }
};


/*! \brief Main class to process command line program inputs

 \par How it works
 For a standard command line program, the \c TinyCLParser class takes as inputs main function \c int argc and \c char** argv parameters. 
 It parses argv to extract parameters of interest with the correct value and type
 
 \par How to use
 Example:
 \code
	TinyCLParser clp(argc,argv);
	string img_file = clp.parse<string>("-img", "default", "Image to show");
	bool interp = clp.parse<bool>("-poly", true, "Use polynomial interpolation"); 
 \endcode
 
*/
class TinyCLParser
{
public:
	TinyCLParser(int argc = 0, char** argv = 0x0): m_argc(argc), m_argv(argv), m_vecOpt() {}
	~TinyCLParser() {}
	void setMainArgs(int argc, char **argv) { m_argc = argc; m_argv = argv; m_vecOpt = std::vector<ClOption>(); }
	template <typename T>
	T parse(const char* opt, const T& def_val = InitializationTraits<T>::init(), const std::string& details = " ")
	{
		T arg(def_val);

		for(int i = 1; i < m_argc; i++)
		{
			if(std::string(m_argv[i]) == std::string(opt))
			{
				arg = CastingTraits<T>::cast(m_argv[i+1]);
				break;
			}
		}
		m_vecOpt.push_back(ClOption().parseOpt<T>(opt, details, def_val));

		return arg;
	}

	void usage(const std::string& title)
	{
		std::string pname(m_argv[0]);
		size_t found = pname.find_last_of("/\\");
		pname = pname.substr(found+1);

		std::cout << title << std::endl
			 << pname;

		for(size_t i = 0; i < m_vecOpt.size(); i++)
			std::cout << " ["  << m_vecOpt[i].m_opt << "] ";

		std::cout << std::endl;

		for(size_t i = 0; i < m_vecOpt.size(); i++)
			std::cout << "\t [" << m_vecOpt[i].m_opt <<  "]\t\t " << m_vecOpt[i].m_details << std::endl;
	}

private:
	struct ClOption
	{
		std::string m_opt, m_details, m_def_val;
		template <typename T>
		ClOption& parseOpt(const std::string& opt, const std::string& details, const T& def_val)
		{
			m_opt = opt;
			m_details = details;
			std::ostringstream oss; oss << def_val;
			m_def_val = oss.str();
			return *this;
		}

	};
	int m_argc;
	char** m_argv;
	std::vector<ClOption> m_vecOpt;
};

}// End namespace tinyclparser
#endif
